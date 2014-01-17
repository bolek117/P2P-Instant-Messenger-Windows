#ifndef ELEMENTS_l
#define ELEMENTS_l

namespace it
{
	Interface	main;
}

namespace eb
{
	EditBox		incoming;
	EditBox		toSend;
}

namespace lbl
{
	StaticLabel incoming;
	StaticLabel commandType;
	StaticLabel sendToUser;
}

namespace btn
{
	Button		send;
	Button		disconnect;

	enum buttons
	{
		EMPTY,
		SEND,
		DISCONNECT,
		END
	};
}

namespace cb
{
	ComboBox commandType;
	ComboBox sendToUser;
}

Settings Settings;
Command command;

SOCKET ServerSocket=NULL;
vector < Host* > clientList;
Host me;

#endif