#pragma once

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2();

	Vec2(float xin, float yin);

	bool operator ==(const Vec2& rhs) const;
	bool operator !=(const Vec2& rhs) const;

	Vec2 operator +(const Vec2& rhs) const;
	Vec2 operator -(const Vec2& rhs) const;
	Vec2 operator /(float rhs) const;
	Vec2 operator *(float rhs) const;

	void operator +=(const Vec2& rhs);
	void operator -=(const Vec2& rhs);
	void operator *=(float rhs);
	void operator /=(float rhs);

	float dist(const Vec2& rhs) const;
	float mag() const;
	Vec2 norm() const;
};