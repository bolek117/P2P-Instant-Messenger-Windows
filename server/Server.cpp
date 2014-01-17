#include "../../Common/Stdafx.h"
#include "SList.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
static UINT id=0;


HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;

	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.cbWndExtra=0;
	wc.cbClsExtra=0;
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = HBRUSH(COLOR_BTNFACE + 1);
	wc.lpszMenuName=NULL;
	string appName = APP_NAME;
	appName += " Server";
	wc.lpszClassName=appName.c_str();

	if(!RegisterClass(&wc))
		return 0;

	it::main.Name(appName);
	it::main.X(10);
	it::main.Y((GetSystemMetrics(SM_CYSCREEN)/2)-240);
	it::main.Width(640);
	it::main.Height(480);

	hwnd=CreateWindow(it::main.Name(),it::main.Name(),WS_POPUPWINDOW|WS_CAPTION|WS_MINIMIZEBOX,it::main.X(),it::main.Y(),it::main.Width(),it::main.Height(),NULL,NULL,hInstance,NULL);

	it::main.HWnd(hwnd);
	it::main.HInstance(hInstance);
	it::main.Name(appName);

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	//while(PeekMessage(&msg,it::main.HWnd(),0,0,PM_NOREMOVE))
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	stringstream text;
	switch(msg)
	{
	case WM_SHOWWINDOW:
		{
#pragma region Data layer
			if(Settings.Winsock(true) == false)
			{
				MessageBox(0,"Winsock initialization failed","Critical Error",MB_ICONERROR);
				SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);
			}

			if (Settings.ReadFromFile("Server.config") == false)
			{
				MessageBox(0,"Server.config file does not exist, is incomplete or content of file is malformed.\r\nDefault values will be used and file will be filled with default data.","Information",MB_ICONINFORMATION);

				ofstream ofile;
				ofile.open("Server.config", ios_base::trunc);

				if (ofile.is_open())
					ofile << "ServerIpAddress=127.0.0.1\n"
						<< "Port=666\n"
						<< "ClientNumberLimit=150\n"
						<< "PublicKey=\n"
						<< "PrivateKey=\n";
				else
					MessageBox(0,"Error occured in time of Server.config creation.\r\nCheck if folder is not READ ONLY or if you have some free space on your hard drive.\r\nDefault settings are used.", "Error",MB_ICONERROR);
			}

			me.KeyType(true);
			me.KeyValue(Settings.KeyValue());

			ServerSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			if (ServerSocket==INVALID_SOCKET)
				SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);

			SOCKADDR_IN SockAddr;
			SockAddr.sin_port=htons(Settings.Port());
			SockAddr.sin_family=AF_INET;
			SockAddr.sin_addr.s_addr=htonl(INADDR_ANY);

			if (bind(ServerSocket,(LPSOCKADDR)&SockAddr,sizeof(SockAddr))==SOCKET_ERROR)
			{
				MessageBox(0,"Unable to bind socket. Is other instance of server running on this host?","Critical Error",MB_ICONERROR);
				SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);
			} 

			WSAAsyncSelect(ServerSocket, it::main.HWnd(), WM_SOCKET,(FD_CLOSE|FD_ACCEPT|FD_READ));

			if (listen(ServerSocket,SOMAXCONN)==SOCKET_ERROR)
			{
				MessageBox(0,"Unable to listen.","Critical Error",MB_ICONERROR);
				SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);
			}
#pragma endregion

#pragma region Presentation layer
			lbl::sendToUser.SetPosition(10, 5, 150, 12);
			lbl::sendToUser.SetLabel(&it::main, "Connected users:");

			cb::sendToUser.SetPosition(lbl::sendToUser.X(), lbl::sendToUser.Y()+lbl::sendToUser.Height()+2, lbl::sendToUser.Width(), it::main.Height()-40);
			cb::sendToUser.Style(CBS_AUTOHSCROLL|WS_VSCROLL);
			cb::sendToUser.AddOption("(None)");
			cb::sendToUser.SetBox(&it::main);

			lbl::incoming.SetPosition(lbl::sendToUser.X()+lbl::sendToUser.Width()+5, lbl::sendToUser.Y(), it::main.Width()-20, 14);
			lbl::incoming.SetLabel(&it::main, "Incoming messages:");

			eb::incoming.SetPosition(lbl::incoming.X(), lbl::incoming.Height()+5, lbl::incoming.Width()-lbl::sendToUser.Width()-10, it::main.Height()-lbl::incoming.Height()-40);
			eb::incoming.SetBoxStyle(true, ES_AUTOVSCROLL|WS_VSCROLL);
			eb::incoming.SetBox(&it::main, "", true, true);
			eb::incoming.RequireTimestamp(true);

			stringstream buffer;

			buffer.str("");
			buffer << "[INFO] Server IP: " << Settings.Ip() << ", Server listening port: " << Settings.Port() << ", Clients limit: " << Settings.MaxClients();
			eb::incoming += buffer.str();
			eb::incoming += "[INFO] To change this values, change content of Server.config file";
			eb::incoming += "[INFO] Waiting for clients...";
#pragma endregion
		}
		break;

	case WM_SOCKET:
		{
			stringstream buffer;
			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
				{
					UINT id = clientList.size();
					
					int size=sizeof(sockaddr);
					Host *client = new Host;
					client->Socket(accept(wParam,client->SockAddr(),&size));

					if(clientList.size()<Settings.MaxClients())
					{
						clientList.push_back(client);

						if (client->Socket()==INVALID_SOCKET)
						{
							buffer.str("");
							buffer << "!!! Connection of client " << id << " failed because of error no "<< WSAGetLastError();
							eb::incoming += buffer.str();

							buffer.str("");

							shutdown(client->Socket(), SD_BOTH);
							closesocket(client->Socket());
							clientList.pop_back();
						}
						else
						{
							if (clientList.size() == 1)
								client->HostId(1);
							else
								client->HostId(clientList[clientList.size()-2]->HostId()+1);
							

							if (client->SockAddr()->sa_family == AF_INET)
							{

								buffer.str("");
								buffer << (UINT)client->SockAddr()->sa_data[2] << "." << (UINT)client->SockAddr()->sa_data[3] << "." << (UINT)client->SockAddr()->sa_data[4] << "." << (UINT)client->SockAddr()->sa_data[5];
								client->Ip(buffer.str());

								command.Send(CMD::CONNECT, MSGS::REQUEST_USERDATA, client);
							}
							else
							{
								buffer.str("");
								buffer << "!!! Client with unresolved address family connected";
								command.Send(CMD::CONNECT, MSGS::UNRESOLVEDADDRESSFAMILY, client);
								clientList.pop_back();
								client->Disconnect();
								eb::incoming += buffer.str();

							}
						}
					}
					else
					{
						eb::incoming += "Connection attempt rejected because of maximum number of clients";
						command.Send(CMD::CONNECT, MSGS::TOOMANYCLIENTS, client);
						client->Disconnect();
						delete client;
					}
				}
				break;

			case FD_READ:
				{
					const UINT chars = 1024;
					char szIncoming[chars];
					Host* client;
					for(unsigned int i=0;i<clientList.size();i++)
					{
						client = clientList[i];
						ZeroMemory(szIncoming,sizeof(szIncoming)); 
						recv(client->Socket(), szIncoming, chars-1, 0);

						string messages = string(szIncoming);
						if (messages.length() != 0)
						{
							command.PrepareToRead(messages);
							switch (command.Type())
							{
							case CMD::RESPONSE:
								{
									vector < string > exploded = command.UnserializeContent('|');
									switch(stringToInt(exploded[0]))
									{
									case MSGS::RESPONSE_USERDATA:
										{
											vector < string > exploded = command.UnserializeContent('|');
											client->Nickname(exploded[1]);
											client->KeyValue(exploded[2]);
											client->Port(stringToInt(exploded[3]));

											bool unique=true;
											for (unsigned int j=0;j<clientList.size();j++)
											{
												if (client->HostId() != clientList[j]->HostId() && client->Nickname() == clientList[j]->Nickname())
													unique = false;
											}

											// if username is unique
											if (unique == true)
											{
												buffer.str("");
												buffer << "[" << client->HostId() << "] " << client->Nickname();
												cb::sendToUser.AddOption(buffer.str());
												client->SetIsActiveDirectly(true);

												
												buffer.str("");
												buffer << me.KeyValue() << "|" << client->Ip();
												command.Send(CMD::CONNECTION_SUCCESS, buffer.str().c_str(), client);

												buffer.str("");
												buffer << "[" << client->HostId() << "] Client connected: nickname[" << client->Nickname() << "], publicKey [" << client->KeyValue() << "], IP[" << client->Ip() << "]";
												eb::incoming += buffer.str();
											}
											else
											{
												command.Send(CMD::CONNECT, MSGS::USERNAMENOTUNIQUE, client);
												client->Disconnect();
												clientList.erase(clientList.begin()+i);

												eb::incoming += "!!! Connection rejected because of not unique nickname";
											}
										}
										break;
									}
								}
								break;

							case MSGS::RESPONSE_USERDATA:
								{
									
								}
								break;

							case CMD::JOIN:
								{
									vector < string > exploded = command.UnserializeContent('|');
									int comm = stringToInt(exploded[0]);
									switch(comm)
									{
									case MSGS::NICKNAMEDETAILS:	// search for nickname
										{
											buffer.str("");
											buffer << "[" << clientList[i]->HostId() << "] Pooling for username [" << exploded[1] << "]";
											eb::incoming += buffer.str();

											client = NULL;
											// search for client in database
											for (unsigned int j=0;j<clientList.size();j++)
											{
												if (clientList[j]->Nickname() == exploded[1])
												{
													client = clientList[j];
													break;
												}
											}

											// if client found
											if (client != NULL)
											{
												if (client->HostId() == clientList[i]->HostId())
												{
													buffer.str("");
													buffer << MSGS::CANNOTADDYOURSELF;
													command.Send(CMD::JOIN, buffer.str(), clientList[i]);

													buffer.str("");
													buffer << "[" << clientList[i]->HostId() << "] Self adding. Terminating.";
													eb::incoming += buffer.str();
												}
												else
												{
													if (client->IsActive() == true)
													{
													
														buffer.str("");
														buffer << "[" << clientList[i]->HostId() << "] Client found in database. Pinging.";
														eb::incoming += buffer.str();

														buffer.str("");
														buffer << MSGS::PING << "|" << clientList[i]->HostId();
														command.Send(CMD::JOIN, buffer.str(), client);
														client->SetIsActiveDirectly(false);
													}
																			else
												{
													command.Send(CMD::JOIN, MSGS::CLIENTNOTACTIVE, client);
													buffer.str("");
													buffer << "[" << clientList[i]->HostId() << "] Response: client found but not connected";
													eb::incoming += buffer.str();
												}
												}
											}
											else
											{
												buffer.str("");
												buffer << "[" << clientList[i]->HostId() << "] Response: client not found";
												eb::incoming += buffer.str();
												command.Send(CMD::JOIN, MSGS::CLIENTNOTFOUND, clientList[i]);
											}
										}
										break;

									case MSGS::PONG:
										{
											client = NULL;

											for (unsigned j=0;j<clientList.size();j++)
											{
												if (clientList[j]->HostId() == stringToInt(exploded[1]))
												{
													client = clientList[j];
													break;
												}
												
											}

											if (client != NULL)
											{
												clientList[i]->SetIsActiveDirectly(true);
												buffer.str("");
												buffer << MSGS::RESPONSE_USERDATA << "|" << clientList[i]->Ip() << "|" << clientList[i]->KeyValue() << "|" << clientList[i]->Port();
												command.Send(CMD::JOIN, buffer.str(), client);

												buffer.str("");
												buffer << "[" << clientList[i]->HostId() << "] Ping status: success.";
												eb::incoming += buffer.str();
											}
											else
											{
												buffer.str("");
												buffer << "[" << clientList[i]->HostId() << "] Ping status: client disconnected.";
												eb::incoming += buffer.str();
											}
										}
										break;
										
									}
								}
								break;

							default:
								{
									//eb::incoming += string(szIncoming);
								}
								break;
							}
						}
					}
				}
				break;

			case FD_CLOSE:
				{
					for(unsigned int i=0;i<clientList.size();i++)
					{
						if ((DWORD)clientList[i]->Socket() == (DWORD)wParam)
						{
							clientList[i]->Disconnect();

							buffer.str("");
							buffer << "[" << clientList[i]->HostId() << "] ";
							cb::sendToUser.DeleteOption(SendMessage(cb::sendToUser.HWnd(), CB_FINDSTRING, 0, (LPARAM)buffer.str().c_str()));

							buffer.str("");
							buffer << "[" << clientList[i]->HostId() << "] Client [" << clientList[i]->Nickname() << "] disconnected.";

							clientList.erase(clientList.begin()+i);
														
							eb::incoming += buffer.str();
							break;
						}
					}
				}
				break;
			}
		}
		break;

	case WM_COMMAND:
		{
			switch(wParam)
			{
			case btn::SEND:
				{
					eb::incoming += eb::toSend.Text();
					eb::toSend.Text("");
				}
				break;
			case btn::DISCONNECT:
				SendMessage(it::main.HWnd(), WM_DISCONNECT, 0, 0);
				break;
			}
		}
		break;

	case WM_CLEARCLIENTLIST:
		{
			if (clientList.size() > 0)
			{
				for (unsigned int i=0;i<clientList.size();i++)
					delete clientList[i];

				clientList.clear();
			}
			
		}
		break;

	case WM_DISCONNECT:
		{
			if (clientList.size() > 0)
			{
				stringstream buffer;
				for (unsigned int i=0;i<clientList.size();i++)
				{	
					if (clientList[i]->IsActive() == true)
					{
						buffer.str("");
						buffer << "Client [" << i << "] disconnected.";
						eb::incoming += buffer.str();
						
						command.Send(CMD::DISCONNECT, CMD::DISCONNECT, clientList[i]);
						clientList[i]->Disconnect();
					}
				}
			}
		}
		break;

	case WM_DESTROY:
		{
			SendMessage(it::main.HWnd(), WM_DISCONNECT, 0, 0);
 			SendMessage(it::main.HWnd(), WM_CLEARCLIENTLIST, 0, 0);
			shutdown(ServerSocket,SD_BOTH);
			closesocket(ServerSocket);
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}