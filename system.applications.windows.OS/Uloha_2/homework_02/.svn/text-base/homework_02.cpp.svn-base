// homework_02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if ((argc==2)&&(argv!=NULL))
	{
		if (_tcscmp(_T("-l"),argv[1])==0)
		{
			return PrintOutProcesses();
		}
		else if (_tcscmp(_T("-d"),argv[1])==0)
		{
			DWORD currentProcessId = 0;
			currentProcessId = GetCurrentProcessId();
			return ProcessDetails(currentProcessId);
		}
		else if (_tcscmp(_T("-k"),argv[1])==0)
		{
			return CreateAndTerminateChild();
		}
		else
		{
			BadParameters();
		}
	}
	else
	{
		BadParameters();
	}
	return 0;
}

void BadParameters()
{
	wprintf_s(L"Program have to run with one of these pamateres: \n");
	wprintf_s(L" -l list of current process with name and ID \n");
	wprintf_s(L" -d details about own process \n");
	wprintf_s(L" -k application creates child process and terminate it");
}

DWORD PrintOutProcesses()
{
	DWORD pids[1024];
	DWORD numPids;
	DWORD numProc;
	HANDLE hProcess;
	DWORD i;
	DWORD lastError = ERROR_SUCCESS;

	EnumProcesses(pids, 1024 * sizeof(DWORD), &numPids);
	if ((pids==NULL)&&(numPids>0))
	{
		goto error;
	}

	numProc = numPids/sizeof(DWORD);

	for (i = 0; i < numPids; i++)
	{
		hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE, 
			pids[i]
		);

		if ((INVALID_HANDLE_VALUE != hProcess ) && (NULL!= hProcess))
		{
		   HMODULE hMod;
           DWORD cbNeeded;
		   WCHAR processName[1024];

		   if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
			   &cbNeeded) )
		   {
			   if(hMod==NULL)
				   goto error;
			   GetModuleBaseName( hProcess, hMod, processName, 
				   sizeof(processName)/sizeof(TCHAR) );
			   wprintf_s(L"Process name: %-25s ID: %u\n", processName, pids[i]);
		   }
		}
    }
	goto final;

error:
	lastError = GetLastError();
	ProcessError( lastError, L"Error occured");
	goto final;
final:
	if ((INVALID_HANDLE_VALUE != hProcess ) && (NULL!= hProcess))
		CloseHandle(hProcess);
	return lastError;
}

DWORD ProcessDetails(DWORD pid)
{
	HANDLE snapShotHandle = NULL;
	PROCESSENTRY32 processEntry;

	THREADENTRY32 threadEntry;

	BOOL bResult = FALSE;

	DWORD lastError = ERROR_SUCCESS;
	
	snapShotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if ((snapShotHandle == INVALID_HANDLE_VALUE) || (snapShotHandle == NULL))
		goto error;
	
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	bResult =  Process32First(snapShotHandle, &processEntry);
	if (bResult == FALSE)
	{
		wprintf_s(L"No processes found");
		goto final;
	}
	while (Process32Next(snapShotHandle, &processEntry))
	{
		if (processEntry.th32ProcessID == pid)
			break;
	}

	wprintf_s(L"Process name: %s\n", processEntry.szExeFile);
	wprintf_s(L"Process ID: %u\n", pid);
	wprintf_s(L"Number of threads: %d\n", processEntry.cntThreads);
	wprintf_s(L"Usage count: %d\n", processEntry.cntUsage);

	snapShotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	if ((snapShotHandle == INVALID_HANDLE_VALUE) || (snapShotHandle == NULL))
		goto error;

	threadEntry.dwSize = sizeof(THREADENTRY32);

	bResult = Thread32First(snapShotHandle, &threadEntry);
	if (bResult == FALSE)
	{
		wprintf_s(L"No thread found");
		goto final;
	}

	wprintf_s(L"Thread Id: %d\n", threadEntry.th32ThreadID);
	goto final;

error:
	lastError = GetLastError();
	ProcessError( lastError, L"Error occured");
	goto final;
final:
	if ((snapShotHandle != INVALID_HANDLE_VALUE) && (snapShotHandle != NULL))
		CloseHandle(snapShotHandle);
	return lastError;
}


DWORD CreateAndTerminateChild()
{
	DWORD lastError = ERROR_SUCCESS;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;
	BOOL result;
	DWORD exitCode = 0;
	WCHAR processName[1024] = L"C:\\Windows\\System32\\cmd.exe";

	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&processInformation, sizeof(processInformation));
	
	result = CreateProcess(
		processName,
		NULL,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&startupInfo,
		&processInformation);
	if (result!=TRUE)
		goto error;
	if ((processInformation.hProcess == INVALID_HANDLE_VALUE) || (processInformation.hProcess == NULL))
		goto error;	
	wprintf_s(L"Child process: %s\n", processName);
	WaitForSingleObject(processInformation.hProcess, 1000);

	TerminateProcess(processInformation.hProcess,exitCode);

	GetExitCodeProcess(processInformation.hProcess, &exitCode);
	wprintf_s(L"Child process terminated with code : %u\n", exitCode);
	
	goto final;

error:
	lastError = GetLastError();
	ProcessError( lastError, L"Error occured");
	goto final;
final:
	if ((processInformation.hProcess != INVALID_HANDLE_VALUE) && (processInformation.hProcess != NULL))
		CloseHandle(processInformation.hProcess);
	if ((processInformation.hThread != INVALID_HANDLE_VALUE) && (processInformation.hThread != NULL))
		CloseHandle(processInformation.hThread);
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

