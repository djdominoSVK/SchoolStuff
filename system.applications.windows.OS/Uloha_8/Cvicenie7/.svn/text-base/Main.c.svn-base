// Cvicenie7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



int _tmain(int argc, _TCHAR* argv[])
{
	DWORD result = TRUE;
    SERVICE_TABLE_ENTRY serviceTable[]=
	{
		{ (LPWSTR) SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain },
		{ NULL, NULL }
	};

	DWORD lastError = ERROR_SUCCESS;

	hLogFile = NULL;
	hLogFile = CreateFile(
		LOG_FILE_NAME, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	if ((hLogFile == INVALID_HANDLE_VALUE) || (hLogFile == NULL))
		goto error;
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_WARN, hLogFile);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, hLogFile);

	_RPTFW0(_CRT_WARN, L"Entering wmain\n");
    
	if ( argc > 1)
	{
		if ( wcscmp(argv[1], L"-w") == 0)
		{
			_RPTFW0(_CRT_WARN, L"-w argument received\n");
			while (!IsDebuggerPresent())
			SleepEx(10000, FALSE);
			
		}
		
	}
	
#ifdef SERVICE

		_RPTFW0(_CRT_WARN, L"Starting Ctrl Dispatcher.\n"); 
		result = StartServiceCtrlDispatcher(serviceTable);
		if (!result)
			goto error;
#else
		_RPTFW0(_CRT_WARN, L"Starting ServiceMain.\n");
	    ServiceMain(argc, argv);
#endif
	
	
	goto final;

error:
    lastError = GetLastError();
	_RPTFW1(_CRT_WARN, L"Error occured, error code : %d.", lastError);
	if ((hLogFile != INVALID_HANDLE_VALUE) && (hLogFile != NULL))
		CloseHandle(hLogFile);
	return 1;

final:
    return 0;
}



//
// Reacts on service control signals
//
VOID WINAPI ServiceCtrlHandler(DWORD control)
{
	DWORD result = ERROR_SUCCESS;

	_RPTFW1(_CRT_WARN, L"Entering ServiceCtrlHandler , control == %d\n", control);
		
	switch(control)
	{	
	case SERVICE_CONTROL_SHUTDOWN:
	case SERVICE_CONTROL_STOP:
		stopFlag = TRUE;
		_RPTFW0(_CRT_WARN, L"Setting wait event\n");
		result = SetEvent(waitObjects[0]);
		if (!result)
			_RPTFW1(_CRT_ERROR, L"Error occured, error code : %d.\n", GetLastError());
		result = UpdateServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 20000);
		break;
	case SERVICE_CONTROL_PAUSE:
		_RPTFW0(_CRT_WARN, L"Setting wait event\n");
		result = SetEvent(waitObjects[2]);
		if (!result)
			_RPTFW1(_CRT_ERROR, L"Error occured, error code : %d.\n", GetLastError());
		result = UpdateServiceStatus(SERVICE_PAUSED, NO_ERROR, 0, 3000);
		break;
	case SERVICE_CONTROL_CONTINUE:
		_RPTFW0(_CRT_WARN, L"Setting wait event\n");
		result = SetEvent(pauseObjects[1]);		// signalizovany continue event, aby sluzba mohla pokracovat
		if (!result)
			_RPTFW1(_CRT_ERROR, L"Error occured, error code : %d.\n", GetLastError());
		result = UpdateServiceStatus(SERVICE_RUNNING, NO_ERROR, 0, 3000);
		break;
	default:
		break;
	}
	
	_RPTFW0(_CRT_WARN, L"Leaving ServiceCtrlHandler.\n");
}