/*-----------------------------------------------------------------------------

	BasicData.cpp

	Basic data class implementations.

	Ported to macOS for Dark Eden client.

-----------------------------------------------------------------------------*/

#include "BasicData.h"

//----------------------------------------------------------------------------
// Point
//----------------------------------------------------------------------------
Point::Point(int _x, int _y)
: x(_x), y(_y)
{
}

Point::Point()
: x(0), y(0)
{
}

Point::~Point()
{
}

void Point::Set(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Point::operator=(int val)
{
	x = val;
	y = val;
}

//----------------------------------------------------------------------------
// Rect
//----------------------------------------------------------------------------
Rect::Rect(int _x, int _y, int _w, int _h)
: x(_x), y(_y), w(_w), h(_h)
{
}

Rect::Rect()
: x(0), y(0), w(0), h(0)
{
}

Rect::~Rect()
{
}

void Rect::XY(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Rect::WH(int _w, int _h)
{
	w = _w;
	h = _h;
}

void Rect::Set(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

void Rect::operator=(int val)
{
	x = val;
	y = val;
	w = val;
	h = val;
}

void Rect::operator=(Point &point)
{
	x = point.x;
	y = point.y;
}

void Rect::operator+=(Point &point)
{
	x += point.x;
	y += point.y;
}

bool Rect::IsInRect(int _x, int _y) const
{
	return (_x >= x && _x < x + w && _y >= y && _y < y + h);
}

bool Rect::IsInRect(const Point &point) const
{
	return (point.x >= x && point.x < x + w && point.y >= y && point.y < y + h);
}

int Rect::Left() const
{
	return x;
}

int Rect::Right() const
{
	return x + w;
}

int Rect::Up() const
{
	return y;
}

int Rect::Down() const
{
	return y + h;
}
