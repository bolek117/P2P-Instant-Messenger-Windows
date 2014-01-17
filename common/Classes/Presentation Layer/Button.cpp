#include "../../Stdafx.h"

void Button::SetButton(Interface *parentWindow, string text, UINT buttonId, INT x, INT y, UINT width, UINT height)
{
	SetAll(parentWindow->HWnd(), parentWindow->Name(), parentWindow->HInstance(), text, x, y, width, height);
	this->ButtonId(buttonId);
	CreateWindowMethod();
}
void Button::SetButton(Interface *parentWindow, string text, UINT buttonId, Position *positionData)
{
	this->SetButton(parentWindow, text, buttonId, positionData->X(), positionData->Y(), positionData->Width(), positionData->Height());
}
void Button::SetButton(Interface *parentWindow, string text, UINT buttonId)
{
	this->SetButton(parentWindow, text, buttonId, this->X(), this->Y(), this->Width(), this->Height());
}

string Button::Text(string value)
{
	if (value != "")
		Name(value);

	return this->Name();
}

HMENU Button::ButtonId(UINT id)
{
	if (id != -1)
	{
		this->buttonId = (HMENU)id;
		this->HMenu(id);
	}

	return this->buttonId;
}

HMENU Button::operator= (UINT id)
{
	return ButtonId(id);
}

string Button::operator<< (string text)
{
	return Text(text);
}