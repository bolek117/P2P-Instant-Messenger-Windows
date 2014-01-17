#include "../../Stdafx.h";

void CustomEnum::AddEnum(string name)
{
	unsigned int valuesSize = values.size();

	if (valuesSize != 0)
		AddEnum(name, values[valuesSize-1]);
	else
		AddEnum(name, 0);
}

void CustomEnum::AddEnum(string name, int value)
{
	values.push_back(value);
	names.push_back(name);
}

int CustomEnum::GetEnum(string name)
{
	for (unsigned int i=0;i<names.size() && i<values.size();i++)
	{
		if (names[i] == name)
			return values[i];
	}
	return NULL;
}