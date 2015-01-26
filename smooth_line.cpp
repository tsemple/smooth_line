#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <streambuf>

#include "vec2.h"

#ifdef _WIN32
#include <windows.h>
#include "glew\glew.h"
#include "SDL2/include/SDL.h"
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew\\glew32.lib")
#pragma comment(lib, "SDL2\\lib\\x86\\SDL2.lib")
#endif // WIN32

#ifdef __linux
// I needed apt-get install build-essential mesa-common-dev libglu1-mesa-dev libglew-dev libsdl2-dev
#include <GL/glew.h>
#include <SDL2/SDL.h>
#endif // __linux

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <SDL2/SDL.h>
#define glGetObjectParameterivARB(a,b,c) glGetObjectParameterivARB((void*)(ptrdiff_t)a, b, c);
#endif // __APPLE__

void demo_init();
void demo_render();

SDL_Window* gSDLWindow = NULL;

#ifdef _WIN32
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	SDL_assert(result >= 0);

	gSDLWindow = SDL_CreateWindow("smooth line", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_assert(gSDLWindow != NULL);

	SDL_GLContext context = SDL_GL_CreateContext(gSDLWindow);
	SDL_assert(context != NULL);

	demo_init();

	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		demo_render();

		SDL_GL_SwapWindow(gSDLWindow);
	}

	SDL_DestroyWindow(gSDLWindow);
	SDL_Quit();

	return 0;
}

GLuint g_bvbo;
GLuint g_program_object = 0;

GLuint compile_shader(const char* file_path, GLenum shader_type)
{
	std::ifstream shader_file(file_path);
	std::string shader_str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());

	GLuint shader_object = glCreateShader(shader_type);

	const char* shader_ptr = shader_str.c_str();
	int len = shader_str.length();
	glShaderSource(shader_object, 1, &shader_ptr, &len);

	glCompileShader(shader_object);

	GLint compiled;
	glGetObjectParameterivARB(shader_object, GL_COMPILE_STATUS, &compiled);

	if (!compiled) 
	{
		GLint infoLen = 0;
		glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &infoLen);
		SDL_assert(infoLen);

		char* buf = (char*)alloca(infoLen);
		SDL_assert(buf);

		glGetShaderInfoLog(shader_object, infoLen, NULL, buf);
		SDL_Log("Could not compile shader %s:\n%s\n", file_path, buf);
		SDL_assert(0);
	}

	return shader_object;
}

struct VertexData
{
	float x;
	float y;
	float red;
	float green;
	float blue;
	float u;
	float v;
	float width;
	float length;
};

const int VERT_SIZE = sizeof(VertexData);
const int VERTS_PER_QUAD = 4;
const int MAX_QUADS = 100;

VertexData g_vertices[MAX_QUADS * VERTS_PER_QUAD * VERT_SIZE];
int g_queued_quads = 0;

void demo_init()
{
#ifndef __APPLE__	
	GLenum initResult = glewInit();
	SDL_assert(initResult == GLEW_OK);
	SDL_assert(GLEW_VERSION_2_1);
#endif // __APPLE__

	GLuint vertex_shader_object = compile_shader("vs.glsl", GL_VERTEX_SHADER);
	GLuint fragment_shader_object = compile_shader("fs.glsl", GL_FRAGMENT_SHADER);

	g_program_object = glCreateProgram();

	glAttachShader(g_program_object, vertex_shader_object);
	glAttachShader(g_program_object, fragment_shader_object);

	glLinkProgram(g_program_object);

	GLint linked;
	glGetObjectParameterivARB(g_program_object, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(g_program_object, GL_INFO_LOG_LENGTH, &infoLen);
		SDL_assert(infoLen);

		char* buf = (char*)alloca(infoLen);
		SDL_assert(buf);

		glGetProgramInfoLog(g_program_object, infoLen, NULL, buf);
		SDL_Log("Could not compile shader:\n%s\n", buf);
		SDL_assert(0);
	}

	glUseProgram(g_program_object);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &g_bvbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_bvbo);

	GLsizei stride = sizeof(VertexData);

	GLuint pos_attr = glGetAttribLocation(g_program_object, "position");
	if (pos_attr != -1)
	{
		glEnableVertexAttribArray(pos_attr);
		glVertexAttribPointer(pos_attr, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, x));
	}

	GLuint color_attr = glGetAttribLocation(g_program_object, "color_in");
	if (color_attr != -1)
	{
		glEnableVertexAttribArray(color_attr);
		glVertexAttribPointer(color_attr, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, red));
	}

	GLuint line_data_attr = glGetAttribLocation(g_program_object, "line_data_in");
	if (line_data_attr != -1)
	{
		glEnableVertexAttribArray(line_data_attr);
		glVertexAttribPointer(line_data_attr, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VertexData, u));
	}

	short ELEMENTS[6 * MAX_QUADS];
	for (int i = 0; i < MAX_QUADS; i++)
	{
		int base = i * 6;
		int vert = i * 4;
		ELEMENTS[base] = vert;
		ELEMENTS[base + 1] = vert + 1;
		ELEMENTS[base + 2] = vert + 2;
		ELEMENTS[base + 3] = vert + 2;
		ELEMENTS[base + 4] = vert + 1;
		ELEMENTS[base + 5] = vert + 3;
	}

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ELEMENTS), &ELEMENTS, GL_STATIC_DRAW);

	int width;
	int height;
	SDL_GetWindowSize(gSDLWindow, &width, &height);

	GLfloat world_matrix[16] = {
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
	};

	GLint loc = glGetUniformLocation(g_program_object, "gWorld");
	glUniformMatrix4fv(loc, 1, GL_FALSE, world_matrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void demo_append_quad(VertexData* vertex_data)
{
	SDL_assert(g_queued_quads < MAX_QUADS);
	memcpy(&g_vertices[g_queued_quads * VERTS_PER_QUAD], vertex_data, sizeof(VertexData) * VERTS_PER_QUAD);
	g_queued_quads++;
}

void demo_draw_line(Vec2 start, Vec2 end, float width, float r, float g, float b)
{
	Vec2 along = Norm(start - end);
	Vec2 perp = Perp(along);

	// We draw the line an extra 1/2 pixel on all sides. This matches what the Photoshop line tools does.

	float length = Len(start - end);
	float half_length = (length + 1.0f) * 0.5f;
	float half_width = (width + 1.0f) * 0.5f;

	Vec2 corner[4];
	corner[0] = start + (along * 0.5f) + (perp * half_width);
	corner[1] = start + (along * 0.5f) - (perp * half_width);
	corner[2] = end - (along * 0.5f) + (perp * half_width);
	corner[3] = end - (along * 0.5f) - (perp * half_width);

	const int quads = 2;

	VertexData quad1[VERTS_PER_QUAD] = {
		{corner[0].x,	corner[0].y,	r, g, b,   half_width, -half_length, half_width, half_length, },
		{corner[1].x,	corner[1].y,	r, g, b,   -half_width, -half_length, half_width, half_length, },
		{corner[2].x,	corner[2].y,	r, g, b,   half_width, half_length, half_width, half_length, },
		{corner[3].x,	corner[3].y,	r, g, b,   -half_width, half_length, half_width, half_length,  },
	};

	demo_append_quad(quad1);
}

void demo_render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	demo_draw_line(V2(100.0f, 100.0f), V2(200.0f, 110.0f), 10.0f, 1.0f, 1.0f, 1.0f);

	demo_draw_line(V2(100.0f, 150.0f), V2(200.0f, 150.0f), 10.0f, 1.0f, 0.0f, 0.0f);

	demo_draw_line(V2(100.0f, 250.0f), V2(200.0f, 180.0f), 1.0f, 1.0f, 0.0f, 1.0f);

	demo_draw_line(V2(100.0f, 300.0f), V2(200.0f, 400.0f), 1.0f, 1.0f, 1.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, g_bvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * VERTS_PER_QUAD * g_queued_quads, g_vertices, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, g_queued_quads * 6, GL_UNSIGNED_SHORT, 0);
	g_queued_quads = 0;
}
