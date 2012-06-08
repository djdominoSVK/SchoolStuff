#pragma once

#define LOG_FILE_NAME L"debug.log"

HANDLE logFileHandle;
HANDLE hFile;
HANDLE hFileMapping;
DWORD fileSize;
DWORD lastError;
PVOID viewOfFile;
BOOL isUnicode;
LPWSTR pointerUnicode;
char* pointerAnsi;