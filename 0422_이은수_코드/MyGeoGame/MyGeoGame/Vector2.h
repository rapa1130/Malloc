#pragma once

struct Vector2
{
public:
	Vector2() : x(0), y(0) { }
	Vector2(float value) : x(value), y(value) { }
	Vector2(float x, float y) : x(x), y(y) { }
	Vector2(const Vector2& other) : x(other.x), y(other.y) { }

	Vector2& operator=(const Vector2& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}
	Vector2 operator/(float scalar) const
	{
		if (scalar != 0)
			return Vector2(x / scalar, y / scalar);
		else
			return Vector2(0, 0); // Avoid division by zero
	}
    Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& operator/=(float scalar)
    {
        if (scalar != 0)
        {
            x /= scalar;
            y /= scalar;
        }
        return *this;
    }

    bool operator==(const Vector2& other) const
    {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }

    Vector2 operator-() const
    {
        return { -x, -y };
    }
public:
	float x, y;
};