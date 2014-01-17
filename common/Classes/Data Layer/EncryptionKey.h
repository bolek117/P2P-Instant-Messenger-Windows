#ifndef ENCRYPTIONKEY_h
#define ENCRYPTIONKEY_h

class EncryptionKey
{
private:
	bool type; // true: public, false: private
	string value;

public:
	EncryptionKey(string keyValue="", bool keyType=true) : type(keyType), value(keyValue) {}

	///<summary>Get type of key</summary>
	///<returns>True if public key, false if private key</returns>
	bool KeyType();

	///<summary>Get type of key</summary>
	///<param name="isPublic">New value of "type".</param>
	///<returns>Updated value of key type, True if public key, false if private key</returns>
	bool KeyType(bool isPublic);

	///<summary>Get value of key.</summary>
	///<returns>Value of key.</returns>
	string KeyValue();

	///<summary>Set value of key</summary>
	///<param name="keyValue">New value of "keyValue".</param>
	///<returns>Updated value of key.</returns>
	string KeyValue(string keyValue);

	///<summary>Same as KeyType()</summary>
	int operator= (string);

	///<summary>Same as KeyValue()</summary>
	int operator<< (string);

	int ClearKey();
};

#endif