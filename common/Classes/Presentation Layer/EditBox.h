#ifndef EDITBOX_h
#define EDITBOX_h

class EditBox : public Interface
{
private:
	string text;
	bool multipleLines;
	unsigned long int singleLinesStyle;
	unsigned long int multipleLinesStyle;
	bool requireTimestamp;

public:
	EditBox() 
	{
		this->singleLinesStyle = WS_CHILD|WS_VISIBLE;
		this->multipleLinesStyle = this->singleLinesStyle|ES_MULTILINE;
		this->Style(this->singleLinesStyle);
		this->Type(WC_EDIT);
		this->AdditionalStyles(WS_EX_CLIENTEDGE);
	}

	///<summary>Set values specific to EditBox and display box to screen.</summary>
	///<param name="parentWindow">"Interface" object of parent window.</param>
	///<param name="text">Text inside of box.</param>
	///<param name="multiline">Allow multiple lines or not.</param>
	///<param name="x">Position x in px.</param>
	///<param name="y">Position y in px.</param>
	///<param name="width">Width of TextBox.</param>
	///<param name="height">Height of TextBox.</param>
	void SetBox(Interface *parentWindow, string text, bool multiline, bool readOnly, UINT x, UINT y, UINT width, UINT height);
	void SetBox(Interface *parentWindow, string text, bool multiline, bool readOnly, Position *positionData);
	void SetBox(Interface *parentWindow, string text, bool multiline, bool readOnly);

	///<summary>Get or set number of lines in textBox.</summary>
	///<param name="lines">If parameter is empty (or == 0), value of "lines" will be returned. &lt;0 for single lines mode, &gt;0 for multiple lines mode.</param>
	///<returns>Actual or updated state of "multipleLines"</returns>
	bool SetLines(bool isMultiLine);

	///<summary>Set text into textBox.</summary>
	///<param name="in">Text to load to textBox</param>
	///<returns>Updated text.</returns>
	string operator<< (string in);

	///<summary>Compare text from textBox.</summary>
	///<param name="in">String to compare.</param>
	///<returns>1 if equal, else 0.</returns>
	UINT operator == (string in);

	///<summary>Append text to textBox.</summary>
	///<param name="in">String to append.</param>
	void operator += (string in);

	///<summary>Get or set text in textBox</summary>
	///<param name="text">If parameter is empty (or == "&gt;&gt;empty&lt;&lt;", value of textBox will be returned. Else, given string will be set to textBox.</param>
	///<returns>Actual or updated value of textBox.</returns>
	string Text(string text);
	string Text();

	///<summary>Set singleLines/multipleLines style.</summary>
	///<param name="changeMultipleLines">Styles to change: true - multiple lines, false - single lines.</param>
	///<param name="style">Style of element.</param>
	///<param name="append">Determine if style shoulb be appended to current style or set as new style.</param>
	///<returns>Actual or updated value of style.</returns>
	unsigned long int SetBoxStyle(bool changeMultipleLines, unsigned long int style, bool append=true);

	///<summary>Scroll to position.</summary>
	///<param name="query">WM_VSCROLL Message.</param>
	void ScrollTo(UINT query);

	///<summary>Set value of requireTimestamp parameter.</summary>
	///<param name="value">New value of requireTimestamp.</param>
	void RequireTimestamp(bool value);

	///<summary>Save content of EditBox to file.</summary>
	///<param name="mainWindow">Pointer to mainWindow interface object</param>
	void saveContentToFile(Interface *mainWindow);
};

#endif