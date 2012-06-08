// SubLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#define SUBLIBAPI extern __declspec(dllexport)

#include "SubLibrary.h"

SUBLIBAPI DWORD timeFunction(DWORD flag){
	HANDLE fileHandle = NULL;
	LPWSTR time = NULL;
	LPWSTR date = NULL;
	LPWSTR message = NULL;
	int runCount = 0;
	DWORD counter = 0;
	DWORD bytesWrite = 0;
	DWORD lastError = ERROR_SUCCESS;
	SYSTEMTIME st;

	fileHandle = CreateFile(
		EVENTS_FILE_NAME,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if((fileHandle == INVALID_HANDLE_VALUE) || (fileHandle == NULL))
		goto error;

	//SetEndOfFile(fileHandle);
	SetFilePointer(fileHandle, GetFileSize(fileHandle, NULL), 0, FILE_BEGIN);
	time=(LPWSTR) malloc(128);
	ZeroMemory(time, 128);
	date=(LPWSTR) malloc(128);
	ZeroMemory(date, 128);
	message=(LPWSTR) malloc(1024);
	ZeroMemory(message, 1024);

	GetSystemTime (&st);

	GetDateFormat(
		LOCALE_USER_DEFAULT,
		0,
		&st,
		L"dd.MM.yyyy",
		date,     
		20);
	
	GetTimeFormat(
		LOCALE_USER_DEFAULT,
		TIME_FORCE24HOURFORMAT,
		&st,
		L"hh:mm:ss",
		time,
		20);

	wcscat(message,date);
	wcscat(message,L" ");
	wcscat(message,time);
	wcscat(message,L" ");

	switch(flag){
	case 1:
		wcscat(message,L"Service is about to stop.");
		counter= wcslen(message);
		WriteFile(
			fileHandle,
			("%s",message),
			sizeof(wchar_t)*counter,
			&bytesWrite,
			NULL
			);
		break;
	case 2:
		runCount = keyValue();
		//int* test;
		//test = &runCount;
		wcscat(message,L"RunCount value is now: ");
		counter= wcslen(message);
		WriteFile(
			fileHandle,
			("%s",message),
			sizeof(wchar_t)*counter,
			&bytesWrite,
			NULL
			);
			WriteFile(
				fileHandle,
				("%d",&runCount),
				1,
				&bytesWrite,
				NULL
				);
			WriteFile(
				fileHandle,
				"\r\n",
				2,
				&bytesWrite,
				NULL
				);
		break;
	}

	goto final;


error:
	lastError = GetLastError();

final:
	if (time != NULL)
		free(time);
	if (date != NULL)
		free(date);
	if (message != NULL)
		free(message);
	if((fileHandle != INVALID_HANDLE_VALUE) && (fileHandle != NULL))
	{
		CloseHandle(fileHandle);
	}
	return lastError;


}


SUBLIBAPI DWORD keyValue()
{
	HKEY registryHandle;
	DWORD errorState;
	DWORD type = 0;
	DWORD bytes = 0;
	DWORD number = 0;
	DWORD lastError = ERROR_SUCCESS;
	

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Wow6432Node\\xmacejov",0,KEY_READ,&registryHandle);
	
	if ((registryHandle==NULL)||(registryHandle==INVALID_HANDLE_VALUE))
		goto error;	

	RegQueryValueEx(
		registryHandle,
		L"RunCount",
		NULL,
		&type,
		NULL,
		&bytes
		);

	if (type == REG_DWORD)
	{
		errorState=RegQueryValueEx(
			registryHandle,
			L"RunCount",
			NULL,
			&type,
			(PBYTE) &number,
			&bytes
			);
		if (errorState!=ERROR_SUCCESS)
			goto error;	
	}
	goto final;
error:
	lastError = GetLastError();
final:
	if ((registryHandle!=NULL)&&(registryHandle!=INVALID_HANDLE_VALUE))
		RegCloseKey(registryHandle);
	return number;
}