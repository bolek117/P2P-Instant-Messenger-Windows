#ifndef HOST_h
#define HOST_h

#pragma warning( disable : 4290 )

class Host : public EncryptionKey, public Address
{
private:
	unsigned int hostId;
	bool isServer;
	bool isActive;
	bool isListen;
	unsigned int port;
	sockaddr *sockAddr;
	sockaddr_in *sockAddrIn;
	SOCKET winsocket;

public:
	Host(bool isActive=false, bool isServer=false) : isServer(isServer), isActive(isActive)
	{
		sockAddr = new sockaddr;
		sockAddrIn = new sockaddr_in;
	}

	~Host()
	{
		delete sockAddrIn;
		delete sockAddr;
	}

	///<summary>Set value of "id" parameter.</summary>
	///<param name="value">Value of "hostId" will be set to "value".</param>
	void HostId(UINT value);

	///<summary>Get value of "id" parameter.</summary>
	///<returns>Actual value of "hostId".</returns>
	UINT HostId();

	///<summary>Get or set value of "isServer" parameter.</summary>
	///<param name="value">If empty (or == 0), value of "isServer" will be returned. Else, value of "isServer" is set to "value".</param>
	///<returns>Actual or updated value of "isServer".</returns>
	bool IsServer(int value=0);

	///<summary>Get or set value of "isActive" parameter.</summary>
	///<param name="check">If empty (or == false), value of "isActive" will be returned. Else, host will be pinged and value of "isActive" is set to proper value.</param>
	///<returns>Actual or updated value of "isActive".</returns>
	bool IsActive(bool check=false);

	///<summary>Set value of "isActive" parameter.</summary>
	///<param name="value">New value of "isActive" parameter.</param>
	void SetIsActiveDirectly(bool value);

	///<summary>Get value of "isListen" parameter.</summary>
	///<returns>Value of "isListen"</returns>
	bool IsListen();

	///<summary>Set value of "isListen" parameter.</summary>
	///<param name="source">Value of "isListen" will be set to "value".</param>
	void IsListen(bool value);

	///<summary>Set value of "sockAddr" parameter.</summary>
	///<param name="source">Value of "sockAddr" will be set to "source".</param>
	void SockAddr(sockaddr *source);

	///<summary>Get value of "sockAddr" parameter.</summary>
	///<returns>Actual value of "sockAddr".</returns>
	sockaddr* SockAddr();

	///<summary>Set value of "sockAddrIn" parameter.</summary>
	///<param name="source">Value of "sockAddrIn" will be set to "source".</param>
	void SockAddrIn(sockaddr_in *source);

	///<summary>Get value of "sockAddrIn" parameter.</summary>
	///<returns>Actual value of "sockAddrIn".</returns>
	sockaddr_in* SockAddrIn();

	///<summary>Set value of "socket" parameter.</summary>
	///<param name="source">Value of "socket" will be set to "source".</param>
	void Socket(SOCKET source);

	///<summary>Get value of "socket" parameter.</summary>
	///<returns>Actual value of "socket".</returns>
	SOCKET Socket();

	///<summary>Check if host is active.</summary>
	///<returns>True if host is active, else false.</returns>
	bool Ping();

	///<summary>Connect to host.</summary>
	///<returns>True if connection success, else false.</returns>
	SOCKET Connect(Interface *mainWindow, Settings *settings, unsigned int socket_messages=WM_SOCKET) throw(string);
	unsigned int CreateListenSocket(Interface *mainWindow, Settings *settings, unsigned int socket_messages=WM_SOCKET);

	///<summary>Disconnect host.</summary>
	void Disconnect();

	///<summary>Set port</summary>
	///<param name="port">Port number</param>
	void Port(unsigned int port);

	///<summary>Get port</summary>
	///<returns>Port number</returns>
	unsigned int Port();

	///<summary>Send personal data to host</summary>
	///<param name="destination">Pointer to Host object.</param>
	///<param name="withIp">Determine if IP address should be sent</param>
	void sendMyData(Host *destination, bool withIp=false);
};

#endif