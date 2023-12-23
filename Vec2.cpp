#include "Vec2.h"

#include <cmath>


/* Constructors */


Vec2::Vec2() = default;

Vec2::Vec2(float xin, float yin)
	: x(xin)
	, y(yin)
{
}


/* Operators */


bool Vec2::operator ==(const Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator !=(const Vec2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator +(const Vec2& rhs) const
{
	return { x + rhs.x, y + rhs.y };
}

Vec2 Vec2::operator -(const Vec2& rhs) const
{
	return { x - rhs.x, y - rhs.y };
}

Vec2 Vec2::operator /(const float rhs) const
{
	return { x / rhs, y / rhs };
}

Vec2 Vec2::operator *(const float rhs) const
{
	return { x * rhs, y * rhs };
}

void Vec2::operator +=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator *=(const float rhs)
{
	x *= rhs;
	y *= rhs;
}

void Vec2::operator /=(const float rhs)
{
	x /= rhs;
	y /= rhs;
}


/* Maths Methods */


float Vec2::dist(const Vec2& rhs) const
{
	return sqrtf(powf(x - rhs.x, 2) + powf(y - rhs.y, 2));
}

float Vec2::mag() const
{
	return dist({ 0.0f, 0.0f });
}

Vec2 Vec2::norm() const
{
	float len = mag();
	if (len != 0)
	{
		return { x / len, y / len };
	}
	return { 0.0f, 0.0f };
}