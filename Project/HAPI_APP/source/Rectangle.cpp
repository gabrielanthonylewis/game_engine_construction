#include "Rectangle.h"

void Rectangle::ClipTo(const Rectangle& other)
{
	if (_left < other._left) _left = other._left;
	if (_right > other._right) _right = other._right;
	if (_bottom > other._bottom) _bottom = other._bottom;
	if (_top < other._top) _top = other._top;
}

void Rectangle::Translate(int differenceX, int differenceY)
{
	_left += differenceX;
	_right += differenceX;
	_bottom += differenceY;
	_top += differenceY;
}

bool Rectangle::CompletelyInside(const Rectangle& other) const
{
	if (_right <= other._right
		&& _left >= other._left
		&& _top >= other._top
		&& _bottom <= other._bottom)
		return true;


	return false;
}

bool Rectangle::CompletelyOutside(const Rectangle& other) const
{
	if (other._right < _left)
		return true;

	if (other._left > _right)
		return true;

	if (other._bottom < _top)
		return true;

	if (other._top > _bottom)
		return true;

	return false;
}