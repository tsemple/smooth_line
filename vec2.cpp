// vec2.cpp
#include "vec2.h"
#include <math.h>

const Vec2 V20 = {0.0f, 0.0f};
const Vec2 V21 = {1.0f, 1.0f};
const Vec2 V2X = {1.0f, 0.0f};
const Vec2 V2Y = {0.0f, 1.0f};

Vec2 V2(float x, float y)
{
	Vec2 retval;
	retval.x = x;
	retval.y = y;
	return retval;
}

bool operator ==(const Vec2& v1, const Vec2& v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}

bool operator !=(const Vec2& v1, const Vec2& v2)
{
	return v1.x != v2.x || v1.y != v2.y;
}

void operator +=(Vec2& v1, const Vec2& v2)
{
	v1.x += v2.x; 
	v1.y += v2.y; 
}

Vec2 operator +(const Vec2& v1, const Vec2& v2)
{
	Vec2 retval;
	retval.x = v1.x + v2.x; 
	retval.y = v1.y + v2.y; 
	return retval;
}

void operator -=(Vec2& v1, const Vec2& v2)
{
	v1.x -= v2.x; 
	v1.y -= v2.y; 
}

Vec2 operator -(const Vec2& v1, const Vec2& v2)
{
	Vec2 retval;
	retval.x = v1.x - v2.x; 
	retval.y = v1.y - v2.y; 
	return retval;
}

void operator *=(Vec2& v, float f)
{
	v.x *= f; 
	v.y *= f; 
}

Vec2 operator *(const Vec2& v, float f)
{
	Vec2 retval;
	retval.x = v.x * f; 
	retval.y = v.y * f; 
	return retval;
}

void operator /=(Vec2& v, float f)
{
	float invF = 1.0f / f;
	v.x *= invF; 
	v.y *= invF; 
}

Vec2 operator /(const Vec2& v, float f)
{
	Vec2 retval;
	float invF = 1.0f / f;
	retval.x = v.x * invF; 
	retval.y = v.y * invF; 
	return retval;
}

Vec2 operator -(const Vec2& v)
{
	Vec2 retval;
	retval.x = -v.x;
	retval.y = -v.y;
	return retval;
}

Vec2 operator*(float f, const Vec2& v)
{
	Vec2 retval;
	retval.x = v.x * f; 
	retval.y = v.y * f; 
	return retval;
}

float Dot(const Vec2& v0,const Vec2& v1)
{
	return v0.x * v1.x + v0.y * v1.y;
}

float SqrLen(const Vec2& v0)
{
	return Dot(v0, v0);
}

float Len(const Vec2& v0)
{
	return sqrt(Dot(v0, v0));
}

float SqrDist(const Vec2& v0, const Vec2& v1)
{
	return SqrLen(v0 - v1);
}

float Dist(const Vec2& v0, const Vec2& v1)
{
	return Len(v0 - v1);
}

float Dist2D(float x1, float y1, float x2, float y2)
{
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

Vec2 Norm(const Vec2& v0)
{
	float length = Len(v0);
	return v0 / length;
}

Vec2 Lerp(const Vec2& v1, const Vec2& v2, float t)
{
	Vec2 retval;

	retval.x = (v2.x - v1.x) * t + v1.x;
	retval.y = (v2.y - v1.y) * t + v1.y;

	return retval;
}

Vec2 Perp(const Vec2& v)
{
	Vec2 retval;
	retval.x = v.y;
	retval.y = -v.x;
	return retval;
}

float Cross2(const Vec2 &a, const Vec2 &b)
{
	return (a.x*b.y) - (a.y*b.x);
}

Vec2 MulEl2(const Vec2& v0, const Vec2& v1)
{
	Vec2 retval;
	retval.x = v0.x * v1.x;
	retval.y = v0.y * v1.y;
	return retval;
}

Vec2 DivEl2(const Vec2& v0, const Vec2& v1)
{
	Vec2 retval;
	retval.x = v0.x / v1.x;
	retval.y = v0.y / v1.y;
	return retval;
}
