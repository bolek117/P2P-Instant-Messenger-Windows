#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <wchar.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define DEBUG 1
#define APP_NAME L"SoFive Server"
#define DEFAULT_PORT 666

#define DEFAULT_BUFLEN 512
#define WM_SOCKETMESSAGE WM_USER+1
#define WIN32_LEAN_AND_MEAN

#include "../../Common/ExitCodes.h"
#include "../../Common/CommonConstans.h"
