// Uloha_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	WORD lastError = ERROR_SUCCESS;
	DWORD result = ERROR_SUCCESS;
	DWORD eventsResult;
	DWORD i=2;

	BOOL allThreadsFinished=FALSE;

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

	// Initialize global variables
	result = Initialize();
	if (result != ERROR_SUCCESS)
		goto error;
	
	hThread1 = CreateThread(
		NULL,
		0,
		&FirstThreadFunc,
		NULL,
		0,
		NULL
		);
	if (hThread1 == NULL)
		goto error;
	
	hThread2 = CreateThread(
		NULL,
		0,
		&SecondThreadFunc,
		NULL,
		0,
		NULL
		);
	if (hThread2 == NULL)
		goto error;

	while (i>0)
	{
		eventsResult=WaitForMultipleObjects(
			HANDLESCOUNT,
			hEvents,
			FALSE,
			TIMEOUT
			);

		switch (eventsResult) 
		{
		// first thread finished
		case WAIT_OBJECT_0 + 0:
			wprintf_s(L"1st thread processed poem to file: %s\n", newTextFilePath);
			_RPTFW1(_CRT_WARN, L"1st thread processed poem to file: %s\n", newTextFilePath);
			FindNextChangeNotification(hEvents[0]);
			i--;
			break; 
		// second thread finished
		case WAIT_OBJECT_0 + 1:
			wprintf_s(L"Process name: %s\n", processName);
			wprintf_s(L"Number of threads: %d\n", numThreads);
			wprintf_s(L"Usage count: %d\n", procUsage);
			_RPTFW1(_CRT_WARN, L"Process name: %s\n", processName);
			_RPTFW1(_CRT_WARN, L"Number of threads: %d\n", numThreads);
			_RPTFW1(_CRT_WARN, L"Usage count: %d\n", procUsage);
			ResetEvent(hEvents[1]);
			i--;
			break;
		//first thread error
		case WAIT_OBJECT_0 + 2: 
			ResetEvent(hEvents[2]);
			i--;
			wprintf_s(L"An error occured in first thread. \n");
			_RPTFW0(_CRT_WARN, L"An error occured in first thread. \n");
			break; 
		//second thread error
		case WAIT_OBJECT_0 + 3: 
			ResetEvent(hEvents[3]);
			i--;
			wprintf_s(L"An error occured in second thread. \n");
			_RPTFW0(_CRT_WARN, L"An error occured in second thread. \n");
			break; 
		//do nothing if timeouted
		case WAIT_TIMEOUT:
			break;

		// Return value is invalid.
		default: 
			goto error; 
		}
	}


	goto final;

error:
	lastError = GetLastError();
final:
	Finalize(lastError);
	return lastError;
}

DWORD Initialize()
{
	DWORD lastError = ERROR_SUCCESS;
	DWORD i;
		// set global variables to NULL
	for(i=0;i<HANDLESCOUNT;i++)
	{
		hEvents[i]=NULL;
	}

	newTextFilePath=(LPWSTR) malloc(MAXIMUM_PATH);
	ZeroMemory(newTextFilePath,MAXIMUM_PATH);
	wcscpy(newTextFilePath, PATH_FOR_POEM);
	wcscat(newTextFilePath, OUTPUT_POEM);

	processName=(LPWSTR) malloc(MAXIMUM_PATH);
	ZeroMemory(processName,MAXIMUM_PATH);
	numThreads=0;
	procUsage=0;

	//creating directory for new poem text file
	//directory already exists error is not a problem
	if	(CreateDirectory(
			PATH_FOR_POEM,
			NULL
		)==ERROR_PATH_NOT_FOUND){
		goto error;
	}

	hEvents[0] = FindFirstChangeNotification(
		PATH_FOR_POEM,
		FALSE,
		FILE_NOTIFY_CHANGE_LAST_WRITE
		);
	if (hEvents[0] == NULL)
		goto error;
	_RPTFW1(_CRT_WARN, L"Change notification handle : %p\n", hEvents[0]);

	hEvents[1] = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		DONE_EVENT
		);
	if (hEvents[1] == NULL)
		goto error;
	_RPTFW1(_CRT_WARN, L"Event handle : %p\n", hEvents[1]);

	hEvents[2] = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		ERROR_EVENT_1
		);
	if (hEvents[2] == NULL)
		goto error;
	_RPTFW1(_CRT_WARN, L"Error event handle : %p\n", hEvents[2]);

	hEvents[3] = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		ERROR_EVENT_2
		);
	if (hEvents[3] == NULL)
		goto error;
	_RPTFW1(_CRT_WARN, L"Error event handle : %p\n", hEvents[3]);

	goto final;

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"An error occured while creating event: %d\n", lastError);
final:
	return lastError;
}

DWORD WINAPI FirstThreadFunc(PVOID unusedParam)
{
	LPWSTR poem = NULL;
	LPWSTR pointer_poem1=NULL;
	LPWSTR pointer_poem2=NULL;
	LPWSTR line = NULL;
	DWORD result = ERROR_SUCCESS;
	HANDLE fileHandle = NULL;

	DWORD bytesWrite = 0;
	int counter= 0;
	char buffer[1024] = "\0";
	DWORD lastError = ERROR_SUCCESS;

	_RPTFW0(_CRT_WARN, L"1st thread: Start.\n");

	poem = GetPoem();

	if(poem == NULL)
	{
		_RPTFW0(_CRT_WARN, L"1st thread: No poem in text file found.\n");
		goto error;
	}

	_RPTFW0(_CRT_WARN, L"1st thread: Processing loaded poem.\n");
	//allocating memory for single line
	line=(LPWSTR) malloc(wcslen(poem));
	//pointers for orientation in poem
	pointer_poem1=poem+1;
	pointer_poem2=poem+1;
	//path to new text file

	_RPTFW0(_CRT_WARN, L"1st thread: Creating new file for poem.\n");
	fileHandle = CreateFile(
		newTextFilePath,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if((fileHandle == INVALID_HANDLE_VALUE) || (fileHandle == NULL))
		goto error;

	while (wcslen(pointer_poem1)>0)
	{
		//first pointer is on first char of line, second pointer goes behind last char
		pointer_poem2=wcschr(pointer_poem1,'.');
		//length of line
		counter=wcslen(pointer_poem1)-wcslen(pointer_poem2);
		//cleaning buffers
		ZeroMemory(line, wcslen(poem));
		ZeroMemory(buffer, 1024);
		wcsncat(line,pointer_poem1,counter);
		wcstombs(buffer,line,counter);
		strcat(buffer,"\r\n");
		WriteFile(
			fileHandle,
			("%s",buffer),
			counter+2,
			&bytesWrite,
			NULL
			);
		if (pointer_poem2[1]=='|'){
			WriteFile(
				fileHandle,
				"\r\n",
				2,
				&bytesWrite,
				NULL
				);
			pointer_poem2++;
		}
		pointer_poem1=pointer_poem2+1;
	}
	goto final;


error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"1st thread: An error occured: %d.\n", lastError);
	result = SetEvent(hEvents[2]);
	if (!result)
		_RPTFW1(_CRT_ERROR,L"1st thread: An error occured while setting event: %d",GetLastError());

final:
	_RPTFW0(_CRT_WARN, L"1st thread: Attempt to free memory from poem.\n");
	if (poem != NULL)
	{
		free(poem);
	}
	if (line != NULL)
	{
		free(line);
	}	
	_RPTFW0(_CRT_WARN, L"1st thread: Attempt to free file handle.\n");
	if((fileHandle != INVALID_HANDLE_VALUE) && (fileHandle != NULL))
	{
			CloseHandle(fileHandle);
	}
	_RPTFW0(_CRT_WARN, L"1st thread: End.\n");
	return 0;

}

LPWSTR GetPoem()
{
	HANDLE hPoem = NULL;
	BOOL result = FALSE;
	DWORD bytesRead = 0;
	LPWSTR bufferForFile = NULL;
	DWORD fileSize = 0;
	DWORD lastError = ERROR_SUCCESS;

	// otvori pozadovany subor
	_RPTFW0(_CRT_WARN, L"1st thread: Openning file with poem.\n");
	hPoem = CreateFile(
		POEM_FILE_NAME,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if((hPoem == INVALID_HANDLE_VALUE) || (hPoem == NULL))
	{
		_RPTFW0(_CRT_ERROR, L"1st thread: No file found.\n");
		goto error;
	}
	// naalokuje pamat podla velkosti suboru
	fileSize = GetFileSize(hPoem, NULL);

	_RPTFW0(_CRT_WARN, L"1st thread: Allocating memory.\n");

	bufferForFile = (LPWSTR) malloc(fileSize + 2);
	if(bufferForFile == NULL)
		goto error;
	ZeroMemory(bufferForFile, fileSize + 2);
	
	// nacita obsah suboru
	_RPTFW0(_CRT_WARN, L"1st thread: Loading data from file.\n");
	ReadFile(
		hPoem,
		bufferForFile,
		fileSize,
		&bytesRead,
		NULL
		);
	if(bytesRead == 0)
		goto error;

	goto final;
error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"1st thread: An error occured, error code : %d.\n", lastError);
final:
	_RPTFW0(_CRT_WARN, L"1st thread: Attempt to free file handle.\n");
	if((hPoem != INVALID_HANDLE_VALUE) && (hPoem != NULL))
	{
			CloseHandle(hPoem);
	}
	if (wcslen(bufferForFile)<1)
	{
		return NULL;
	}
	else
	{
	return bufferForFile;
	}
}



DWORD WINAPI SecondThreadFunc(PVOID unusedParam)
{
	DWORD lastError = ERROR_SUCCESS;
	DWORD result = ERROR_SUCCESS;
	BOOL bResult = FALSE;
	DWORD pid = 0;	// current process id

	HANDLE snapshotHandle = NULL;
	PROCESSENTRY32 processEntry;

	_RPTFW0(_CRT_WARN, L"2nd thread: Start.\n");

	pid = GetCurrentProcessId();
	snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if ((snapshotHandle == INVALID_HANDLE_VALUE) || (snapshotHandle == NULL))
		goto error;
	
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	bResult =  Process32First(snapshotHandle, &processEntry);
	if (bResult == FALSE)
	{
		wprintf_s(L"2nd thread: No processes found.\n");
		_RPTFW0(_CRT_WARN, L"2nd thread: No processes found.\n");
		goto final;
	}
	_RPTFW0(_CRT_WARN, L"2nd thread: Finding current process.\n");
	while (Process32Next(snapshotHandle, &processEntry))
	{
		if (processEntry.th32ProcessID == pid)
			break;
	}

	_RPTFW0(_CRT_WARN, L"2nd thread: Saving process info.\n");
	wcscpy(processName, processEntry.szExeFile);
	numThreads=processEntry.cntThreads;
	procUsage=processEntry.cntUsage;

	result = SetEvent(hEvents[1]);
		if (!result)
		_RPTFW1(_CRT_ERROR,L"2nd thread: An error occured while setting event: %d",GetLastError());

	goto final;

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"2nd thread: An error occured: %d.\n", lastError);
	result = SetEvent(hEvents[3]);
	if (!result)
		_RPTFW1(_CRT_ERROR,L"2nd thread: An error occured while setting event: %d",GetLastError());
final:
	if ((snapshotHandle != NULL) && (snapshotHandle != INVALID_HANDLE_VALUE))
		CloseHandle(snapshotHandle);
	_RPTFW0(_CRT_WARN, L"2nd thread: End.\n");
	return lastError;
}



VOID Finalize(DWORD lastError)
{
	DWORD i;

	if ((hEvents[0] != NULL) && (INVALID_HANDLE_VALUE != hEvents[0] ))
	{
		_RPTFW0(_CRT_WARN, L"Attempt to free change notification object handle.\n");
		FindCloseChangeNotification(hEvents[0]);
	}

	for (i=1;i<4;i++)
	{
		if ((hEvents[i] != NULL) && (INVALID_HANDLE_VALUE != hEvents[i]))
		{
			_RPTFW0(_CRT_WARN, L"Attempt to free event handle.\n");
			CloseHandle(hEvents[i]);
		}

	}

	if ((hThread1 != NULL) && (INVALID_HANDLE_VALUE != hThread1))
	{
		_RPTFW0(_CRT_WARN, L"Attempt to free first thread handle.\n");
		CloseHandle(hThread1);
	}
	
	if ((hThread2 != NULL) && (INVALID_HANDLE_VALUE != hThread2))
	{
		_RPTFW0(_CRT_WARN, L"Attempt to free second thread handle.\n");
		CloseHandle(hThread2);
	}

	if((logFileHandle != INVALID_HANDLE_VALUE) && (logFileHandle != NULL))
	{
		_RPTFW1(_CRT_WARN, L"Leaving _tmain, lastError: %d\n", lastError);
		CloseHandle(logFileHandle);
	}

	return;
}
