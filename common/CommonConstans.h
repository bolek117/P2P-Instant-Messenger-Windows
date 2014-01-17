#ifndef COMMONCONSTANS_h
#define COMMONCONSTANS_h

#define SERVER_ADDR "127.0.0.1" //TODO server_addr to conf.xml
#define DEFAULT_PORT 666 //TODO default port to conf.xml
#define APP_NAME "SoFive"


namespace CMD
{
	enum cmd
	{
		EMPTY=0,
		CONNECT,
		CONNECTION_SUCCESS,
		MESSAGE,
		REQUEST,
		RESPONSE,
		JOIN,
		DISCONNECT,
		ENDLIST
	};
}

namespace MSGS
{
	enum msg
	{
		EMPTY=0,
		SUCCESS,
		UNRESOLVEDADDRESSFAMILY,	//connect
		TOOMANYCLIENTS,
		USERNAMENOTUNIQUE,
		DISCONNECTANDCLOSE,
		NICKNAMEDETAILS,	//connection Success
		CLIENTNOTFOUND,	//join
		CLIENTNOTACTIVE,
		PING,
		PONG,
		CANNOTADDYOURSELF,
		REQUEST_CLIENTLIST,
		RESPONSE_CLIENTLIST,
		REQUEST_USERDATA,
		REQUEST_USERDATA_QUESTION,
		RESPONSE_USERDATA,
		ENDLIST
	};
}

enum WindowMessages
{
	WM_SOCKET=101,
	WM_SERVERSOCKET,
	WM_LISTENSOCKET,
	WM_CLIENTSOCKET,
	WM_DISCONNECT,
	WM_CLEARCLIENTLIST
};

const int clientMaxClients=15;
const int nMaxClients=15;

#endif