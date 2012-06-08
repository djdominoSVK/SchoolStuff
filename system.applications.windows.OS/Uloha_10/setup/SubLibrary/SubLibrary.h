

#ifdef SUBLIBAPI

#else

#define SUBLIBAPI extern __declspec(dllimport)

#endif

#define EVENTS_FILE_NAME L"events.txt"

SUBLIBAPI DWORD timeFunction(DWORD flag);
SUBLIBAPI DWORD keyValue();
