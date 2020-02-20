#include "Hack.h"

VOID GameInfo()
{
	if (V_RememberGameName || V_RememberGamePassword)
	{
		LPGAMESTRUCTINFO Info = D2CLIENT_GetGameInfo();

		if (Info)
		{
			if (V_RememberGameName) wcscpy(V_GameName, Info->szGameName);
			if (V_RememberGamePassword) wcscpy(V_GamePassword, Info->szGamePassword);
		}
	}
}

VOID FASTCALL LogInGameMessages(WCHAR * Message)
{
	CHAR buf[0x400];
	CHAR c;
	c = 0;
	LPSTR p = buf;

	for (WCHAR * pw = Message; *pw; ++pw)
	{
		if (*pw == 0xFF)
			c = 3;

		if (c != 0)
			--c;
		else
			*p++ = *(LPSTR)pw;
	}

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	CHAR File[MAX_PATH];
	strncpy_s(File, sizeof(File), V_FileNamePath, sizeof(File));
	strcat_s(File, "\\Message Log.txt");

	HANDLE hFile = OpenFileWrite(File);
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile, buf, (DWORD)strlen(buf));
	CloseHandle(hFile);
}

BOOL FASTCALL NextGameName(LPCONTROL Box, BOOL (STDCALL * FunCallBack)(LPCONTROL, DWORD, DWORD))
{
	if (!V_RememberGameName || wcslen(V_GameName) <= 0)
		return FALSE;

	D2WIN_SetControlText(Box, V_GameName);
	D2WIN_SelectEditBoxText(Box, V_GameName);
	D2WIN_SetEditBoxProc(Box, FunCallBack);
	return TRUE;
}

BOOL FASTCALL NextGamePassword(LPCONTROL Box, BOOL (STDCALL * FunCallBack)(LPCONTROL, DWORD, DWORD))
{
	if (!V_RememberGamePassword || wcslen(V_GamePassword) <= 0)
		return FALSE;

	D2WIN_SetControlText(Box, V_GamePassword);
	D2WIN_SelectEditBoxText(Box, V_GamePassword);
	D2WIN_SetEditBoxProc(Box, FunCallBack);
	return TRUE;
}