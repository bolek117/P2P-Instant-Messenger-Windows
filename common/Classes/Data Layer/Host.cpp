#include "../../Stdafx.h"

void Host::HostId(UINT value)
{
	this->hostId = value;
}

UINT Host::HostId()
{
	return this->hostId;
}

bool Host::IsServer(int value)
{
	if (value != 0)
	{
		if (value == -1)
			this->isServer = false;
		else
			this->isServer = true;
	}

	return this->isServer;
}

bool Host::IsActive(bool check)
{
	if (check != false)
		this->isActive = this->Ping();

	return this->isActive;
}

void Host::SetIsActiveDirectly(bool value)
{
	this->isActive = value;
}

bool Host::IsListen()
{
	return this->isListen;
}

void Host::IsListen(bool value)
{
	this->isListen = value;
}

void Host::SockAddr(sockaddr *source)
{
	this->sockAddr = source;
}

sockaddr* Host::SockAddr()
{
	return this->sockAddr;
}

void Host::SockAddrIn(sockaddr_in *source)
{
	this->sockAddrIn = source;
}

sockaddr_in* Host::SockAddrIn()
{
	return this->sockAddrIn;
}

void Host::Socket(SOCKET source)
{
	this->winsocket = source;
}

SOCKET Host::Socket()
{
	return this->winsocket;
}

// TODO Ping
///<summary>Check if host is active.</summary>
///<returns>True if host is active, else false.</returns>
bool Host::Ping()
{

	return false;
}

SOCKET Host::Connect(Interface *mainWindow, Settings *settings, unsigned int socket_messages)
{
	stringstream buffer;

	this->Socket(NULL);
	if(settings->Winsock(true) == false)
	{
		MessageBox(mainWindow->HWnd(),"Winsock initialization failed","Critical Error",MB_ICONERROR);
		return NULL;
	}

	this->Socket(socket(AF_INET,SOCK_STREAM,IPPROTO_TCP));
	if(this->Socket()==INVALID_SOCKET)
	{
		buffer.str("");
		buffer << "Socket creation failed, WinApi error no " << WSAGetLastError();
		MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	if(WSAAsyncSelect(this->Socket(),mainWindow->HWnd(),socket_messages,(FD_CLOSE|FD_READ)))
	{
		buffer.str("");
		buffer << "WSAAsyncSelect failed, WinApi error no " << WSAGetLastError();
		MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	struct hostent *host;
	unsigned int ip;
	ip=inet_addr(this->Ip().c_str());
	host=gethostbyaddr((char*)&ip, this->Ip().length(), AF_INET);

	if(host==NULL)
	{
		buffer.str("");
		buffer << "GetHostByAddr failed, WinApi error no " << WSAGetLastError();
		MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	// Set up our socket address structure
	this->SockAddrIn()->sin_family=AF_INET;
	this->SockAddrIn()->sin_port=htons(this->Port());
	this->SockAddrIn()->sin_addr.s_addr=*((unsigned long*)host->h_addr);

	connect(this->Socket(),(LPSOCKADDR)this->SockAddrIn(),sizeof(sockaddr));

	this->isActive = true;
	return this->Socket();
}

unsigned int Host::CreateListenSocket(Interface *mainWindow, Settings *settings, unsigned int socket_messages)
{
	stringstream buffer;

	this->Socket(NULL);
	if(settings->Winsock(true) == false)
	{
		MessageBox(mainWindow->HWnd(),"Winsock initialization failed","Critical Error",MB_ICONERROR);
		return NULL;
	}

	this->Socket(socket(AF_INET,SOCK_STREAM,IPPROTO_TCP));
	if (this->Socket()==INVALID_SOCKET)
	{
		buffer.str("");
		buffer << "Unable to create socket, WinSock error no " << WSAGetLastError();
		MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port=htons(this->Port());
	SockAddr.sin_family=AF_INET;
	SockAddr.sin_addr.s_addr=htonl(INADDR_ANY);

	int result;
	for (unsigned int i=0;i<100;i++)
	{
		this->Port(this->Port()+1);
		SockAddr.sin_port=htons(this->Port());
		result = bind(this->Socket(),(LPSOCKADDR)&SockAddr,sizeof(SockAddr));
		if (result != SOCKET_ERROR)
			break;
	}

	if (result == SOCKET_ERROR)
	{
		buffer.str("");
		buffer << "Unable to bind socket, WinSock error no " << WSAGetLastError();
		MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}	

	WSAAsyncSelect(this->Socket(), mainWindow->HWnd(), socket_messages, (FD_CLOSE|FD_ACCEPT|FD_READ));

	if (listen(this->Socket(),SOMAXCONN)==SOCKET_ERROR)
	{
		buffer.str("");
		buffer << "Unable to listen, WinSock error no " << WSAGetLastError();
		MessageBox(mainWindow->HWnd(), buffer.str().c_str(), "Critical Error", MB_ICONERROR);
		return NULL;
	}

	this->IsListen(true);
	return this->Port();
}

void Host::Disconnect()
{
	this->isActive = false;
	closesocket(this->Socket());
}

void Host::Port(unsigned int port)
{
	this->port = port;
}

unsigned int Host::Port()
{
	return this->port;
}

void Host::sendMyData(Host *destination, bool withIp)
{
	stringstream buffer;
	Command command;

	if (withIp == false)
	{
		buffer.str("");
		buffer << MSGS::RESPONSE_USERDATA << "|" << this->Nickname() << "|" << this->KeyValue() << "|" << this->Port();
		command.Send(CMD::RESPONSE, buffer.str(), destination);
	}
	else
	{
		buffer.str("");
		buffer << MSGS::RESPONSE_CLIENTLIST << "|" << this->Nickname() << "|" << this->KeyValue() << "|" << this->Port() << "|" << this->Ip();
		command.Send(CMD::RESPONSE, buffer.str(), destination);
	}

	
		

	
}