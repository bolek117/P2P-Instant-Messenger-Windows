#include "../../Common/Stdafx.h"
#include "CList.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wc;

#pragma region Main window declaration
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.cbWndExtra=0;
	wc.cbClsExtra=0;
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndProc;
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = HBRUSH(COLOR_BTNFACE + 1);
	wc.lpszMenuName=(LPCSTR)IDR_MENU1;
	stringstream appName;
	appName << APP_NAME << " v0.1 [User+" << (int)time(NULL) << "]";
	string tmp = appName.str();
	wc.lpszClassName = tmp.c_str();

	if(!RegisterClass(&wc))
		return 0;

	it::main.Name(appName.str());
	it::main.X(GetSystemMetrics(SM_CXSCREEN)/2+10);
	it::main.Y((GetSystemMetrics(SM_CYSCREEN)/2)-240);
	it::main.Width(640);
	it::main.Height(480);

	hwnd=CreateWindow(it::main.Name(),it::main.Name(),WS_POPUPWINDOW|WS_CAPTION|WS_MINIMIZEBOX,it::main.X(),it::main.Y(),it::main.Width(),it::main.Height(),NULL,LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)),hInstance,NULL);

	it::main.HWnd(hwnd);
	it::main.HInstance(hInstance);

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);
#pragma endregion 

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	stringstream buffer;
	switch(msg)
	{
	case WM_SHOWWINDOW:
		{
			if (settings.ReadFromFile("Settings.config") == false)
			{
				MessageBox(0,"Settings.config file does not exist, is incomplete or content of file is malformed.\r\nDefault values will be used and file will be filled with default data.","Information",MB_ICONINFORMATION);

				ofstream ofile;
				ofile.open("Settings.config", ios_base::trunc);

				if (ofile.is_open())
					ofile << "ServerIpAddress=127.0.0.1\n"
						<< "Port=666\n"
						<< "ClientNumberLimit=150\n"
						<< "PublicKey=\n"
						<< "PrivateKey=\n"
						<< "NickName=\n";
				else
					MessageBox(0,"Error occured in time of Settings.config creation.\r\nCheck if folder is not READ ONLY or if you have some free space on your hard drive.\r\nDefault settings are used.", "Error",MB_ICONERROR);
			}

			me.Nickname(settings.Nickname());
			if (me.Nickname() == "")
			{
				string nickname="User+";
				nickname.append(intToString((int)time(NULL)));
				me.Nickname(nickname);
			}
			
			me.KeyType(true);
			me.KeyValue(settings.KeyValue());

			lbl::addClient.SetPosition(10, 0, 135, 20);
			lbl::addClient.SetLabel(&it::main, "Add person to conversation:");

			btn::addClient.Width(50);
			eb::addClient.SetPosition(lbl::addClient.X()+lbl::addClient.Width(), lbl::addClient.Y(), it::main.Width()-lbl::addClient.Width()-btn::addClient.Width()-30, lbl::addClient.Height());
			eb::addClient.SetBox(&it::main, me.Nickname(), false, false);

			btn::addClient.SetPosition(eb::addClient.X()+eb::addClient.Width()+5, eb::addClient.Y(), 50, lbl::addClient.Height());
			btn::addClient.SetButton(&it::main, "Add", btn::ADDCLIENT);

			eb::messages.SetPosition(lbl::addClient.X(), lbl::addClient.Y()+lbl::addClient.Height(), 620, 300);
			eb::messages.SetBoxStyle(true, ES_AUTOVSCROLL|WS_VSCROLL);
			eb::messages.SetBox(&it::main, "", true, true);
			eb::messages.RequireTimestamp(true);
			eb::messages += "Attempting to connect to server...";

			eb::toSend.SetPosition(10, eb::messages.Y()+eb::messages.Height(), 520, 100);
			eb::toSend.Style(ES_AUTOVSCROLL|WS_VSCROLL);
			eb::toSend.SetBox(&it::main, "You must add someone to conversation before texting.", true, true);

			btn::send.SetPosition(eb::toSend.X()+eb::toSend.Width()+5, eb::toSend.Y(), 90, eb::toSend.Height());
			btn::send.SetButton(&it::main, "Send", btn::SEND);
			EnableWindow(btn::send.HWnd(),FALSE);

			//data layer
			server.Ip(settings.Ip());
			server.Port(settings.Port());
			server.Connect(&it::main, &settings, WM_SERVERSOCKET);
			server.SetIsActiveDirectly(true);

			me.Port(settings.Port()-1);
			me.CreateListenSocket(&it::main, &settings, WM_CLIENTSOCKET);
			if (me.Socket() == NULL)
				eb::messages += "Listen socket creation failed.";
			else
			{
				buffer.str("");
				if (me.Port() != settings.Port())
					buffer << "Your prefered port is busy. First free port was selected. ";
				buffer << "Listening on port " << me.Port();
				eb::messages += buffer.str();
			}
		}
		break;

	case WM_SERVERSOCKET:
		{
			if(WSAGETSELECTERROR(lParam))
			{
				MessageBox(0,"Server is down.","Error",	MB_ICONERROR);
				SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);
			}

			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_READ:
				{
					string incoming;
					const UINT chars = 1024;
					char szIncoming[chars];
					ZeroMemory(szIncoming,sizeof(szIncoming)); 

					recv(server.Socket(), szIncoming, chars-1, 0);

					incoming = szIncoming;
					command.PrepareToRead(incoming);

					if (command.Type() != CMD::MESSAGE)
					{
						unsigned int comm=0;
						comm = stringToInt(command.Content());

						switch (command.Type())
						{
						case CMD::CONNECT:
							{
								switch(comm)
								{
								case MSGS::REQUEST_USERDATA:
									me.sendMyData(&server); //cmd::response -> MSGS::RESPONSE_USERDATA
									break;
								case MSGS::UNRESOLVEDADDRESSFAMILY:
									eb::messages += "Unresolved address family. Disconnected";
									break;
								case MSGS::TOOMANYCLIENTS:
									eb::messages += "To many connections to server. Disconnected";
									break;
								case MSGS::USERNAMENOTUNIQUE:
									eb::messages += "!!! Your username is not unique. Change it and connect again.";
									break;
								case MSGS::DISCONNECTANDCLOSE:
									eb::messages += "Remote application close request";
									SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);
									break;
								case MSGS::PING:
									command.Send(CMD::CONNECT, MSGS::PONG, &server);
									break;

								}
							}
							break;

						case CMD::CONNECTION_SUCCESS:
							{
								server.KeyType(true);
								vector <string> exploded = command.UnserializeContent('|');
								server.KeyValue(exploded[0]);
								me.Ip(exploded[1]);

								buffer.str("");
								buffer << "Connection success. Server public key [" << server.KeyValue() << "]";
								eb::messages += buffer.str();
							}
							break;

						case CMD::JOIN:
							{
								vector < string > exploded = command.UnserializeContent('|');

								int comm = stringToInt(exploded[0]);
								switch (comm)
								{
								case MSGS::RESPONSE_USERDATA:
									{
										Host *client;

										client = clientList.back();
										
										if (clientList.size() == 1)
											client->HostId(1);
										else
											client->HostId(clientList.back()->HostId()+1);

										client->Ip(exploded[1]);
										client->SetIsActiveDirectly(true);
										client->KeyType(true);
										client->KeyValue(exploded[2]);
										client->Port(stringToInt(exploded[3]));
										client->Connect(&it::main, &settings, WM_CLIENTSOCKET);

										buffer.str("");
										buffer << "[Connect] Connecting to client " << client->Nickname() << " [" << client->Ip() << ":" << client->Port() << "]...";
										eb::messages += buffer.str();

										connectingTo = client->HostId();
									}
									break;

								case MSGS::CLIENTNOTFOUND:
									eb::messages += "Client not found.";
									break;

								case MSGS::PING:
									{
										buffer.str("");
										buffer << MSGS::PONG << "|" << exploded[1];
										command.Send(CMD::JOIN, buffer.str(), &server);
									}
									break;

								case MSGS::CANNOTADDYOURSELF:
									eb::messages += "You cannot add yourself to conversation.";
									break;

								case MSGS::CLIENTNOTACTIVE:
									eb::messages += "User is offline";
									break;
								}

							}
							break;

						case CMD::DISCONNECT:
							{
								server.SetIsActiveDirectly(false);									
								MessageBox(0,"Server closed connection","Connection closed!",MB_ICONINFORMATION|MB_OK);
								eb::messages += "Server closed connection";

								SendMessage(eb::addClient.HWnd(), EM_SETREADONLY, 1, 0);
								EnableWindow(btn::addClient.HWnd(),FALSE);
								eb::addClient.Text("Server is down. Now you cannot add new persons.");
							}
							break;

						//default:
							//eb::messages += command.Content();
						}
					}
					//else
						//eb::messages += command.Content();
				}
				break;

			case FD_CLOSE:
				{
					if (server.IsActive() == true)
					{
						server.SetIsActiveDirectly(false);
						MessageBox(0,"Server closed connection","Connection closed!",MB_ICONINFORMATION|MB_OK);
						eb::messages += "Server closed connection";
					}
					SendMessage(eb::addClient.HWnd(), EM_SETREADONLY, 1, 0);
					EnableWindow(btn::addClient.HWnd(),FALSE);
					eb::addClient.Text("Server is down. Now you cannot add new persons.");
				}
				break;
			}
		}
		break;

	case WM_CLIENTSOCKET:
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

					if(clientList.size()<settings.MaxClients())
					{
						clientList.push_back(client);

						if (client->Socket()==INVALID_SOCKET)
						{
							buffer.str("");
							buffer << "Connection failed because of error no "<< WSAGetLastError();
							eb::messages += buffer.str();

							buffer.str("");

							shutdown(client->Socket(), SD_BOTH);
							closesocket(client->Socket());
							clientList.pop_back();
							delete client;
						}
						else
						{
							if (clientList.size() == 1)
								client->HostId(1);
							else
								client->HostId(clientList.back()->HostId()+1);

							if (client->SockAddr()->sa_family == AF_INET)
							{
								buffer.str("");
								buffer << (UINT)client->SockAddr()->sa_data[2] << "." << (UINT)client->SockAddr()->sa_data[3] << "." << (UINT)client->SockAddr()->sa_data[4] << "." << (UINT)client->SockAddr()->sa_data[5];
								client->Ip(buffer.str());
								buffer.str("");
								buffer << MSGS::REQUEST_USERDATA;
								command.Send(CMD::REQUEST, buffer.str(), client);
							}
							else
							{
								buffer.str("");
								buffer << "Client with unresolved address family connected";
								command.Send(CMD::CONNECT, MSGS::UNRESOLVEDADDRESSFAMILY, client);
								clientList.pop_back();
								client->Disconnect();
								delete client;
								eb::messages += buffer.str();

							}
						}
					}
					else
					{
						eb::messages += "Connection attempt rejected because of maximum number of clients";
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
								case CMD::MESSAGE:
									{
										buffer.str("");
										buffer << "[" << client->Nickname() << "] " << command.Content();
										eb::messages += buffer.str();
									}
									break;

								case CMD::REQUEST:
									{
										vector < string > exploded = command.UnserializeContent('|');
										int comm = stringToInt(exploded[0]);

										switch (comm)
										{
											case MSGS::REQUEST_USERDATA:
												{
													me.sendMyData(client);
												}
												break;

											case MSGS::REQUEST_USERDATA_QUESTION:
												{
													buffer.str("");
													buffer << MSGS::REQUEST_CLIENTLIST;
													command.Send(CMD::REQUEST, buffer.str(), client);
												}
												break;

											case MSGS::REQUEST_CLIENTLIST:
												{
													for(unsigned int j=0;j<clientList.size();j++)
														if (clientList[j] != clientList[i])
														{
															clientList[j]->sendMyData(client, true);
															Sleep(150);
														}

													buffer.str("");
													buffer << MSGS::RESPONSE_CLIENTLIST << "|" << MSGS::ENDLIST;
													command.Send(CMD::RESPONSE, buffer.str(), client);
												}
												break;
										}
									}
									break;

								case CMD::RESPONSE:
									{
										vector < string > exploded = command.UnserializeContent('|');
										int comm = stringToInt(exploded[0]);

										switch (comm)
										{
											case MSGS::RESPONSE_USERDATA:
												{
													int index = 1;
													client->Nickname(exploded[index++]);
													client->SetIsActiveDirectly(true);
													client->KeyType(true);
													client->KeyValue(exploded[index++]);
													client->Port(stringToInt(exploded[index++]));

													command.Send(CMD::REQUEST, MSGS::REQUEST_USERDATA_QUESTION, client);
												}
												break;

											case MSGS::RESPONSE_CLIENTLIST:
												{
													int index = 1;
													if (client->HostId() == connectingTo)
													{
														if (stringToInt(exploded[1]) == MSGS::ENDLIST)
														{
															buffer.str("");
															buffer << CMD::CONNECTION_SUCCESS;
															command.Send(CMD::RESPONSE, buffer.str(), client);
														}
														else
														{
															bool alreadyExists=false;
															for (unsigned int j=0;j<clientList.size();j++)
															{
																if (clientList[j]->Nickname() == clientList[i]->Nickname())
																{
																	alreadyExists = true;
																	break;
																}
															}

															/*if (alreadyExists == false)
															{*/
																client = new Host;
																clientList.push_back(client);

																client->Nickname(exploded[index++]);
																client->SetIsActiveDirectly(true);
																client->KeyType(true);
																client->KeyValue(exploded[index++]);
																client->Port(stringToInt(exploded[index++]));
																client->Ip(exploded[index++]);
																client->Connect(&it::main, &settings, WM_CLIENTSOCKET);

																buffer.str("");
																buffer << "[Connect] Connecting to client " << client->Nickname() << " [" << client->Ip() << ":" << client->Port() << "]...";
																eb::messages += buffer.str();
															//}
														}
													}
													else
													{
														buffer.str("");
														buffer << CMD::CONNECTION_SUCCESS << "|" << MSGS::ENDLIST;
														command.Send(CMD::RESPONSE, buffer.str(), client);
													}
												}
												break;

											case CMD::CONNECTION_SUCCESS:
												{
													if (exploded.size() < 2)	// no endlist parameter
													{
														buffer.str("");
														buffer << CMD::CONNECTION_SUCCESS << "|" << MSGS::ENDLIST;
														command.Send(CMD::RESPONSE, buffer.str(), client);
													}

													buffer.str("");
													buffer << "Connection process success";
													eb::messages += buffer.str();

													SendMessage(eb::toSend.HWnd(), EM_SETREADONLY, 0, 0);
													EnableWindow(btn::send.HWnd(),TRUE);
													eb::toSend.Text(ENTERTEXT);
												}
												break;
										}
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
							buffer << "[" << clientList[i]->Nickname() << "] disconnected.";

							clientList.erase(clientList.begin()+i);

							if (clientList.size() == 0)
							{
								EnableWindow(btn::send.HWnd(),TRUE);
								SendMessage(eb::toSend.HWnd(), EM_SETREADONLY, 1, 0);
								eb::toSend.Text("You must add someone to conversation before texting.");
							}

							eb::messages += buffer.str();
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
		if (HIWORD(wParam) == EN_SETFOCUS)
		{
			if (LOWORD(wParam) == GetDlgCtrlID(eb::toSend.HWnd()) && eb::toSend.Text() == ENTERTEXT)
				eb::toSend.Text("");
		}
		else if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			if (LOWORD(wParam) == GetDlgCtrlID(eb::toSend.HWnd()) && eb::toSend.Text().length() == 0)
				eb::toSend.Text(ENTERTEXT);
		}
		else
		{
			switch (LOWORD(wParam))
			{
			case btn::SEND:
				{
					if (eb::toSend.Text().length() != 0)
					{
						for (unsigned int i=0;i<clientList.size();i++)
						{
							command.Send(CMD::MESSAGE, eb::toSend.Text(), clientList[i]);
						}

						buffer.str("");
						buffer << "[Me] " << eb::toSend.Text();
						eb::messages += buffer.str();
						eb::toSend.Text("");
					}
				}
				break;

			case btn::ADDCLIENT:
				{
					Host *client = new Host;
					clientList.push_back(client);

					client->Nickname(eb::addClient.Text());

					buffer.str("");
					buffer << "Pooling server for user " << client->Nickname() << "...";
					eb::messages += buffer.str();

					buffer.str("");
					buffer << MSGS::NICKNAMEDETAILS << "|" << client->Nickname();
					command.Send(CMD::JOIN, buffer.str(), &server);
				}
				break;

			case ID_COMMUNICATOR_CLOSE:
				{
					int option;
					option = MessageBox(it::main.HWnd(), "Do you realy want to close application?", "Prompt", MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL);
					if (option == 6)
						SendMessage(it::main.HWnd(), WM_DESTROY, 0, 0);
				}
				break;

			case ID_CONVERSATION_SAVETOFILE:
				{
					eb::messages.saveContentToFile(&it::main);
				}
				break;

			case ID_CONVERSATION_CLEARCONVERSATION:
				{
					int option;
					option = MessageBox(it::main.HWnd(), "Do you realy want to clear conversation?\r\nThis operation cannot be reverted!", "Prompt", MB_YESNO|MB_ICONQUESTION|MB_APPLMODAL);
					if (option == 6)
					{
						eb::messages.Text("");
						eb::messages += "Conversation cleared";
					}
				}
				break;

			case ID_CONVERSATION_ADDPERSONTOCONVERSATION:
				{
					/*ShowWindow(it::addPerson.HWnd(), SW_SHOW);
					UpdateWindow(it::addPerson.HWnd());*/
				}
				break;

			}
		}
		}
		break;

	case WM_DESTROY:
		{
			shutdown(Socket,SD_BOTH);
			closesocket(Socket);
			PostQuitMessage(0);
			return 0;
		}
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}