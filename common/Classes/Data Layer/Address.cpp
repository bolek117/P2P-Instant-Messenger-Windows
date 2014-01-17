#include "../../Stdafx.h"

string Address::Nickname(string value)
{
	if (value != "")
		this->nickname = value;

	return this->nickname;
}

string Address::Ip(string value)
{
	if (value != "0.0.0.0")
		this->ip = value;

	return this->ip;
}

string Address::operator<< (string value)
{
	return Nickname(value);
}

string Address::operator= (string value)
{
	return Ip(value);
}