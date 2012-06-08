#pragma once

#define LOG_FILE_NAME L"debug.log"
#define MAX_PATH 128
#define MAX_NUM_OF_FILES 64

BOOL LoadFileNumber();
VOID FileList();
BOOL LoadFiles(LPWSTR dir);
BOOL OpenDir();
VOID Init();
VOID ProcessError(DWORD lastError, LPWSTR userMessage);



HANDLE directoryHandle;
HANDLE findHandle;

DWORD numFiles;
DWORD fileNumber;

LPWSTR directoryPath;

WIN32_FIND_DATA findData[MAX_NUM_OF_FILES];