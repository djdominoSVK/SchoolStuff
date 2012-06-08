// hw6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD lastError = ERROR_SUCCESS;
	DWORD whatHappenned = 0;
	HKEY hMyKey = NULL;
	HANDLE logFileHandle = NULL;
	DWORD type = 0;
	DWORD bytes = 0;
	DWORD number = 1;

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

	_RPTFW0(_CRT_WARN, L"Opening or creating registry key.\n");
	lastError = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		subKeyName,
		0,
		NULL,
		REG_OPTION_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hMyKey,
		&whatHappenned
		);
	if ((hMyKey==NULL)||(hMyKey==INVALID_HANDLE_VALUE))
		goto error;
	if (lastError!=ERROR_SUCCESS)
		goto error;

	if (whatHappenned==REG_CREATED_NEW_KEY)
	{
		_RPTFW0(_CRT_WARN, L"New registry key created.\n");
		lastError=RegSetValueEx(
			hMyKey,
			valueName,
			0,
			REG_DWORD,
			(PBYTE) &number,
			sizeof(number)
			);
		if (lastError!=ERROR_SUCCESS)
			goto error;
		_RPTFW1(_CRT_WARN, L"Application running %d. time \n", number);
		wprintf(L"Application running %d. time \n", number);
	}
	else
	{
		_RPTFW0(_CRT_WARN, L"Existing registry key opened, loading RunCount value.\n");
		lastError=RegQueryValueEx(
			hMyKey,
			valueName,
			NULL,
			&type,
			NULL,
			&bytes
			);
		if (lastError!=ERROR_SUCCESS)
			goto error;
		_RPTFW0(_CRT_WARN, L"Checking RunCount value type.\n");
		if (type == REG_DWORD)
		{
			lastError=RegQueryValueEx(
				hMyKey,
				valueName,
				NULL,
				&type,
				(PBYTE) &number,
				&bytes
				);
			if (lastError!=ERROR_SUCCESS)
				goto error;
			number++;
			_RPTFW1(_CRT_WARN, L"Application running %d. time \n", number);
			wprintf(L"Application running %d. time \n", number);

			lastError=RegSetValueEx(
				hMyKey,
				valueName,
				0,
				REG_DWORD,
				(PBYTE) &number,
				sizeof(number)
				);
			if (lastError!=ERROR_SUCCESS)
				goto error;
		}
		else
		{
			_RPTFW0(_CRT_ERROR, L"Existing registry key has different value type.\n");
			wprintf(L"Existing registry key has different value type.\n");
			goto final;
		}
	}
	wprintf(L"HKEY_LOCAL_MACHINE\\%s\\%s \n",subKeyName,valueName);
	_RPTFW2(_CRT_WARN, L"HKEY_LOCAL_MACHINE\\%s\\%s \n",subKeyName,valueName);
	goto final;

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"An error occured: %d\n", lastError);
	ProcessError( lastError,  L"Error occured" );
final:
	_RPTFW0(_CRT_ERROR, L"Closing registry key handle.\n");
	if ((hMyKey!=NULL)&&(hMyKey!=INVALID_HANDLE_VALUE))
		RegCloseKey(hMyKey);
	_RPTFW0(_CRT_WARN, L"Closing log file.\n");
	if ((logFileHandle!=NULL)&&(hMyKey!=logFileHandle))
		CloseHandle(logFileHandle);
	return lastError;
}

VOID ProcessError(DWORD lastError, LPWSTR userMessage)
{
	WCHAR message[1024];

	ZeroMemory(message, 2048);

	FormatMessage( 
	    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		lastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		message, 
		1024, 
		NULL );

	wprintf_s(L"%s: %d : %s", userMessage, lastError, message);

}

