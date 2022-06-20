#include <stdio.h>
#include <Windows.h>
#include "beacon.h"

WINBASEAPI HANDLE WINAPI KERNEL32$CreateFileA (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI HANDLE WINAPI KERNEL32$CloseHandle (HANDLE hObject); 
WINBASEAPI HANDLE WINAPI KERNEL32$GetFileSize (HANDLE hFile, LPDWORD lpFileSizeHigh); 
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$ReadFile (HANDLE, PVOID, DWORD, PDWORD, LPOVERLAPPED);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$WriteFile (HANDLE, PVOID, DWORD, PDWORD, LPOVERLAPPED);
WINBASEAPI int __cdecl MSVCRT$strcmp (const char* str1, const char* str2);
WINBASEAPI size_t __cdecl MSVCRT$strlen (const char *str);
WINBASEAPI void* WINAPI MSVCRT$malloc(SIZE_T);


size_t _fwrite(const void *ptr, size_t size, size_t nmemb, HANDLE *stream)
{
	KERNEL32$WriteFile(stream, ptr, size, nmemb, NULL);
	return nmemb;	
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

void go(char* args, int len) 
{
	datap parser;	
	BeaconDataParse(&parser, args, len);
		
	CHAR * input_file;
	CHAR * output_file; 
	int ciphern;

	input_file = BeaconDataExtract(&parser, NULL);
	output_file = BeaconDataExtract(&parser, NULL);
	ciphern = BeaconDataInt(&parser);

	LPCSTR filename = TEXT(input_file); 
	const char *mode = 'r';

	HANDLE fileHandle = _fopen(filename, "r");
	
	if(fileHandle != INVALID_HANDLE_VALUE){
		BeaconPrintf(CALLBACK_OUTPUT, "%s handle opened\n\r", input_file);
	} else {
		BeaconPrintf(CALLBACK_OUTPUT, "Unable to open handle to %s\n\r", input_file);
		return;
	}

	DWORD fileSize;
	fileSize = KERNEL32$GetFileSize(fileHandle, NULL);
	char *buffer;	
	buffer = (char *) MSVCRT$malloc(fileSize * sizeof(char));
	KERNEL32$ReadFile(fileHandle, buffer, fileSize, NULL, NULL);
	_fclose(fileHandle);
	
	for (int i = 0; i < fileSize; i++) {
		buffer[i] = (buffer[i] + ciphern)%256;
	}

  	HANDLE *outptr;
	LPCSTR filenameout = TEXT(output_file); 
  	outptr = _fopen(filenameout,"w");

  	if(outptr == NULL)
  	{
    		BeaconPrintf(CALLBACK_OUTPUT, "Unable to open handle to %s\n\r", output_file);  
    		return;
  	}
	else 
	{
		BeaconPrintf(CALLBACK_OUTPUT, "%s handle opened\n\r", output_file);
	}
       
	BeaconPrintf(CALLBACK_OUTPUT, "Writing encrypted data\n\r", output_file);	
	_fwrite(buffer, MSVCRT$strlen(buffer), NULL, outptr);
  	_fclose(outptr);

	return;
}
