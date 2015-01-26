#version 120

attribute vec2 position;
attribute vec3 color_in;
attribute vec4 line_data_in;
uniform mat4 gWorld;
varying vec3 color;
varying vec4 line_data;

void main() 
{
    color = color_in;
    line_data = line_data_in;
    gl_Position = gWorld * vec4(position, 0.0, 1.0);
}
