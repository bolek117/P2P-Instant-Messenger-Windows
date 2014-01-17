#include "../../Stdafx.h"

bool EncryptionKey::KeyType()
{
	return this->type;
}

bool EncryptionKey::KeyType(bool keyType )
{
	this->type = keyType;
	return this->type;
}

string EncryptionKey::KeyValue()
{
	return this->value;
}

string EncryptionKey::KeyValue(string keyValue)
{
	this->value = keyValue;
	return this->value;
}

int EncryptionKey::operator= (string keyType)
{
	if (keyType == "private")
		this->type = false;
	else if (keyType == "public")
		this->type = true;
	else
		return 0;

	return 1;
}

int EncryptionKey::operator<< (string keyValue)
{
	this->value = keyValue;
	return 1;
}

int EncryptionKey::ClearKey()
{
	this->value = "";
	return 1;
}