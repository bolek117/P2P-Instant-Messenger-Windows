#ifndef STATICLABEL_h
#define STATICLABEL_h

class StaticLabel : public Interface
{
private:
	string text;

public:
	StaticLabel()
	{
		this->Style(WS_CHILD|WS_VISIBLE);
		this->Type(WC_STATIC);
	}

	///<summary>Get or set text in window.</summary>
	///<param name="text">If empty (or == ""), value of text will be returned. Else, "value" will be set as text in window.</param>
	///<returns>Actual or updated value of text.</returns>
	string Text(string text="");

	///<summary>Set values specific to StaticLabel and display to screen.</summary>
	///<param name="parentWindow">"Interface" object of parent window.</param>
	///<param name="text">Text inside of label.</param>
	///<param name="x">Position x in px.</param>
	///<param name="y">Position y in px.</param>
	///<param name="width">Width.</param>
	///<param name="height">Height.</param>
	void SetLabel(Interface *parentWindow, string text, INT x, INT y, UINT width, UINT height);
	void SetLabel(Interface *parentWindow, string text, Position *positionData);
	void SetLabel(Interface *parentWindow, string text);
};

#endif