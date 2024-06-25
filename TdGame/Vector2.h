#pragma once
class Vector2
{
public:
    double x = 0;
    double y = 0;

public:
    Vector2() = default;
    ~Vector2() = default;

    Vector2(double x, double y);
    Vector2 operator+(const Vector2& vec) const;
    Vector2& operator+=(const Vector2& vec);
    Vector2 operator-(const Vector2& vec) const;
    Vector2& operator-=(const Vector2& vec);
    Vector2 operator*(double val) const;
    double operator*(const Vector2& vec) const;
    Vector2& operator*=(double val);
    bool operator==(const Vector2 vec) const;
    bool operator>(const Vector2 vec) const;
    bool operator<(const Vector2 vec) const;

    bool approx_zero()const;
    double length() const;

    Vector2 normalize();
};

