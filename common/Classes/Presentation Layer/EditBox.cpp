#include "../../Stdafx.h"

bool EditBox::SetLines(bool isMultiLine)
{
	if (isMultiLine == false)
		this->Style(this->singleLinesStyle);
	else
		this->Style(this->multipleLinesStyle);

	this->multipleLines = isMultiLine;

	return this->multipleLines;
}

string EditBox::Text(string text)
{
	this->text = text;
	SetWindowText(this->HWnd(), this->text.c_str());

	return text;
}

string EditBox::Text()
{
	char buffer[65536];
	int txtlen=GetWindowTextLength(this->HWnd());
	if (txtlen < 65536)
		GetWindowText(this->HWnd(), buffer, txtlen+1);

	this->Text(buffer);
	return this->text;
}

string EditBox::operator<< (string in)
{
	this->Text(in);
	return in;
}

UINT EditBox::operator== (string in)
{
	if (this->Text() == in)
		return 1;
	else
		return 0;
}

void EditBox::operator+= (string in)
{
	char buffer[16];
	buffer[0] = 0;

	if (this->requireTimestamp == true)
	{
		time_t rawtime;
		struct tm timeinfo;

		time ( &rawtime );
		localtime_s (&timeinfo, &rawtime);

		strftime (buffer,16,"[%a %H:%M:%S] ",&timeinfo);
	}

	stringstream result;
	result << this->Text();
	if (result.str().size() > 0)
		result << "\r\n";
	result << buffer << in;
	this->Text(result.str());

	this->Text(this->Text());
	this->ScrollTo(SB_BOTTOM);
}

void EditBox::SetBox(Interface *parentWindow, string text, bool multipleLines, bool readOnly, UINT x, UINT y, UINT width, UINT height)
{
	this->SetLines(multipleLines);

	if (readOnly == true)
		this->Style(this->Style()|ES_READONLY);

	SetAll(parentWindow->HWnd(), parentWindow->ParentClassName(), parentWindow->HInstance(), text, x, y, width, height);
	CreateWindowMethod();
}
void EditBox::SetBox(Interface *parentWindow, string text, bool multiLines, bool readOnly, Position *positionData)
{
	this->SetBox(parentWindow, text, multiLines, readOnly, positionData->X(), positionData->Y(), positionData->Width(), positionData->Height());
}
void EditBox::SetBox(Interface *parentWindow, string text, bool multiLines, bool readOnly)
{
	this->SetBox(parentWindow, text, multiLines, readOnly, this->X(), this->Y(), this->Width(), this->Height());
}

unsigned long int EditBox::SetBoxStyle(bool changeMultipleLines, unsigned long int style, bool append)
{
	if (changeMultipleLines == true)
	{
		if (append == true)
			this->multipleLinesStyle |= style;
		else
			this->multipleLinesStyle = style;

		return this->multipleLinesStyle;
	}
	else
	{
		if (append == true)
			this->singleLinesStyle |= style;
		else
			this->singleLinesStyle = style;

		return this->singleLinesStyle;
	}
}

void EditBox::RequireTimestamp(bool value)
{
	this->requireTimestamp = value;
}

void EditBox::ScrollTo(UINT query)
{
	/*UINT mode=SB_TOP;

	if (query == "bottom")
	mode = SB_BOTTOM;
	else if (query == "linedown")
	mode = SB_LINEDOWN;
	else if (query == "lineup")
	mode = SB_LINEUP;
	else if (query == "pageup")
	mode = SB_PAGEUP;
	else if (query == "pagedown")
	mode = SB_PAGEDOWN;
	else if (query == "top")
	mode = SB_TOP;*/

	SendMessage(this->HWnd(), WM_VSCROLL, query, 0);
}

void EditBox::saveContentToFile(Interface *mainWindow)
{
	OPENFILENAME saveFileDialog;
	char szSaveFileName[MAX_PATH];
	ZeroMemory(&saveFileDialog, sizeof(saveFileDialog));
	for(unsigned int i=0;i<MAX_PATH;i++)
		szSaveFileName[i] = 0;
	saveFileDialog.lStructSize= sizeof(saveFileDialog);
	saveFileDialog.hwndOwner = mainWindow->HWnd();
	saveFileDialog.hInstance = NULL;
	saveFileDialog.lpstrFilter = "Text File (*.txt)\0*.txt\0\0";
	saveFileDialog.nFilterIndex = 0;
	saveFileDialog.lpstrFile = szSaveFileName;
	saveFileDialog.nMaxFile = sizeof(szSaveFileName);
	//saveFileDialog.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT;
	saveFileDialog.Flags = OFN_EXPLORER;	
	saveFileDialog.lpstrTitle = "Save as...";
	if(GetSaveFileName(&saveFileDialog))
	{
		string fileName = szSaveFileName;
		size_t found = fileName.find_last_of('.');
		if (found == string::npos)
			fileName.append(".txt");

		ofstream file;

		file.open(fileName, ios_base::trunc);
		if (file.is_open())
		{
			file << this->Text();
			file.close();

			stringstream buffer;
			buffer.str("");
			buffer << "File saved to: " << fileName;
			MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Success", MB_ICONINFORMATION);
		}
		else
			MessageBox(mainWindow->HWnd(), "Error occured. Try again.", "Error", MB_ICONERROR);
	}
	else
	{
		if (CommDlgExtendedError() != 0)
		{
			stringstream buffer;
			buffer.str("");
			buffer << "Error no " << CommDlgExtendedError();
			MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Error", MB_ICONERROR);
		}
	}					
}