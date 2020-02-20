#define	PRIMARY		1
#define	DESIGN		2
#define	ENCHANT		3
#define	PACKETS		4
#define PARTY		5
#define TELEPORT	6

typedef struct Thread_t
{
	HANDLE Handle;
	DWORD ID;
} THREAD, *LPTHREAD;

BOOL STDCALL Threads(LPVOID Parameter);
HANDLE MakeThread(LPVOID Process, LPVOID Parameter);
VOID DestroyThread(HANDLE Thread, DWORD Parameter, DWORD MaxWaitTime);

VOID CreateInGameThreads();
VOID DestroyInGameThreads();