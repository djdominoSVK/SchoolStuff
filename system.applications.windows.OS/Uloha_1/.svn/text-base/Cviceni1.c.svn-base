// Cviceni1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "crtdbg.h"

// TODO : osetrit chyby dle vzoru v ucebnim textu
int _tmain(int argc, _TCHAR* argv[])
{
	LPWSTR poem = NULL;
	LPWSTR pointer_poem1=NULL;
	LPWSTR pointer_poem2=NULL;
	LPWSTR line = NULL;
	DWORD bytesWrite = 0;
	int counter= 0;
	char buffer[1024] = "\0";
	DWORD lastError = ERROR_SUCCESS;
	BOOL saveFileStatus = FALSE;
	//unsigned int i=0;

	// vytvori logovaci subor
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
		goto error1;

	// TODO : nastavit hLogFile jako cil chybovych hlaseni
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
	_CrtSetReportFile(_CRT_WARN, logFileHandle);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
	_CrtSetReportFile(_CRT_ERROR, logFileHandle);	
	_RPTFW0(_CRT_WARN, L"Starting debug logs.\n");

	_RPTFW0(_CRT_WARN, L"Getting poem from file.\n");
	poem = GetPoem();

	if(poem == NULL)
		goto error;

	// TODO : spracovat basen podla pokynov v zadani ulohy

	// first idea
	//	for(i=1;i<wcslen(poem);i++){
	//		if (poem[i]!='.'){
	//			wprintf(L"%c",poem[i]);
	//		}
	//		else{
	//			//new verse
	//			wprintf(L"\n");
	//					if (poem[i+1]=='|'){
	//						//new strophe
	//						wprintf(L"\n");
	//						i++;
	//					}
	//		}
	//	}

	_RPTFW0(_CRT_WARN, L"Processing loaded poem.\n");
	//allocating memory for single line
	line=(LPWSTR) malloc(wcslen(poem));
	//pointers for orientation in poem
	pointer_poem1=poem+1;
	pointer_poem2=poem+1;

	//test for arguments, first argument is name of file for formated poem
	if (argc==2){
		saveFileStatus = TRUE;
		_RPTFW0(_CRT_WARN, L"Creating new file for poem.\n");
		fileHandle = CreateFile(
			argv[1],
			GENERIC_ALL,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);

		if((fileHandle == INVALID_HANDLE_VALUE) || (fileHandle == NULL))
			goto error;
	}

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
		wprintf(L"%s \n",line);
		wcstombs(buffer,line,counter);
		strcat(buffer,"\r\n");
		if (saveFileStatus==TRUE){
			WriteFile(
				fileHandle,
				("%s",buffer),
				counter+2,
				&bytesWrite,
				NULL
				);
		}
		if (pointer_poem2[1]=='|'){
			wprintf(L"\n");
			if (saveFileStatus){
				WriteFile(
					fileHandle,
					"\r\n",
					2,
					&bytesWrite,
					NULL
					);
			}
			pointer_poem2++;
		}
		//first pointer goes on first char of next line
		pointer_poem1=pointer_poem2+1;
	}


	goto final;

//error situation without log file
error1:
	lastError = GetLastError();
	wprintf(L"%d",lastError);

error:
	lastError = GetLastError();
	_RPTFW1(_CRT_WARN, L"An error occured, error code : %d.\n", lastError);

final:
	_RPTFW0(_CRT_WARN, L"Attempt to free memory from poem.\n");
	free(poem);
	free(line);
	_RPTFW0(_CRT_WARN, L"Attempt to free file handle.\n");
	CloseHandle(logFileHandle);
	CloseHandle(fileHandle);
	return 0;
}

LPWSTR GetPoem()
{
	HANDLE fileHandle = NULL;
	BOOL result = FALSE;
	DWORD bytesRead = 0;
	LPWSTR bufferForFile = NULL;
	DWORD fileSize = 0;
	DWORD lastError = ERROR_SUCCESS;

	logFileHandle = NULL;

	// otvori pozadovany subor
	_RPTFW0(_CRT_WARN, L"Openning file with poem.\n");
	fileHandle = CreateFile(
		POEM_FILE_NAME,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if((fileHandle == INVALID_HANDLE_VALUE) || (fileHandle == NULL))
		goto error;

	// naalokuje pamat podla velkosti suboru
	fileSize = GetFileSize(fileHandle, NULL);

	_RPTFW0(_CRT_WARN, L"Allocating memory.\n");

	bufferForFile = (LPWSTR) malloc(fileSize + 2);
	if(bufferForFile == NULL)
		goto error;
	ZeroMemory(bufferForFile, fileSize + 2);
	
	// nacita obsah suboru
	_RPTFW0(_CRT_WARN, L"Loading data from file.\n");
	ReadFile(
		fileHandle,
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
	_RPTFW1(_CRT_WARN, L"An error occured, error code : %d.\n", lastError);
final:
	_RPTFW0(_CRT_WARN, L"Attempt to free file handle.\n");
	CloseHandle(fileHandle);
	return bufferForFile;
}