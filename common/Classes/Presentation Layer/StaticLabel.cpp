#include "../../Stdafx.h"

string StaticLabel::Text(string text)
{
	if (text != "")
	{
		this->text = text;
		SetWindowText(this->HWnd(), this->text.c_str());
	}
	else
	{
		char buffer[65536];
		int txtlen=GetWindowTextLength(this->HWnd());
		if (txtlen < 65536)
			GetWindowText(this->HWnd(), buffer, txtlen+1);

		text = buffer;	
	}

	this->text = text;
	return text;
}

void StaticLabel::SetLabel(Interface *parentWindow, string text, INT x, INT y, UINT width, UINT height)
{
	SetAll(parentWindow->HWnd(), parentWindow->ParentClassName(), parentWindow->HInstance(), text, x, y, width, height);
	CreateWindowMethod();
}
void StaticLabel::SetLabel(Interface *parentWindow, string text, Position *positionData)
{
	this->SetLabel(parentWindow, text, positionData->X(), positionData->Y(), positionData->Width(), positionData->Height());
}
void StaticLabel::SetLabel(Interface *parentWindow, string text)
{
	this->SetLabel(parentWindow, text, this->X(), this->Y(), this->Width(), this->Height());
}