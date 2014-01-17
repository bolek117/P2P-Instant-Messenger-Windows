#ifndef BUTTON_h
#define BUTTON_h

class Button : public Interface
{
private:
	HMENU buttonId;

public:
	//Button(HWND hWnd, HINSTANCE hInstance, string text="", UINT buttonId=999) : buttonId((HMENU)buttonId), text(text)
	Button()
	{
		this->Type(WC_BUTTON);
		this->Style(WS_CHILD|WS_VISIBLE);
	}

	///<summary>Set all values for button.</summary>
	///<param name="parentWindow">Pointer to parent.</param>
	///<param name="text">Value of text on button.</param>
	///<param name="buttonId">ID of button.</param>
	///<param name="x">X position of button.</param>
	///<param name="y">Y position of button.</param>
	///<param name="width">Width of button.</param>
	///<param name="height">Height of button.</param>
	void SetButton(Interface *parentWindow, string text, UINT buttonId, INT x, INT y, UINT width, UINT height);
	void SetButton(Interface *parentWindow, string text, UINT buttonId, Position *positionData);
	void SetButton(Interface *parentWindow, string text, UINT buttonId);

	///<summary>Get or set value of text on button.</summary>
	///<param name="value">If parameter is empty (or == ""), value of "text" will be returned. Else value of "text" will be set to "value" value.</param>
	///<returns>Actual or updated value of "text".</returns>
	string Text(string value="");

	///<summary>Get or set value of buttonId.</summary>
	///<param name="value">If parameter is empty (or == -1), value of "buttonId" will be returned. Else value of "buttonId" will be set to "id" value.</param>
	///<returns>Actual or updated value of "buttonId".</returns>
	HMENU ButtonId(UINT id=999);

	///<summary>Get or set value of buttonId.</summary>
	///<param name="value">If parameter is empty (or == 999), value of "buttonId" will be returned. Else value of "buttonId" will be set to "id" value.</param>
	///<returns>Actual or updated value of "buttonId".</returns>
	HMENU operator= (UINT id);

	///<summary>Get or set value of text on button.</summary>
	///<param name="value">If parameter is empty (or == ""), value of "text" will be returned. Else value of "text" will be set to "value" value.</param>
	///<returns>Actual or updated value of "text".</returns>
	string operator<< (string text);
};

#endif