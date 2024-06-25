#include "Vector2.h"
#include <cmath>

Vector2::Vector2(double x, double y):x(x),y(y)
{
}

Vector2 Vector2::operator+(const Vector2& vec) const
{
	return Vector2(x + vec.x, y + vec.y);
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& vec) const
{
	return Vector2(x - vec.x, y - vec.y);
}

double Vector2::operator*(const Vector2& vec) const
{
	return x * vec.x + y * vec.y;
}

Vector2 Vector2::operator*(double val) const
{
	return Vector2(x * val, y * val);
}

Vector2& Vector2::operator*=(double val)
{
	x *= val, y *= val;
	return *this;
}

bool Vector2::operator==(const Vector2 vec) const
{
	return x == vec.x && y == vec.y;
}

bool Vector2::operator>(const Vector2 vec) const
{
	return length() > vec.length();
}

bool Vector2::operator<(const Vector2 vec) const
{
	return length()<vec.length();
}

bool Vector2::approx_zero() const
{
	return length() < 0.0001;
}

double Vector2::length() const
{
	return sqrt(x * x+y * y);
}

Vector2 Vector2::normalize()
{
	double len = length();

	if (len == 0)
		return Vector2(0, 0);
	return Vector2(x / len, y / len);
}
