// vec2.h

struct Vec2
{
	float x;
	float y;
};

extern const Vec2 V20;
extern const Vec2 V21;
extern const Vec2 V2X;
extern const Vec2 V2Y;

Vec2 V2(float x, float y);

bool operator ==(const Vec2& v1, const Vec2& v2);
bool operator !=(const Vec2& v1, const Vec2& v2);
void operator +=(Vec2& v1, const Vec2& v2);
Vec2 operator +(const Vec2& v1, const Vec2& v2);
void operator -=(Vec2& v1, const Vec2& v2);
Vec2 operator -(const Vec2& v1, const Vec2& v2);
void operator *=(Vec2& v, float f);
Vec2 operator *(const Vec2& v, float f);
void operator /=(Vec2& v, float f); 
Vec2 operator /(const Vec2& v, float f);
Vec2 operator -(const Vec2& v);
Vec2 operator*(float f, const Vec2& v);
Vec2 MulEl2(const Vec2& v0, const Vec2& v1);
Vec2 DivEl2(const Vec2& v0, const Vec2& v1);
float Dot(const Vec2& v0,const Vec2& v1);
float SqrLen(const Vec2& v0);
float Len(const Vec2& v0);
float SqrDist(const Vec2& v0, const Vec2& v1);
float Dist(const Vec2& v0, const Vec2& v1);
float Dist2D(float x1, float y1, float x2, float y2);
Vec2 Norm(const Vec2& v0);
Vec2 Perp(const Vec2& v);
float Cross2(const Vec2 &a, const Vec2 &b);
Vec2 Lerp(const Vec2& v1, const Vec2& v2, float t);



