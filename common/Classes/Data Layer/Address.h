#ifndef ADDRESS_h
#define ADDRESS_h

class Address
{
private:
	string nickname;
	string ip;

public:
	Address(string nickname="", string ip="0.0.0.0") : nickname(nickname), ip(ip) {}

	///<summary>Set nickname.</summary>
	///<param name="value">If empty (or == ""), "nickname" will be returned. Else,	new value of nickname will be set.</param>
	///<returns>Actual or updated value of nickname.</returns>
	string Nickname(string value="");

	///<summary>Set IP adress.</summary>
	///<param name="value">If empty (or == "0.0.0.0"), "ip" will be returned. Else, new value of IP will be set.</param>
	///<returns>Actual or updated value of IP.</returns>
	string Ip(string value="0.0.0.0");

	///<see cref="Nickname" />
	string operator<< (string value);

	///<see cref="Ip" />
	string operator= (string value);
};

#endif