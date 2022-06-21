#include <stdio.h>
#include <Windows.h>
#include "beacon.h"

#pragma comment(lib, "winmm.lib")

WINBASEAPI HANDLE WINAPI KERNEL32$CreateFileA (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI HANDLE WINAPI KERNEL32$CloseHandle (HANDLE hObject); 
WINBASEAPI HANDLE WINAPI KERNEL32$GetFileSize (HANDLE hFile, LPDWORD lpFileSizeHigh); 
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$ReadFile (HANDLE, PVOID, DWORD, PDWORD, LPOVERLAPPED);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$WriteFile (HANDLE, PVOID, DWORD, PDWORD, LPOVERLAPPED);
WINBASEAPI int __cdecl MSVCRT$strcmp (const char* str1, const char* str2);
WINBASEAPI size_t __cdecl MSVCRT$strlen (const char *str);
WINBASEAPI void __cdecl MSVCRT$memset (void *dest, int c, size_t count);
DECLSPEC_IMPORT WINBASEAPI void * WINAPI KERNEL32$HeapAlloc (HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
DECLSPEC_IMPORT WINBASEAPI HANDLE WINAPI KERNEL32$GetProcessHeap();
WINBASEAPI void* WINAPI MSVCRT$malloc(SIZE_T);
DECLSPEC_IMPORT WINBASEAPI DWORD WINAPI WINMM$mciSendStringA (CHAR*, CHAR*, DWORD, DWORD);
DECLSPEC_IMPORT WINBASEAPI VOID WINAPI KERNEL32$GetSystemTime (SYSTEMTIME*);
DECLSPEC_IMPORT WINBASEAPI VOID WINAPI KERNEL32$GetLocalTime (SYSTEMTIME*);
DECLSPEC_IMPORT WINBASEAPI INT WINAPI KERNEL32$FileTimeToSystemTime (FILETIME*, SYSTEMTIME*);
DECLSPEC_IMPORT WINBASEAPI INT WINAPI KERNEL32$SystemTimeToFileTime (SYSTEMTIME*, FILETIME*);
WINBASEAPI char WINAPI MSVCRT$strcat(char *destination, const char *source);


void _sleep(int seconds)
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

int power(int number, int p) 
{
	int total = number;
	for (int itr = 1 ; itr < p ; itr++) 
	{	
		total *= 10;
	}
	return total;
}

int _atoi(char *string_value)
{
	int len = MSVCRT$strlen(string_value);
	int int_value = 0;
	
	for (int itr = 0 ; itr < len ; itr++) 
	{
		if (string_value[itr] == '1') int_value += power(10, (len-itr-1));
		else if (string_value[itr] == '2') int_value += power(2, (len-itr));
		else if (string_value[itr] == '3') int_value += power(3, (len-itr));
		else if (string_value[itr] == '4') int_value += power(4, (len-itr));
		else if (string_value[itr] == '5') int_value += power(5, (len-itr));
		else if (string_value[itr] == '6') int_value += power(6, (len-itr));
		else if (string_value[itr] == '7') int_value += power(7, (len-itr));
		else if (string_value[itr] == '8') int_value += power(8, (len-itr));
		else if (string_value[itr] == '9') int_value += power(9, (len-itr));
	}

	return int_value;
}


size_t _fwrite(const void *ptr, size_t size, size_t nmemb, HANDLE *stream)
{
	KERNEL32$WriteFile(stream, ptr, size, nmemb, NULL);
	return nmemb;	
}

size_t _fread(char *ptr, size_t size, size_t nmemb, HANDLE *stream)
{
	void *tmp_ptr;

	DWORD dwBytesRead;
	DWORD totalBytesRead;

	KERNEL32$ReadFile(stream, &tmp_ptr, 1, &dwBytesRead, NULL);
	totalBytesRead += dwBytesRead;

	int i = 0; 
	while(i < size) 
	{
		ptr[i] = tmp_ptr;	
		KERNEL32$ReadFile(stream, &tmp_ptr, 1, &dwBytesRead, NULL);
		
		totalBytesRead += dwBytesRead;
		i++;
	}

	return 0;	
}



HANDLE *_fopen(const char *filename, const char *mode)
{
	HANDLE *fh;
	DWORD ACCESS_TYPE;
	DWORD CREATE_TYPE;

	if (MSVCRT$strcmp(mode, "r") == 0)
	{
		// read file
		ACCESS_TYPE = GENERIC_READ;
		CREATE_TYPE = OPEN_EXISTING;
	}
	else if (MSVCRT$strcmp(mode, "w"))
	{
		// write file, this will overwrite a file not append
		ACCESS_TYPE = GENERIC_WRITE;	
		CREATE_TYPE = CREATE_ALWAYS;
	}
	else if (MSVCRT$strcmp(mode, "rw"))
	{
		// read/write file
		ACCESS_TYPE = GENERIC_READ | GENERIC_WRITE;
		CREATE_TYPE = CREATE_ALWAYS;
	}
	else
	{
		BeaconPrintf(CALLBACK_OUTPUT, "Mode %s not implemented\n", mode);
		return NULL;
	}

	fh = KERNEL32$CreateFileA(
 		filename, 
 		ACCESS_TYPE, 
 		0, 
 		NULL, 
 		CREATE_TYPE, 
 		FILE_ATTRIBUTE_NORMAL,
 		NULL
 	);

	return fh;
}

int _fclose(HANDLE *fh)
{
	int success = KERNEL32$CloseHandle(fh);
	return success;
}
