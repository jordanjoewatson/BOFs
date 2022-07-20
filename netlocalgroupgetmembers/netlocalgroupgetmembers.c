#include <windows.h>
#include "beacon.h"
#include <lmaccess.h>

DECLSPEC_IMPORT NET_API_STATUS WINAPI NETAPI32$NetLocalGroupGetMembers(LPWSTR, LPWSTR, DWORD, LPBYTE, DWORD, LPDWORD, LPDWORD, LPDWORD);

void go(char* args, int len) 
{
        //datap parser;
        //BeaconDataParse(&parser, args, len);

        //LPCWSTR *groupname;
        //groupname = BeaconDataExtract(&parser, NULL);
        LPCWSTR groupname = L"administrators";

        NET_API_STATUS status;

        LOCALGROUP_MEMBERS_INFO_1* buffer = NULL;
        DWORD preffered_max_len = MAX_PREFERRED_LENGTH;
        DWORD entries_read = 0;
        DWORD total_entries = 0;
        PDWORD_PTR resume_handle = 0;
        status = NETAPI32$NetLocalGroupGetMembers(NULL, groupname, 1, (LPBYTE*)&buffer, preffered_max_len, &entries_read, &total_entries, resume_handle);

        for (DWORD i = 0; i < entries_read; i++) {
                BeaconPrintf(CALLBACK_OUTPUT, "%S", buffer[i].lgrmi1_name);
        }

        return;
}
