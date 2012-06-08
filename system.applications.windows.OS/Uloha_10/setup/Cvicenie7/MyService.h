#pragma once

#define LOG_FILE_NAME L"debug.log"
#define SERVICE_NAME L"MyService"
#define STOP_EVENT_NAME L"StopEvent"
#define PAUSE_EVENT_NAME L"PauseEvent"
#define CHANGE_EVENT_NAME L"ChangeEvent"
#define CONTINUE_EVENT_NAME L"CountinueEvent"

#define STOP_EVENT 0
#define REGISTRY_EVENT 1
#define PAUSE_EVENT 2
#define CONTINUE 1

#define WAIT_OBJECT_COUNT 3
#define PAUSE_OBJECT_COUNT 2

#define subKeyName L"Software\\xmacejov"

// Service functions
void WINAPI ServiceMain(DWORD argc, LPWSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD contol);

int ServiceInit(int argc, WCHAR ** argv);
int ServiceStop();
VOID LogKeyValue();
BOOL UpdateServiceStatus(
		DWORD currentState, 
		DWORD win32ExitCode,
		DWORD serviceSpecificExitCode,
		DWORD waitHint);

SERVICE_STATUS_HANDLE serviceStatusHandle;
HANDLE hLogFile;
HANDLE waitObjects[3];
HANDLE pauseObjects[2];

//typedef DWORD (*INTOP) (DWORD);
//typedef DWORD (*INIT) ();
//typedef VOID (*FINAL) ();
//INTOP time;
//INIT startTimeFunct;
//FINAL finalTimeFunct;
//HINSTANCE inst;

BOOL stopFlag;

DWORD serviceCurrentStatus;
DWORD serviceCurrentCheckpoint;

HKEY registryHandle;