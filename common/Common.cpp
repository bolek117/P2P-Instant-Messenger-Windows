#include "Stdafx.h"

bool stopOnFailure(bool statement, string criticalMessageText)
{
	if(statement == true)
	{
		MessageBox(0,criticalMessageText.c_str(),"Critical Error",MB_ICONERROR);
		PostQuitMessage(0);
		return true;
	}
	else
		return false;
}

string intToString(int valueToConvert)
{
	stringstream result;
	result << valueToConvert;
	return result.str();
}

int stringToInt(string toConvert)
{
	int comm=0;
	istringstream iType(toConvert);
	iType >> comm;

	return comm;
}

vector<string> explode(string toExplode, char delim)
{
	string next;
	vector < string > result;

	for (unsigned int i=0;i<toExplode.length();i++) {
    	// If we've hit the terminal character
    	if (toExplode[i] == delim) {
    		// If we have some characters accumulated
    		if (next.length() > 0) {
    			// Add them to the result vector
    			result.push_back(next);
    			next = "";
    		}
    	} else {
    		// Accumulate the next character into the sequence
    		next += toExplode[i];
    	}
    }
	result.push_back(next);

	return result;
}

SOCKET createListenSocket(SOCKET s, Settings *settings, Interface *it, Host *me, unsigned int messagesInterface)
{
	stringstream buffer;

	if(settings->Winsock(true) == false)
	{
		MessageBox(it->HWnd(),"Winsock initialization failed","Critical Error",MB_ICONERROR);
		return NULL;
	}

	s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (s==INVALID_SOCKET)
	{
		buffer.str("");
		buffer << "Unable to create socket, WinSock error no " << WSAGetLastError();
		MessageBox(it->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	me->Port(settings->Port());
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port=htons(me->Port());
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_addr.s_addr=htonl(INADDR_ANY);

	int result;
	for (unsigned int i=0;i<100;i++)
	{
		me->Port(me->Port()+1);
		SockAddr.sin_port=htons(me->Port());
		result = bind(s,(LPSOCKADDR)&SockAddr,sizeof(SockAddr));
		if (result != SOCKET_ERROR)
			break;
	}

	if (result == SOCKET_ERROR)
	{
		buffer.str("");
		buffer << "Unable to bind socket, WinSock error no " << WSAGetLastError();
		MessageBox(it->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}	

	WSAAsyncSelect(s, it->HWnd(), messagesInterface, (FD_CLOSE|FD_ACCEPT|FD_READ));

	if (listen(s,SOMAXCONN)==SOCKET_ERROR)
	{
		buffer.str("");
		buffer << "Unable to listen, WinSock error no " << WSAGetLastError();
		MessageBox(it->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	return s;
}