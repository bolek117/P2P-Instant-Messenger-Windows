#ifndef POSITION_h
#define POSITION_h

class Position
{
private:
	int x;
	int y;
	UINT width;
	UINT height;

public:
	Position(int x=0, int y=0, UINT width=0, UINT height=0) : x(x), y(y), width(width), height(height) {}
	
	///<summary>Get or set X parameter</summary>
	///<param name="x">If parameter is empty (or == -1), value of X will be returned. Else value of X will be set to "x" value.</param>
	///<returns>Actual or updated value of x.</returns>
	int X(int x = -1);

	///<summary>Get or set Y parameter</summary>
	///<param name="y">If parameter is empty (or == -1), value of Y will be returned. Else value of Y will be set to "y" value.</param>
	///<returns>Actual or updated value of y.</returns>
	int Y(int y = -1);

	///<summary>Get or set width of object</summary>
	///<param name="width">If parameter is empty (or == 0), value of width will be returned. Else value of width will be set to "width" value.</param>
	///<returns>Actual or updated value of width.</returns>
	UINT Width(UINT width=0);

	///<summary>Get or set height of object</summary>
	///<param name="height">If parameter == 0 (or empty), value of height will be returned. Else value of height will be set to "height" value.</param>
	///<returns>Actual or updated value of height.</returns>
	UINT Height(UINT height=0);

	///<summary>Add (or subtract) some value to parameter.</summary>
	///<param name="parameter">x, y, width or height.</param>
	///<param name="value">Value of change.</param>
	void AddTo(string parameter, int value);

	///<summary>Add (or subtract) some value to actual postion.</summary>
	///<param name="x">Change of x value.</param>
	///<param name="y">Change of y value.</param>
	///<param name="width">Change of width value.</param>
	///<param name="height">Change of height value.</param>
	//void AddToPosition(int x=0, int y=0, int width=0, int height=0);

	///<summary>Set all values of actual postion.</summary>
	///<param name="x">Value of x value.</param>
	///<param name="y">Value of y value.</param>
	///<param name="width">Value of width value.</param>
	///<param name="height">Value of height value.</param>
	void SetPosition(int x=0, int y=0, int width=0, int height=0);

	///<summary>Clear content of Position type object.</summary>
	///<param name="mode">Mode of clearing. Gets values "size", "position". Any other string (er empty string) clear both variables.</param>
	///<returns>Method always return 1.</returns>
	int ClearPosition(string mode="both");
};

#endif