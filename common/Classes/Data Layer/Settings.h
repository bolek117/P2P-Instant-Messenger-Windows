#ifndef SETTINGS_h
#define SETTINGS_h

class Settings : public EncryptionKey, public Address
{
private:
	bool winSockPresent;
	unsigned int port;
	unsigned int maxClients;
	SOCKADDR_IN sockAddr;

public:
	Settings() : port(666), winSockPresent(false), maxClients(15) {}

	///<summary>Get or set connection port for application.</summary>
	///<param name="value">If empty (or == 0), actual value of port will be returned. Else, value will be set to parameter value.</param>
	///<returns>Actual or updated value of port.</returns>
	unsigned int Port(unsigned int value=0);
	
	///<summary>Geet or set maximal number of clients</summary>
	///<param name="value">Number of maximum clients, or empty if actual value should be returned</param>
	///<returns>If value is empty - actual value of maximum clients limit.</returns>
	void MaxClients(unsigned int value);
	unsigned int MaxClients();

	///<summary>Get or set state of winsock.</summary>
	///<param name="check">If empty (or == false), actual state will be returned. Else, presence of WinSock will be check and "winSockPresent" will be set.</param>
	///<returns>Actual or updated value of WinSock presence.</returns>
	bool Winsock(bool check=false);
	
	///<summary>Set SOCKADDR_IN structure.</summary>
	///<param name="family">sin_family structure field.</param>
	///<param name="addr">sin_addr.s_addr structure field.</param>
	void SetSockAddr(UINT family, DWORD addr);

	///<summary>Get SOCKADDR_IN pointer.</summary>
	///<returns>Pointer to struct.</returns>
	SOCKADDR_IN* GetSockAddr();

	///<summary>Read settings from file and set proper fields</summary>
	///<param name="filename">Name of settings file (in program directory)</param>
	///<returns>True if everything is ok, else false</returns>
	bool ReadFromFile(string filename);
};

#endif