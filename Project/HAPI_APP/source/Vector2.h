#pragma once

// Required for sqrt:
#include <cmath>

class Vector2
{
public:

	float x = 0.F;
	float y = 0.F;

	Vector2() = default;
	Vector2(float xVal, float yVal) : x(xVal), y(yVal) {}

	// + operator. Returns the result of adding the rhs to this
	Vector2 operator + (const Vector2 &rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	// - operator. Returns the result of subtracting the rhs from this
	Vector2 operator - (const Vector2 &rhs) const 
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	// * operator. Returns the result of multiplying the rhs scaler to this
	Vector2 operator * (float rhs) const
	{
		return Vector2(x * rhs, y * rhs);
	}

	// *= operator. Alters this to be the result of multiplying by the rhs scaler. 
	// Returns a reference to this for chaining purposes
	Vector2& operator *= (float rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	Vector2& operator += (float rhs)
	{
		x += rhs;
		y += rhs;

		return *this;
	}

	Vector2& operator -= (float rhs)
	{
		x -= rhs;
		y -= rhs;

		return *this;
	}

	// Returns the length (magnitude) of this vector
	// Note: the sqrt makes this a relatively slow operation so it is common to 
	// add a SquaredLength fn that returns the squared length for use in comparisons etc.
	float Length() const
	{
		return sqrt(x * x + y * y);
	}

	// Normalises this vector so its length is 1
	// The resulting vector is known as a unit vector
	// Useful when you wish to just use the vector as a direction
	void NormaliseInPlace()
	{
		float length = Length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
	}

	
	// The dot product can be used to calculate the angle between 2 vectors.
	// If both vectors are already unit vectors then the dot product returns the
	// cosine of the angle between them. If they are not unit vectors then you must divide
	// the result by the product of the vector lengths to get the cosine of the angle.
	// Another use of the dot product is to find the distance of a point to a plane
	float Dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}
		
	// The cross product gives a value which is 2 times the area of the triangle.
	// Can also be used to find which rotation direction a vector is, relative to another (+-)
	float Cross(const Vector2 &other) const
	{
		return x * other.y - y * other.x;
	}
}; 

