#ifndef COMBOBOX_h
#define COMBOBOX_h

class ComboBox : public Interface
{
private:
	vector< string > options;
	bool isDropdown;

public:
	ComboBox() : isDropdown(true)
	{
		this->Style(WS_CHILD|WS_VISIBLE);
		this->Type(WC_COMBOBOX);
		this->Name("x");
	}

	///<summary>Add option to ComboBox.</summary>
	///<param name="name">Text to display.</param>
	///<returns>1 if success, 0 if any error occured.</returns>
	int AddOption(string name);
	
	///<summary>Highlight option on list.</summary>
	///<param name="position">ID of desired option (form 0).</param>
	///<returns>ID of highlighted option or -1 if wrong "position" given.</returns>
	int SelectOption(UINT position);
	
	///<summary>Get actual selected option.</summary>
	///<returns>ID of actualy selected option</returns>
	int GetActualOption();
	
	///<summary>Add option to ComboBox</summary>
	///<param name="name">Text to display.</param>
	///<returns>1 if success, 0 if any error occured.</returns>
	int operator+= (string name);
	
	///<summary>Highlight option on list.</summary>
	///<param name="position">ID of desired option (form 0).</param>
	///<returns>ID of highlighted option.</returns>
	int operator= (UINT option);
	
	///<summary>Add option to ComboBox</summary>
	///<param name="name">Text to display.</param>
	///<returns>1 if success, 0 if any error occured.</returns>
	int operator<< (string name);
	
	///<summary>Select next option on list.</summary>
	///<returns>ID of selected option.</returns>
	int operator++ (int);
	
	///<summary>Select prev option on list.</summary>
	///<returns>ID of selected option.</returns>
	int operator-- (int);

	///<summary>Set all values for element.</summary>
	///<param name="parentWindow">Pointer to parent.</param>
	///<param name="isDropdown">If combobox should be in form of dropdown list</param>
	///<param name="x">X position of element.</param>
	///<param name="y">Y position of element.</param>
	///<param name="width">Width of element.</param>
	///<param name="height">Height of element.</param>
	void SetBox(Interface *parentWindow, bool isDropdown, INT x, INT y, UINT width, UINT height);
	void SetBox(Interface *parentWindow, bool isDropdown, Position *positionData);
	void SetBox(Interface *parentWindow, bool isDropdown);
	void SetBox(Interface *parentWindow);

	///<summary>Get or set dropdown mode</summary>
	///<param name="mode">If 0, value of actual mode will be return. If -1, value will be set to false, if 1 - to true</param>
	///<returns>Actual or updated value of mode</returns>
	bool Dropdown(int mode=0);

	///<summary>Delete option form list.</summary>
	///<param name="id">ID of element to delete.</param>
	///<returns>Count of the strings remaining in the list or -1 if id is greater than range.</returns>
	int DeleteOption(UINT id);
};

#endif