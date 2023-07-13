#pragma once

class Vec2
{
public:
    float x = 0;
    float y = 0;

    Vec2();
    Vec2(float xin, float yin);

    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    float operator/(const Vec2& other) const;
    float operator*(const Vec2& other) const;

    void operator+=(const Vec2& other);
    void operator-=(const Vec2& other);
    void operator*=(const float value);
    void operator/=(const float value);

    float dist(const Vec2& other) const;
    float length() const;

    Vec2& normalize();
    Vec2& multiply(const float in);
};
