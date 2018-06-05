#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

// Defines a rectangle for the purpose of drawing partially off screen, and possibly collision in the future
class Rectangle
{
public:
	int _left = 0, _right = 0, _top = 0, _bottom = 0;

public:
	inline Rectangle(int left, int right, int top, int bottom) : _left(left), _right(right), _top(top), _bottom(bottom) {}
	inline ~Rectangle() {};

	// Clips this rectangle to the other.
	void ClipTo(const Rectangle& other);

	// Test if the other rectangle is completely inside/outside this one.
	bool CompletelyInside(const Rectangle& other) const;
	bool CompletelyOutside(const Rectangle& other) const;

	// Move the rectangle
	void Translate(int differenceX, int differenceY);

	// Getter Functions
	inline int GetWidth() const { return (_right - _left); }
	inline int GetHeight() const { return (_bottom - _top); }

	// Override operator to make coding easier
	void operator *=(const float rhs)
	{
		_left = (int)(_left * rhs);
		_right = (int)(_right * rhs);
		_top = (int)(_top * rhs);
		_bottom = (int)(_bottom * rhs);
	}
};

