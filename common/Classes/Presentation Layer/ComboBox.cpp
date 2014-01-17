#include "../../Stdafx.h"

int ComboBox::AddOption(string name)
{
	try
	{
		options.push_back(name);
		SendMessage(this->HWnd(), CB_ADDSTRING, 0, (LPARAM)name.c_str());
	}
	catch(...)
	{
		return 0;
	}

	return 1;
}

int ComboBox::SelectOption(UINT position)
{
	if (position >= 0 && position < options.size())
		SendMessage(this->HWnd(),CB_SETCURSEL,(WPARAM)position,0);
	else 
		return -1;

	return position;
}

int ComboBox::GetActualOption()
{
	int result;
	result = SendMessage(this->HWnd(),CB_GETCURSEL,0,0);
	return result;
}

int ComboBox::operator+= (string name)
{
	return AddOption(name);
}

int ComboBox::operator= (UINT option)
{
	return SelectOption(option);
}

int ComboBox::operator<< (string name)
{
	return AddOption(name);
}

int ComboBox::operator++ (int)
{
	int actualOption = GetActualOption();

	if (actualOption < 0) actualOption = 0;
	if ((UINT)actualOption < options.size())
	{
		SelectOption(actualOption+1);
		return actualOption+1;
	}
	else
		return actualOption;
}

int ComboBox::operator-- (int)
{
	int actualOption = GetActualOption();

	if (actualOption < 0) actualOption = 0;
	if (actualOption > 0)
	{
		SelectOption(actualOption-1);
		return actualOption-1;
	}
	else
		return actualOption;
}

int ComboBox::DeleteOption(UINT id)
{
	return SendMessage(this->HWnd(), CB_DELETESTRING, id, 0);
}

void ComboBox::SetBox(Interface *parentWindow, bool isDropdown, INT x, INT y, UINT width, UINT height)
{
	this->SetAll(parentWindow->HWnd(), parentWindow->Name(), parentWindow->HInstance(), "", x, y, width, height);

	if (isDropdown == true)
		this->Dropdown(1);
	else 
		this->Dropdown(-1);

	CreateWindowMethod();

	for(UINT i=0;i<options.size();i++)
	{
		SendMessage(this->HWnd(),CB_ADDSTRING,0,(LPARAM)options[i].c_str());
	}

	this->SelectOption(0);
}
void ComboBox::SetBox(Interface *parentWindow, bool isDropdown, Position *positionData)
{
	this->SetBox(parentWindow, isDropdown, positionData->X(), positionData->Y(), positionData->Width(), positionData->Height());
}
void ComboBox::SetBox(Interface *parentWindow, bool isDropdown)
{
	this->SetBox(parentWindow, isDropdown, this->X(), this->Y(), this->Width(), this->Height());
}
void ComboBox::SetBox(Interface *parentWindow)
{
	this->SetAll(parentWindow->HWnd(), parentWindow->Name(), parentWindow->HInstance(), "", this->X(), this->Y(), this->Width(), this->Height());
	CreateWindowMethod();

	for(UINT i=0;i<options.size();i++)
	{
		SendMessage(this->HWnd(),CB_ADDSTRING,0,(LPARAM)options[i].c_str());
	}

	this->SelectOption(0);
}

bool ComboBox::Dropdown(int mode)
{
	if (mode != 0)
	{
		if (mode < 0)
		{
			this->isDropdown = false;
			this->Style(this->Style() & ~(CBS_DROPDOWNLIST));	//disable bits
		}
		else
		{
			this->isDropdown = true;
			this->Style(this->Style()|CBS_DROPDOWNLIST);	//enable bits
		}
	}

	return this->isDropdown;
}