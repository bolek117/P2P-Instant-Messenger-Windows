#ifndef CUSTOMENUM_h
#define CUSTOMENUM_h

class CustomEnum
{
private:
	vector < string > names;
	vector < int > values;

public:
	CustomEnum();

	///<summary>Add next element to enum list.</summary>
	///<param name="name">Name of added element.</param>
	void AddEnum(string name);

	///<summary>Add element to enum list.</summary>
	///<param name="value">Name of added element.</param>
	///<parameter name="value">Desired value of new element.</parameter>
	void AddEnum(string name, int value);

	///<summary>Get value of element from enum list.</summary>
	///<parameter name="name">Name of desired element.</parameter>
	///<returns>Value of element.</returns>
	int GetEnum(string name);
};

#endif