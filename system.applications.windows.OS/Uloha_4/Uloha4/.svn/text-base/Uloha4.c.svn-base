// Uloha4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	pointerAnsi=NULL;
	lastError=ERROR_SUCCESS;

	logFileHandle = CreateFile(
		LOG_FILE_NAME,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if((logFileHandle == INVALID_HANDLE_VALUE) || (logFileHandle == NULL))
		goto error;

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
	_CrtSetReportFile(_CRT_WARN, logFileHandle);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
	_CrtSetReportFile(_CRT_ERROR, logFileHandle);

	_RPTFW0(_CRT_WARN, L"Starting debug logs.\n");

	if ((argc!=2)||(argv==NULL))
		goto help;
	
	_RPTFW0(_CRT_WARN, L"Openning file.\n");	
	hFile = CreateFile(
		argv[1],
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	
	if((hFile == INVALID_HANDLE_VALUE) || (hFile == NULL))
		goto error;

	_RPTFW0(_CRT_WARN, L"Getting size of file.\n");
	fileSize = GetFileSize(hFile,NULL);
	if (fileSize==0)
	{
		wprintf_s(L"File is empty, nothing to reverse.\n");
		_RPTFW0(_CRT_ERROR, L"File is empty, nothing to reverse.\n");
		goto final;
	}
	
	_RPTFW0(_CRT_WARN, L"Mapping file.\n");
	hFileMapping = CreateFileMapping(hFile,
		NULL,
		PAGE_READWRITE,
		0,
		fileSize+1,
		NULL);
	if((hFileMapping == INVALID_HANDLE_VALUE) || (hFileMapping == NULL))
		goto error;

	_RPTFW0(_CRT_WARN, L" Creating view of file.\n");
	viewOfFile=MapViewOfFile(hFileMapping,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		fileSize+1);
	if(viewOfFile == NULL)
		goto error;

	isUnicode=IsTextUnicode(viewOfFile,fileSize,NULL);

	if (isUnicode){
		_wcsrev((LPWSTR)viewOfFile);
		wcscat((LPWSTR)viewOfFile,L"0");
		pointerUnicode = (LPWSTR)viewOfFile;
		while (pointerUnicode!=NULL){
			pointerUnicode=wcsstr(pointerUnicode,L"\n\r");
			if (pointerUnicode!=NULL && wcslen(pointerUnicode)>1)
			{
				pointerUnicode[0]=L'\r';
				pointerUnicode[1]=L'\n';
			}
		}
	}
	else{
		_strrev((CHAR*)viewOfFile);
		strcat((CHAR*)viewOfFile,"0");
		pointerAnsi=(CHAR*)viewOfFile;
		while (pointerAnsi!=NULL){
			pointerAnsi=strstr(pointerAnsi,"\n\r");
			if (pointerAnsi!=NULL){
				pointerAnsi[0]='\r';
				pointerAnsi[1]='\n';
			}
		}
	}

	_RPTFW0(_CRT_WARN, L"Closing view of file.\n");
	if(viewOfFile == NULL)
		goto error;
	UnmapViewOfFile(viewOfFile);

	_RPTFW0(_CRT_WARN, L"Unmapping file.\n");
	if((hFileMapping == INVALID_HANDLE_VALUE) || (hFileMapping == NULL))
		goto error;
	CloseHandle(hFileMapping);

	_RPTFW0(_CRT_WARN, L"Setting file pointer.\n");
	if (SetFilePointer(hFile,
		fileSize,
		0,
		FILE_BEGIN)==INVALID_SET_FILE_POINTER)
		goto error;

	_RPTFW0(_CRT_WARN, L"Setting end of file.\n");
	if (SetEndOfFile(hFile)==0)
		goto error;

	goto final;

help:
	_RPTFW0(_CRT_ERROR, L"No parameter selected.\n");
	wprintf_s(L"Program have to run with one paramater,\n");
	wprintf_s(L"path to file that should be reversed!\n");

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"An error occured: %d\n", lastError);

final:
	_RPTFW0(_CRT_WARN, L"Closing handle of opened file.\n");
	if((hFile != INVALID_HANDLE_VALUE) && (hFile != NULL))
		CloseHandle(hFile);
		_RPTFW0(_CRT_WARN, L"Closing log file.\n");
	if((logFileHandle != INVALID_HANDLE_VALUE) && (logFileHandle != NULL))
		CloseHandle(logFileHandle);
	return lastError;
}

