#include "../../Stdafx.h"

unsigned int Settings::Port(unsigned int value)
{
	if (value != 0)
		this->port = value;

	return this->port;
}

void Settings::MaxClients(unsigned int value)
{
	this->maxClients = value;
}

unsigned int Settings::MaxClients()
{
	return this->maxClients;
}

bool Settings::Winsock(bool check)
{
	WSADATA WsaDat;

	if (check == true)
	{
		if (WSAStartup(MAKEWORD(2,2),&WsaDat) == 0)
			this->winSockPresent = true;
		else
			this->winSockPresent = false;
	}

	return this->winSockPresent;
}

void Settings::SetSockAddr(UINT family, DWORD addr)
{
	this->sockAddr.sin_port = htons(this->port);
	this->sockAddr.sin_family = family;
	this->sockAddr.sin_addr.s_addr = htonl(addr);
}

SOCKADDR_IN* Settings::GetSockAddr()
{
	return &sockAddr;
}

bool Settings::ReadFromFile(string filename)
{
	ifstream file;
	file.open(filename);

	if(!file.is_open()) // Set default values and override settings file
	{
		this->Ip("127.0.0.1");
		this->Port(666);
		this->MaxClients(150);

		return false;
	}
	else
	{
		const unsigned int maxSize = 100;
		char buffer[maxSize];

		unsigned int i;
		for (i=1;file.getline(buffer, maxSize-1);i++)
		{
			vector<string> elements;
			elements = explode(buffer, '=');

			if (elements[0] == "ServerIpAddress")
			{
				this->Ip(elements[1]);
			}
			else if (elements[0] == "Port")
			{
				this->Port(stringToInt(elements[1]));
			}
			else if (elements[0] == "ClientNumberLimit")
			{
				this->MaxClients(stringToInt(elements[1]));
			}
			else if (elements[0] == "PublicKey")
			{
				this->KeyType(true);

				if (elements[1] != "")
					this->KeyValue(elements[1]);
				else
					this->KeyValue("abcdef");
			}
			else if (elements[0] == "PrivateKey")
			{
				this->KeyType(false);

				if (elements[1] != "")
					this->KeyValue(elements[1]);
				else
					this->KeyValue("abcdef");
			}
			else if (elements[0] == "NickName")
			{
				vector < string > nickname = explode(elements[1], '|');
				if (elements.size() > 1)
					this->Nickname(nickname[0]);
			}
			else	// settings file is malformed
				return false;
		}
	}

	return true;
}