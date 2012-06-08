#pragma once

#define LOG_FILE_NAME L"debug.log"
#define PATH_FOR_POEM L"C:\\myPoem"
#define NOTIFICATION_OBJECT L"notificationObject"
#define DONE_EVENT L"doneEvent"
#define ERROR_EVENT_1 L"errroEvent1"
#define ERROR_EVENT_2 L"errorEvent2"
#define MAXIMUM_PATH 128
#define HANDLESCOUNT 4 
#define TIMEOUT 5000 
#define POEM_FILE_NAME L"poem.txt"
#define OUTPUT_POEM L"\\out.txt"


DWORD WINAPI FirstThreadFunc(PVOID);
DWORD WINAPI SecondThreadFunc(PVOID);
DWORD Initialize();
LPWSTR GetPoem();
VOID Finalize(DWORD);


HANDLE logFileHandle;
HANDLE hThread1;
HANDLE hThread2;
HANDLE hEvents[4];

LPWSTR newTextFilePath;
LPWSTR processName;
DWORD numThreads;
DWORD procUsage;
