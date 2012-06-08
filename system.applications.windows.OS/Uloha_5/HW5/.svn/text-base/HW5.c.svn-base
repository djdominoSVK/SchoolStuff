// HW5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD lastError=ERROR_SUCCESS;
	LPWSTR filePath=NULL;
	HANDLE logFileHandle=NULL;
	HANDLE openFileHandle= NULL;
	
	BOOL copyingBool=TRUE;
	DWORD counter=0;
	DWORD readed = 0;
	DWORD lengthToRead=0;
    LPWSTR buffer = NULL;
	LARGE_INTEGER lengthOfFile = {0};
	

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
	
	Init();
	
	counter=0;
	wprintf(L"Choose existing no empty directory: \n");
	wscanf(L"%s",directoryPath);
	while((counter<2)&&!OpenDir())
	{
		wprintf(L"Choose existing no empty directory (for example C:\\mydir): \n");
		wscanf(L"%s",directoryPath);
		counter++;
	}
	if(!OpenDir())
	{
		_RPTFW0(_CRT_WARN, L"No path selected. \n");
		wprintf(L"No path selected. \n");
		lastError=ERROR_INVALID_COMMAND_LINE;
		goto final;
	}

	wcscat(directoryPath,L"\\*");

	_RPTFW0(_CRT_WARN, L"Loading files in directory.\n");
	if(!LoadFiles(directoryPath))
	{
		_RPTFW0(_CRT_WARN, L"Directory is empty. \n");
		wprintf(L"Directory is empty. \n");
		lastError=ERROR_INVALID_COMMAND_LINE;
		goto final;
	}
    wprintf(L"Select number of file to show: \n");
	counter=0;
	while((counter<2)&&!LoadFileNumber())
	{
		wprintf(L"Select number of file to show: \n");
		counter++;
	}
	if (counter==2)
	{
		if(!LoadFileNumber())
		{
			_RPTFW0(_CRT_WARN, L"Invalid input. \n");
			wprintf(L"Invalid input. \n");
			lastError=ERROR_INVALID_COMMAND_LINE;
			goto final;
		}
	}
	_RPTFW0(_CRT_WARN, L"Allocating memory for path of file to read.\n");
	filePath=(LPWSTR) malloc(sizeof(WCHAR)*MAX_PATH);
	ZeroMemory(filePath,MAX_PATH);
	wcscat(filePath,directoryPath);
	filePath[wcslen(directoryPath)-1]='\0';
	counter = wcslen(filePath)+wcslen(findData[fileNumber-1].cFileName);
	if ((wcslen(filePath)+wcslen(findData[fileNumber-1].cFileName)) > MAX_PATH)
	{
		_RPTFW0(_CRT_WARN, L"Path too long. \n");
		wprintf(L"Path too long. \n");
		lastError=ERROR_FILENAME_EXCED_RANGE;
		goto final;
	}
	wcscat(filePath,findData[fileNumber-1].cFileName);

	_RPTFW0(_CRT_WARN, L"Opening file to read.\n");
    openFileHandle = CreateFile(
        filePath,
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
		0,
        NULL);
    if (openFileHandle == INVALID_HANDLE_VALUE)
        goto error;

	_RPTFW0(_CRT_WARN, L"Getting file size.\n");
	GetFileSizeEx(openFileHandle,&lengthOfFile);
	
	if (lengthOfFile.HighPart != 0)
        goto error;

	if (lengthOfFile.LowPart==0)
	{
		wprintf(L"File is empty. /n", buffer);
	}
	else
	{

		lengthToRead=lengthOfFile.LowPart +sizeof(WCHAR);

		_RPTFW0(_CRT_WARN, L"Allocating memory for buffer.\n");	
		buffer = (LPWSTR) malloc(lengthToRead);

		ZeroMemory(buffer, lengthToRead);

		_RPTFW0(_CRT_WARN, L"Reading file.\n");	    
		ReadFile(openFileHandle,(LPVOID) buffer, lengthToRead-1 , &readed, NULL);
		wprintf(L"%s\n", buffer);
	}
	FileList();

	//for possibility delete opened file
	if((openFileHandle != INVALID_HANDLE_VALUE) && (openFileHandle != NULL))
	{
		CloseHandle(openFileHandle);
		openFileHandle=NULL;
	}

    wprintf(L"Select number of file to delete: \n");
	counter=0;
	while((counter<2)&&!LoadFileNumber())
	{
		wprintf(L"Select number of file to delete: \n");
		counter++;
	}
	if (counter==2)
	{
		if(!LoadFileNumber())
		{
			_RPTFW0(_CRT_WARN, L"Invalid input. \n");
			wprintf(L"Invalid input. \n");
			lastError=ERROR_INVALID_COMMAND_LINE;
			goto final;
		}
	}

	_RPTFW0(_CRT_WARN, L"Allocating memory for path of file to delete.\n");
	ZeroMemory(filePath,MAX_PATH);
	wcscat(filePath,directoryPath);
	filePath[wcslen(directoryPath)-1]='\0';
	if ((wcslen(filePath)+wcslen(findData[fileNumber-1].cFileName)) > MAX_PATH)
	{
		_RPTFW0(_CRT_WARN, L"Path too long. \n");
		wprintf(L"Path too long. \n");
		lastError=ERROR_FILENAME_EXCED_RANGE;
		goto final;
	}
	wcscat(filePath,findData[fileNumber-1].cFileName);

	_RPTFW0(_CRT_WARN, L"Deleting file.\n");
	if(!DeleteFile(filePath))
		goto error;
	wprintf(L"File deleted. \n");

	_RPTFW0(_CRT_WARN, L"Reloading files in directory.\n");
	LoadFiles(directoryPath);

    wprintf(L"Select number of file to copy: \n");
	counter=0;
	while((counter<2)&&!LoadFileNumber())
	{
		wprintf(L"Select number of file to copy: \n");
		counter++;
	}
	if (counter==2)
	{
		if(!LoadFileNumber())
		{
			_RPTFW0(_CRT_WARN, L"Invalid input. \n");
			wprintf(L"Invalid input. \n");
			lastError=ERROR_INVALID_COMMAND_LINE;
			goto final;
		}
	}

	_RPTFW0(_CRT_WARN, L"Allocating memory for path of file to copy.\n");
	ZeroMemory(filePath,MAX_PATH);
	wcscat(filePath,directoryPath);
	filePath[wcslen(directoryPath)-1]='\0';
	if ((wcslen(filePath)+wcslen(findData[fileNumber-1].cFileName)) > MAX_PATH)
	{
		_RPTFW0(_CRT_WARN, L"Path too long. \n");
		wprintf(L"Path too long. \n");
		lastError=ERROR_FILENAME_EXCED_RANGE;
		goto final;
	}
	wcscat(filePath,findData[fileNumber-1].cFileName);

	ZeroMemory(directoryPath,MAX_PATH);
	wprintf(L"Select destination to copy (for example C:\\mydir\\new.txt): \n");
	wscanf(L"%s",directoryPath);
	_RPTFW0(_CRT_WARN, L"Copying file.\n");
	CopyFile(filePath,directoryPath,copyingBool);
	counter=0;
	while((counter<2)&&!copyingBool)
	{
		wprintf(L"Select destination to copy (for example C:\\mydir\\new.txt):\n");
		wscanf(L"%s",directoryPath);
		CopyFile(filePath,directoryPath,copyingBool);
		counter++;
	}
	if(!copyingBool)
	{
		_RPTFW0(_CRT_WARN, L"Invalid input. \n");
		wprintf(L"Invalid input. \n");
		lastError=ERROR_INVALID_COMMAND_LINE;
		goto final;
	}
	wprintf(L"File copied. \n");
	goto final;
error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"An error occured: %d\n", lastError);
	ProcessError( lastError,  L"Error occured" );
final:
	_RPTFW0(_CRT_WARN, L"Free allocated memory. \n");
	if (directoryPath!=NULL)
		free(directoryPath);
	if (filePath!=NULL)
		free(filePath);
	_RPTFW0(_CRT_WARN, L"Closing handles.\n");
	if((openFileHandle != INVALID_HANDLE_VALUE) && (openFileHandle != NULL))
		CloseHandle(openFileHandle);
	if((directoryHandle != INVALID_HANDLE_VALUE) && (directoryHandle != NULL))
		CloseHandle(directoryHandle);
	if((findHandle != INVALID_HANDLE_VALUE) && (findHandle != NULL))
		FindClose(findHandle);
	_RPTFW0(_CRT_WARN, L"Closing log file.\n");
	if((logFileHandle != INVALID_HANDLE_VALUE) && (logFileHandle != NULL))
		CloseHandle(logFileHandle);
	return lastError;
}

BOOL OpenDir()
{
	_RPTFW0(_CRT_WARN, L"Opening directory.\n");
	directoryHandle = CreateFile(
		directoryPath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
		);
	if((directoryHandle == INVALID_HANDLE_VALUE) || (directoryHandle == NULL))
		return FALSE;
	else 
		return TRUE;
}

BOOL LoadFiles(LPWSTR dir)
{
	numFiles=0;
	findHandle=FindFirstFile(dir, &findData[0]);

	if (INVALID_HANDLE_VALUE == findHandle) 
		goto error;

	if (!FindNextFile(findHandle, &findData[0]))
		goto error;

	while (FindNextFile(findHandle, &findData[numFiles]) != 0)
	{
		numFiles++;
		wprintf(L"%d %s %d B \n",numFiles,findData[numFiles-1].cFileName,findData[numFiles-1].nFileSizeLow);
	}
	if (numFiles<1)
	{
		_RPTFW0(_CRT_ERROR, L"Empty directory.\n");
		wprintf(L"Empty directory.\n");
		return FALSE;
	}
	return TRUE;
error:
	_RPTFW0(_CRT_ERROR, L"Invalid directory.\n");
	return FALSE;
}

VOID FileList()
{
	DWORD i=0;
	for(i;i<numFiles;i++)
	{
		wprintf(L"%d %s %d B \n",i+1,findData[i].cFileName,findData[i].nFileSizeLow);
	}
}

BOOL LoadFileNumber()
{
	wscanf(L"%d",&fileNumber);
	if (fileNumber>numFiles||fileNumber<0)
		return FALSE;
	else
		return TRUE;
}

VOID Init()
{
	directoryHandle=NULL;
	findHandle=NULL;
	numFiles=0;
	fileNumber=0;
	_RPTFW0(_CRT_WARN, L"Allocating memory for directory path.\n");		
	directoryPath=(LPWSTR) malloc(sizeof(WCHAR)*MAX_PATH);
	ZeroMemory(directoryPath,MAX_PATH);
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