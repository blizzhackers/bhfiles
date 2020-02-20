#ifndef _OFFSETS_H
#define _OFFSETS_H

#define INST_INT3	0xCC
#define INST_CALL	0xE8
#define INST_NOP	0x90
#define INST_JMP	0xE9
#define INST_RET	0xC3

#define INGAME	1
#define	OUTGAME	0

typedef struct Patch_t
{
	VOID (*pFunc)(DWORD, DWORD, DWORD);
	DWORD dwAddr;
	DWORD dwFunc;
	DWORD dwLen;
	BOOL Type;
	LPBYTE bOldCode;
} PATCH;

VOID DefineOffsets();
DWORD GetDllOffset(INT Number);
DWORD GetDllOffset(LPSTR DLLName, INT Offset);
VOID RemovePatches(BOOL Type);
VOID InstallPatches(BOOL Type);
VOID PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen);
VOID PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
VOID PatchFill(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
VOID PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
VOID InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen);
VOID FillBytes(LPVOID Address, BYTE Fill, DWORD Length);
BOOL WriteBytes(LPVOID pAddr, VOID *pData, DWORD dwLen);
VOID GamePatch(BOOL InGame);

#endif