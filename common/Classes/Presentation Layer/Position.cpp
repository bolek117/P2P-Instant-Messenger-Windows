#include "../../Stdafx.h"

int Position::X(int value)
{
	if (value != -1)
		this->x = value;

	return this->x;
}

int Position::Y(int value)
{
	if (value != -1)
		this->y = value;

	return this->y;
}

UINT Position::Width(UINT value)
{
	if (value != 0)
		this->width = value;

	return this->width;		
}

UINT Position::Height(UINT value)
{
	if (value != 0)
		this->height = value;

	return this->height;		
}

void Position::AddTo(string parameter, int value)
{
	if (parameter == "x")
		this->x += value;

	if (parameter == "y")
		this->y += value;

	if (parameter == "width")
		this->width += value;

	if (parameter == "height")
		this->height += height;
}

/*void Position::AddToPosition(int x, int y, int width, int height)
{
	this->x += x;
	this->y += y;
	this->width += width;
	this->height += height;
}*/

void Position::SetPosition(int x, int y, int width, int height)
{
	if (x != 0)
		this->X(x);

	if (y != 0)
		this->Y(y);

	if (width != 0)
		this->Width(width);

	if (height != 0)
		this->Height(height);
}

int Position::ClearPosition(string value)
{
	if (value == "position")
	{
		this->x = 0;
		this->y = 0;
	} 
	else if (value == "size")
	{
		this->width = 0;
		this->height = 0;
	}
	else 
	{
		this->x = 0;
		this->y = 0;
		this->width = 0;
		this->height = 0;
	}

	return 1;
}