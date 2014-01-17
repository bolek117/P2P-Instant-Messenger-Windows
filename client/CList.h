#ifndef ELEMENTS_l
#define ELEMENTS_l

#include "resource.h"
#define ENTERTEXT "Enter your message here..."


namespace it
{
	Interface main;
	Interface addPerson;
}

namespace eb
{
	EditBox messages;
	EditBox toSend;
	EditBox addClient;
}

namespace lbl
{
	StaticLabel messages;
	StaticLabel addClient;
}

namespace btn
{
	Button send;
	Button addClient;

	enum buttons
	{
		EMPTY,
		SEND,
		ADDCLIENT,
		END
	};
}

namespace cb
{

}

Settings settings;
Host server;
Host me;

Command command;

vector < Host* > clientList;

SOCKET Socket = NULL;
SOCKET listenSocket = NULL;


UINT nResult;
char szHistory[10000];

unsigned int connectingTo = NULL;

#endif