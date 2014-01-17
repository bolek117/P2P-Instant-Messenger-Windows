#ifndef COMMAND_h
#define COMMAND_h

class Command : public EncryptionKey
{
private:
	UINT type;
	bool isSerialized;
	bool isEncrypted;
	string content;

public:
	Command() : type(0), isSerialized(false), isEncrypted(false), content("") {}

	///<summary>Get value of "type" parameter.</summary>
	///<returns>Actual value of "type" parameter</returns>
	UINT Type();

	///<summary>Set value of "type" parameter.</summary>
	///<param name="value">New value of "type" parameter.</param>
	void Type(UINT value);

	///<summary>Get value of "content" parameter.</summary>
	///<returns>Actual value of "content" parameter</returns>
	string Content();

	///<summary>Set value of "content" parameter.</summary>
	///<param name="value">New value of "content" parameter.</param>
	void Content(string value);
	void Content(int value);

	///<summary>Clear content of "content" parameter.</summary>
	void Clear();

	///<summary>Get value of "isSerialized" parameter.</summary>
	///<returns>Actual value of "isSerialized" parameter</returns>
	bool IsSerialized();
	void IsSerialized(bool value);

	///<summary>Serialize content of unencrypted object.</summary>
	///<returns>If success - true. False if error occured (for example - content if encrypted).</returns>
	bool Serialize();
	

	///<summary>Unserialize content of object.</summary>
	///<returns>String with unserialized data.</returns>
	string Unserialize();
	vector < string > UnserializeContent(char delim);

	///<summary>Get value of "isEncrypted" parameter.</summary>
	///<returns>Actual value of "isEncrypted" parameter</returns>
	bool IsEncrypted();
	void IsEncrypted(bool value);

	///<summary>Encrypt content of object.</summary>
	///<returns>String with encrypted data.</returns>
	string Encrypt();

	///<summary>Decrypt content of object.</summary>
	///<returns>String with decrypted data.</returns>
	string Decrypt();

	///<summary>Serialize and encrypt if necessary.</summary>
	void PrepareToSend();

	//<summary>Decrypt and unserialize command.</summary>
	void PrepareToRead(string content);

	///<summary>Encrypt and serialize (if necessary) and send data to proper user.</summary>
	///<param name="target">Pointer to Host object who should recive message.</param>
	///<returns>True if length of sended data is equal to length of source data.</returns>
	bool Send(Host *target);
	bool Send(unsigned int type, int content, Host *target);
	bool Send(unsigned int type, string content, Host *target);

	///<summary>Encrypt and serialize (if necessary) and send data to users from vector.</summary>
	///<param name="nicknamsList">Vector of nicknames of user who should recive message.</param>
	///<returns>True if length of sended data is equal to length of source data, as a vector corresponding to nickname list.</returns>
	vector < bool > Broadcast(vector < Host* > clientList);

	///<summary>Set value of "content" parameter.</summary>
	///<param name="value">New value of "content" parameter.</param>
	///<returns>New value of "content" parameter.</returns>
	string operator= (string value);

	///<summary>Append string to "content" parameter.</summary>
	///<param name="value">String to append.</param>
	///<returns>New value of "content" parameter.</returns>
	string operator+= (string value);
};

#endif