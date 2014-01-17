#include "../../Stdafx.h"

Interface::~Interface()
{
	if (this->hWnd != NULL)
	{
		DestroyWindow(hWnd);
	}
}

UINT Interface::Id(UINT id)
{
	if (id != 0)
		this->id = id;

	return this->id;
}

HWND Interface::HWnd(HWND newHWnd)
{
	if (newHWnd != NULL)
		this->hWnd = newHWnd;

	return this->hWnd;
}

LPCSTR Interface::Name(string newName)
{
	if (!newName.empty())
		this->name = newName;

	return (LPCSTR)this->name.c_str();
}

LPCSTR Interface::ParentClassName(string newName)
{
	if (!newName.empty())
		this->parentClassName = newName;

	return (LPCSTR)this->parentClassName.c_str();
}

string Interface::Type(string newType)
{
	if (newType != "")
		this->type = newType;

	return this->type;
}

HFONT Interface::SetFont(string fontName, bool update)
{
	if (fontName != "")
		this->font = CreateFont(0,0,0,0,0,0,0,0,0,0,0,0,0,fontName.c_str());

	if (update == true)
		UpdateFont();

	return this->font;
}

HFONT Interface::SetFont(HGDIOBJ stockFont, bool update)
{
	this->font = (HFONT)stockFont;

	if (update == true)
		UpdateFont();

	return this->font;
}

void Interface::UpdateFont()
{
	SendMessage(this->HWnd(), WM_SETFONT, (WPARAM)this->font, 0); 
}

UINT Interface::TabStop(bool mode, int positionInList)
{
	if (positionInList < 0)
	{
		mode = false;
		positionInList = -1;
	}

	this->tabStop = mode;
	this->tabStopId = positionInList;

	return 1;
}

long unsigned int Interface::Style(long unsigned int style, bool additional, bool reset)
{
	if (style != 0)
	{
		if (additional == false)
		{
			if (reset == true)
				this->style = style;
			else
				this->style |= style;

			return this->style;
		}
		else
		{
			if (reset == true)
				this->additionalStyles = style;
			else
				this->additionalStyles |= style;

			return this->additionalStyles;
		}
	}
	return this->style;
}

HWND Interface::CreateWindowMethod()
{
	this->hWnd = CreateWindowExA(this->additionalStyles,this->type.c_str(),this->name.c_str(),this->style,this->X(),this->Y(),this->Width(),this->Height(),this->hParent,this->hMenu,this->hInstance,0);

	if (this->hWnd == NULL) 
	{
		stringstream text;
		text << "WinAPI error " << GetLastError();
		MessageBox(HWND_DESKTOP,text.str().c_str(),"Error",MB_OK);
	}

	UpdateFont();

	return this->hWnd;
}

HINSTANCE Interface::HInstance(HINSTANCE pointer)
{
	if (pointer != NULL)
		this->hInstance = pointer;

	return this->hInstance;
}

long unsigned int Interface::AdditionalStyles(long unsigned int additionalStyles)
{
	if (additionalStyles != 0)
		this->additionalStyles = additionalStyles;

	return this->additionalStyles;
}

HWND Interface::HParent(HWND pointer)
{
	if (pointer != NULL)
		this->hParent = pointer;

	return this->hParent;
}

void Interface::SetAll(HWND hParent, string parentClassName, HINSTANCE hInstance, string name, INT x, INT y, UINT width, UINT height)
{
	this->HParent(hParent);
	this->ParentClassName((LPCSTR)parentClassName.c_str());
	this->HInstance(hInstance);
	this->Name(name);
	this->X(x);
	this->Y(y);
	this->Width(width);
	this->Height(height);
}

HMENU Interface::HMenu(int id)
{
	if (id >= 0)
		this->hMenu = (HMENU)id;

	return this->hMenu;
}

void Interface::SetFocus()
{
	SendMessage(this->HWnd(), WM_SETFOCUS, 0, 0);
}