#include "stdafx.h"

void WINAPI ServiceMain(DWORD argc, LPWSTR *argv)
{
    
	DWORD result = ERROR_SUCCESS;
	DWORD lastError = ERROR_SUCCESS;
		
#ifdef SERVICE

	_RPTFW0(_CRT_WARN, L"Registering Ctrl Handler.\n");
	
	serviceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME,
		                           (LPHANDLER_FUNCTION)ServiceCtrlHandler);
    if (!serviceStatusHandle)
	{
	    _RPTFW0(_CRT_WARN, L"!serviceStatusHandle.\n");	
		goto error;
	}
#endif

	_RPTFW0(_CRT_WARN, L"Initialize the service.\n");
	// Initialize the service. 
	if (!ServiceInit(argc, argv))
		goto error;
	_RPTFW0(_CRT_WARN, L"Service initialized.\n");

#ifdef SERVICE
	// Indicate that the service is properly initialized and running.
	result = UpdateServiceStatus(SERVICE_RUNNING, NO_ERROR, 0, 0);
	if (!result)
		goto error;
#endif

	// wait for any of the events to be signalled
	
	_RPTFW1(_CRT_WARN, L"stopFlag : %d\n", stopFlag);
	

	while(!stopFlag)
	{
		result = WaitForMultipleObjectsEx(WAIT_OBJECT_COUNT, &waitObjects[0], FALSE, INFINITE, FALSE);
		switch (result)
		{
		case WAIT_OBJECT_0 + STOP_EVENT:
			_RPTFW0(_CRT_WARN, L"Stop event signalled.\n");		// v ServiceCtrlHandler sa nam zmenil stopFlag na true, cyklus sa skonci
			result = timeFunction(1);
			if (result !=ERROR_SUCCESS){
				goto error;
			}
			break;
		case WAIT_OBJECT_0 + REGISTRY_EVENT:
			_RPTFW0(_CRT_WARN, L"Registry change signalled.\n");
			//RegNotifyChangeKeyValue();	// musi sa nastavit aby mohol prijat dalsi event
			RegNotifyChangeKeyValue(registryHandle,TRUE,REG_NOTIFY_CHANGE_LAST_SET,waitObjects[1],TRUE);
			if ((NULL == waitObjects[1]) || (INVALID_HANDLE_VALUE == waitObjects[1]))
				goto error;
			LogKeyValue();
			result = timeFunction(2);
			if (result !=ERROR_SUCCESS){
				goto error;
			}
			break;
		case WAIT_OBJECT_0 + PAUSE_EVENT:
			#ifdef SERVICE
			UpdateServiceStatus(SERVICE_PAUSED, NO_ERROR, 0, 0);
			_RPTFW0(_CRT_WARN, L"Service status is now SERVICE_PAUSED\n");
			serviceCurrentStatus = SERVICE_PAUSED;
			#endif
			_RPTFW0(_CRT_WARN, L"Pause event signalled.\n");
			result = WaitForMultipleObjectsEx(PAUSE_OBJECT_COUNT, &pauseObjects[0], FALSE, INFINITE, FALSE);
			switch (result)
			{
			case WAIT_OBJECT_0 + STOP_EVENT:
				 _RPTFW0(_CRT_WARN, L"Stop event signalled.\n");
				 break;
			case WAIT_OBJECT_0 + CONTINUE:
				 _RPTFW0(_CRT_WARN, L"Continue event signalled.\n");
				 break;
			default:
				goto error;
			}
			break;
		default:
			goto error;
		}
	}
	goto final;

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"Error occured, error code : %d.\n", lastError);

final:
	_RPTFW0(_CRT_WARN, L"Leaving ServiceMain.\n");
	ServiceStop();
#ifdef SERVICE
	UpdateServiceStatus(SERVICE_STOPPED, lastError, 0, 0);
#endif
	return;
}





int ServiceInit(int argc, WCHAR ** argv)
{
	DWORD result = TRUE;
	DWORD lastError = ERROR_SUCCESS;
	int i = 0;

	registryHandle = NULL;

	_RPTFW0(_CRT_WARN, L"Setting global variables to NULL.\n");

	for (i = 0; i < WAIT_OBJECT_COUNT; i++)
	{
		waitObjects[i] = NULL;
	}
	for (i = 0; i < PAUSE_OBJECT_COUNT; i++)
	{
		pauseObjects[i] = NULL;
	}
	result=RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"Software\\Wow6432Node\\xmacejov",0,KEY_READ,&registryHandle);
	//result=RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKeyName,0,KEY_READ,&registryHandle);
	if (result)
	{
		_RPTFW0(_CRT_WARN, L"Application never runned before.\n");
		result = FALSE;
		goto final;
	}
	if ((registryHandle==NULL)||(registryHandle==INVALID_HANDLE_VALUE))
		goto error;
	result=TRUE;

#ifdef SERVICE
	result = UpdateServiceStatus(SERVICE_START_PENDING,NO_ERROR,0,3000);
	if (!result)
		goto error;
	_RPTFW0(_CRT_WARN, L"Service Status is now SERVICE_START_PENDING\n");
#endif

	// Event signalled when the service is stopping
	waitObjects[0] = CreateEvent(
		NULL, 
		FALSE, 
		FALSE, 
		STOP_EVENT_NAME
		);
	if (NULL == waitObjects[0])
		goto error;
	_RPTFW1(_CRT_WARN, L"Creating event, handle : %p\n", waitObjects[0]);

#ifdef SERVICE
	result = UpdateServiceStatus(SERVICE_START_PENDING,NO_ERROR,0,3000);
	if (!result)
		goto error;
#endif
	waitObjects[1] = CreateEvent(
		NULL, 
		FALSE, 
		FALSE, 
		CHANGE_EVENT_NAME
		);
	if ((NULL == waitObjects[1]) || (INVALID_HANDLE_VALUE == waitObjects[1]))
		goto error;
	RegNotifyChangeKeyValue(registryHandle,TRUE,REG_NOTIFY_CHANGE_LAST_SET,waitObjects[1],TRUE);
	if ((NULL == waitObjects[1]) || (INVALID_HANDLE_VALUE == waitObjects[1]))
		goto error;
	_RPTFW1(_CRT_WARN, L"Notify change key value wait handle %p.\n", waitObjects[1]);

#ifdef SERVICE
	result = UpdateServiceStatus(SERVICE_START_PENDING,NO_ERROR,0,3000);
	if (!result)
		goto error;
	_RPTFW0(_CRT_WARN, L"Service Status is now SERVICE_START_PENDING\n");
#endif

	waitObjects[2] = CreateEvent(
		NULL, 
		FALSE, 
		FALSE, 
		PAUSE_EVENT_NAME
		);
	if (NULL == waitObjects[2])
		goto error;
	_RPTFW1(_CRT_WARN, L"Creating event, handle : %p\n", waitObjects[0]);

#ifdef SERVICE
	result = UpdateServiceStatus(SERVICE_START_PENDING,NO_ERROR,0,3000);
	if (!result)
		goto error;
	_RPTFW0(_CRT_WARN, L"Service Status is now SERVICE_START_PENDING\n");
#endif

	// Event signalled when the service is stopping
	pauseObjects[0] = waitObjects[0];
	//staci pauseObjects[0] = waitObjects[0];
	//pauseObjects[0] = CreateEvent(
	//	NULL, 
	//	FALSE, 
	//	FALSE, 
	//	STOP_EVENT_NAME
	//	);
	if (NULL == pauseObjects[0])
		goto error;
	_RPTFW1(_CRT_WARN, L"Creating event, handle : %p\n", pauseObjects[0]);

#ifdef SERVICE
	result = UpdateServiceStatus(SERVICE_START_PENDING,NO_ERROR,0,3000);
	if (!result)
		goto error;
	_RPTFW0(_CRT_WARN, L"Service Status is now SERVICE_START_PENDING\n");
#endif

	// Event signalled when the service is continues
	pauseObjects[1] = CreateEvent(
		NULL, 
		FALSE, 
		FALSE, 
		CONTINUE_EVENT_NAME
		);
	if (NULL == pauseObjects[1])
		goto error;
	_RPTFW1(_CRT_WARN, L"Creating event, handle : %p\n", pauseObjects[1]);

	goto final;

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_ERROR, L"Dispatch initialization failed with error : %d\n", lastError);
	result = FALSE;
final:
	_RPTFW1(_CRT_WARN, L"Leaving ServiceStart, result: %d\n", result);
	return result;
}


int ServiceStop()
{
	int i;
	_RPTFW0(_CRT_WARN, L"Entering ServiceStop.\n");

#ifdef SERVICE
	UpdateServiceStatus(SERVICE_STOP_PENDING, 0, 0, 20000);
	_RPTFW0(_CRT_WARN, L"Service status is now SERVICE_STOP_PENDING\n");
	serviceCurrentStatus = SERVICE_STOP_PENDING;
#endif


	_ASSERTE(waitObjects);
	for (i = 0; i < WAIT_OBJECT_COUNT; i++)
	{
		if (waitObjects[i] != NULL)
		{
			_RPTW1(_CRT_WARN, L"Attempt to free wait object handle no. %d\n", i);
			CloseHandle(waitObjects[i]);
		}
	}
	// CloseHandle k pauseObjects
	if (pauseObjects[1] != NULL)
	{
		_RPTW1(_CRT_WARN, L"Attempt to free continue object handle ", i);
		CloseHandle(pauseObjects[1]);
	}


	_RPTFW0(_CRT_WARN, L"Closing registry key handle.\n");
	if ((registryHandle!=NULL)&&(registryHandle!=INVALID_HANDLE_VALUE))
		RegCloseKey(registryHandle);


	_RPTFW1(_CRT_WARN, L"_CrtDumpMemoryLeaks() returned %d\n", _CrtDumpMemoryLeaks());
	if ((hLogFile != NULL) && (hLogFile != INVALID_HANDLE_VALUE))
	{
		_RPTFW0(_CRT_WARN, L"Attempting to free log file handle\n");
		CloseHandle(hLogFile);
	}
	
	return 0;
}

BOOL UpdateServiceStatus(DWORD currentState, 
						 DWORD win32ExitCode,
						 DWORD serviceSpecificExitCode,
						 DWORD waitHint
						 )
{

	SERVICE_STATUS ss;
	DWORD result;
	
	_RPTFW0(_CRT_WARN, L"UpdateServiceStatus()\n");

	serviceCurrentStatus = currentState;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = currentState;

	switch (currentState)
	{
	case SERVICE_START_PENDING:
	case SERVICE_STOP_PENDING:
		ss.dwControlsAccepted = 0;	// sluzba aktualne neprijima ziadne prikazy
		serviceCurrentCheckpoint++;
		break;
	case SERVICE_PAUSED:
	case SERVICE_RUNNING:
	case SERVICE_STOPPED:
		ss.dwControlsAccepted = SERVICE_ACCEPT_STOP			
			|SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;	// sluzbu je mozne zastavit/pauznut/nechat pokracovat
		serviceCurrentCheckpoint = 0;
	}

	if(serviceSpecificExitCode == 0)
	{
		ss.dwWin32ExitCode = win32ExitCode;
	}
	else
	{
		ss.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
	}

	ss.dwServiceSpecificExitCode = serviceSpecificExitCode;
	ss.dwCheckPoint = serviceCurrentCheckpoint;
	ss.dwWaitHint = serviceCurrentCheckpoint == 0 ? 0 : waitHint;

	_RPTFW3(_CRT_WARN, L"Changing Service Status to %d, checkpoint %d, hint %d\n", currentState, serviceCurrentCheckpoint, waitHint);
	
	result = SetServiceStatus(serviceStatusHandle, &ss);

	if (!result)
		goto error;

	goto final;

error:
	_RPTFW1(_CRT_ERROR, L"Error occured, error code : %d.\n", GetLastError());
	return FALSE;

final:
	return TRUE;
}

VOID LogKeyValue()
{
	DWORD errorState;
	DWORD type = 0;
	DWORD bytes = 0;
	DWORD number = 0;
	errorState=RegQueryValueEx(
		registryHandle,
		L"RunCount",
		NULL,
		&type,
		NULL,
		&bytes
		);
	if (errorState!=ERROR_SUCCESS)
		_RPTFW0(_CRT_ERROR, L"Error loading value from register key.\n");
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
			_RPTFW0(_CRT_ERROR, L"Error loading value from register key.\n");
		_RPTFW1(_CRT_WARN, L"Application running %d. time \n", number);			
	}
}
