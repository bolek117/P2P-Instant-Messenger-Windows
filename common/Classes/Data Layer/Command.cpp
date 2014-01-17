#include "../../Stdafx.h"

UINT Command::Type()
{
	return this->type;
}

void Command::Type(UINT value)
{
	this->type = value;
}

string Command::Content()
{
	return this->content;
}

void Command::Content(string value)
{
	this->content = value;
}

void Command::Content(int value)
{
	stringstream result;
	result << value;
	this->content = result.str();
}

void Command::Clear()
{
	this->Type(0);
	this->isSerialized = false;
	this->isEncrypted = false;
	this->Content("");
}

bool Command::IsSerialized()
{
	return this->isSerialized;
}
void Command::IsSerialized(bool value)
{
	this->isSerialized = value;
}

bool Command::Serialize()
{
	if (this->IsSerialized() == false && this->IsEncrypted() == false)
	{
		stringstream result;
		result << this->Type() << "|" << this->Content();
		this->Content(result.str());

		this->isSerialized = true;
		return true;
	}
	else
		return false;
}

string Command::Unserialize()
{
	if (this->IsSerialized() == true)
	{
		if (this->IsEncrypted() == true)
			this->Decrypt();

		unsigned int pos = this->Content().find_first_of('|');
		unsigned int type=0;

		istringstream iType(this->Content().substr(0, pos+1));
		iType >> type;
		this->Type(type);

		this->Content(this->Content().substr(pos+1));

		this->isSerialized = false;
	}

	return this->Content();
}

vector < string > Command::UnserializeContent(char delim)
{
	string next;
	vector < string > result;

	for (unsigned int i=0;i<this->Content().length();i++) {
    	// If we've hit the terminal character
    	if (this->Content().operator[](i) == delim) {
    		// If we have some characters accumulated
    		if (next.length() > 0) {
    			// Add them to the result vector
    			result.push_back(next);
    			next = "";
    		}
    	} else {
    		// Accumulate the next character into the sequence
    		next += this->Content().operator[](i);
    	}
    }
	result.push_back(next);

	return result;
}


bool Command::IsEncrypted()
{
	return this->isEncrypted;
}
void Command::IsEncrypted(bool value)
{
	this->isEncrypted = value;
}

// TODO Encrypt
///<summary>Encrypt content of object.</summary>
///<returns>String with encrypted data.</returns>
string Command::Encrypt()
{
	this->isEncrypted = true;
	return this->Content();
}

// TODO Decrypt
///<summary>Decrypt content of object.</summary>
///<returns>String with decrypted data.</returns>
string Command::Decrypt()
{
	this->isEncrypted = false;
	return this->Content();
}

void Command::PrepareToSend()
{
	if (this->IsSerialized() == false)
		this->Serialize();

	if (this->IsEncrypted() == false)
		this->Encrypt();
}

void Command::PrepareToRead(string content)
{
	this->Content(content);
	this->IsEncrypted(true);
	this->IsSerialized(true);

	this->Encrypt();
	this->Unserialize();
}

bool Command::Send(Host *target)
{
	this->PrepareToSend();

	UINT count;
	count = send(target->Socket(), this->Content().c_str(), this->Content().length(), 0);
	if (count == this->Content().length())
		return true;
	else
		return false;
}

bool Command::Send(unsigned int type, int content, Host *target)
{
	this->isEncrypted = false;
	this->isSerialized = false;
	this->Type(type);
	this->Content(content);
	return Send(target);
}

bool Command::Send(unsigned int type, string content, Host *target)
{
	this->isEncrypted = false;
	this->isSerialized = false;
	this->Type(type);
	this->Content(content);
	return Send(target);
}

vector < bool > Command::Broadcast(vector < Host* > clientList)
{
	vector <bool> result;
	for (unsigned int i=0;i<clientList.size();i++)
		result.push_back(this->Send(clientList[i]));

	return result;
}

string Command::operator= (string value)
{
	this->Content(value);
	return this->Content();
}

string Command::operator+= (string value)
{
	this->Content(this->Content().append(value));

	return this->Content();
}