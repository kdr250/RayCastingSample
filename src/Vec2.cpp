#include "Vec2.h"
#include <cmath>

Vec2::Vec2() {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator==(const Vec2& other) const
{
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return x != other.x || y != other.y;
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(x - other.x, y - other.y);
}

float Vec2::operator/(const Vec2& other) const
{
    return x / other.x + y / other.y;
}

float Vec2::operator*(const Vec2& other) const
{
    return x * other.x + y * other.y;
}

void Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
}

void Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
}

void Vec2::operator*=(const float value)
{
    x *= value;
    y *= value;
}

void Vec2::operator/=(const float value)
{
    x /= value;
    y /= value;
}

float Vec2::dist(const Vec2& other) const
{
    return std::sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

float Vec2::length() const
{
    return std::sqrtf(x * x + y * y);
}

Vec2& Vec2::normalize()
{
    float len = length();
    x /= len;
    y /= len;
    return *this;
}

Vec2& Vec2::multiply(const float in)
{
    x *= in;
    y *= in;
    return *this;
}
