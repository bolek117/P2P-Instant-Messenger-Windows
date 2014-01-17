#ifndef COMMON_h
#define COMMON_h

bool stopOnFailure(bool failureCondition, string criticalMessageText);
string intToString(int valueToConvert);
int stringToInt(string toConvert);
vector<string> explode(string toExplode, char delim);

SOCKET createListenSocket(SOCKET s, Settings *settings, Interface *it, Host *me, unsigned int messagesInterface);

#endif