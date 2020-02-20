#include "Hack.h"

BOOL ClientReady(BOOL StateCheck)
{
	if (!Me)
		return FALSE;

	if (StateCheck)
		if (Me->dwMode == PLAYER_MODE_DEAD || Me->dwMode == PLAYER_MODE_DEATH)
			return FALSE;

	if (Me->pPath)
		if (Me->pPath->pRoom1)
			if (Me->pPath->pRoom1->pRoom2)
				if (Me->pPath->pRoom1->pRoom2->pLevel)
					return TRUE;

	return FALSE;
}

LPWSTR ColorD2String(LPWSTR wString, DWORD dwColor)
{
	memmove(wString + 3, wString, (wcslen(wString) + 1) * sizeof(WCHAR));

	wString[0] = 0xFF;
	wString[1] = 'c';
	wString[2] = '0' + (INT)dwColor;

	return wString;
}

INT GetTextWidth(LPSTR szText, DWORD dwSize)
{
	if (strlen(szText) == NULL)
		return NULL;

	DWORD dwWidth, dwFileNo, dwOldSize;
	WCHAR wText[66666] = {0};

	MyMultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szText, (INT)strlen(szText), wText, (INT)strlen(szText));

	RemoveColorSpecs(wText);

	dwOldSize = D2WIN_SetTextSize(dwSize);
	D2WIN_GetTextWidthFileNo(wText, &dwWidth, &dwFileNo);
	D2WIN_SetTextSize(dwOldSize);

	return (V_MusicPlayerSongLength > 0 && dwWidth > (DWORD)V_MusicPlayerSongLength) ? (dwWidth - (DWORD)V_MusicPlayerSongLength) : dwWidth;
}

INT LongestCharName(INT Length, INT Name, INT LongestName)
{
	if (V_Players.IsEmpty())
		return NULL;

	for (LPROSTERUNIT Unit = *p_D2CLIENT_PlayerUnitList; Unit; Unit = Unit->pNext)
	{
		if (Unit != NULL)
			Name = (INT)strlen(Unit->szName);

		if (Name > Length)
			if (Name > LongestName && strcmp(Unit->szName, Me->pPlayerData->szName))
				LongestName = Name;
	}

	return LongestName;
}

VOID MyMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, INT cbMultiByte, LPWSTR lpWideCharStr, INT cchWideChar)
{
	MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);

	for(DWORD i = 0; i < wcslen(lpWideCharStr); i++)
	{
		if(lpWideCharStr[i] >= 0xFF && lpWideCharStr[i + 1] == L'c')
		{
			if(lpWideCharStr[i + 2] >= L'0' && lpWideCharStr[i + 2] <= L'9')
			{
				lpWideCharStr[i] = 0xFF;
			}
		}
	}
}

BOOL Print(BOOL Bottom, INT Color, CHAR * Format, ...)
{
	if (!ClientReady(FALSE))
		return FALSE;

	if (strlen(Format) <= NULL)
		return FALSE;

	CHAR *szString = new CHAR[8192];
	::memset(szString, 0, 8192);
	va_list vaArgs;

	va_start(vaArgs, Format);
	vsprintf(szString, Format, vaArgs);
	va_end(vaArgs);

	WCHAR Buffer[8192] = L"";
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szString, (INT)strlen(szString), Buffer, (INT)strlen(szString));

	if (Bottom)
		D2CLIENT_PrintPartyString(Buffer, Color);
	else
		D2CLIENT_PrintGameString(Buffer, Color);

	delete[] szString;
	return TRUE;
}

VOID PrintPacket(LPBYTE aPacket, DWORD aLen, BOOL PacketReceived)
{
	CHAR Packet[0x400] = "";

	for(DWORD x = 0; x < aLen; x++)
	{
		if (((aPacket[x] & 0xF0) / 0x10) <= 0x9)
			Packet[x * 3] = 48 + ((aPacket[x] & 0xF0) / 0x10);
		else
			Packet[x * 3] = 55 + ((aPacket[x] & 0xF0) / 0x10);

		if ((aPacket[x] & 0x0F) <= 0x9) 
			Packet[x * 3 + 1] = 48 + (aPacket[x] & 0x0F);
		else
			Packet[x * 3 + 1] = 55 + (aPacket[x] & 0x0F);

		Packet[x * 3 + 2] = ' ';
	}

	if (PacketReceived)
		Print(0, 0, "ÿc4::Receivedÿc0: %s", Packet);
	else
		Print(0, 0, "ÿc4::Sentÿc0: %s", Packet);
}

INT RemoveColorSpecs(LPWSTR lpwsz)
{
	if (lpwsz == NULL)
		return 0;

	CONST INT LEN = (INT)wcslen(lpwsz);
	LPWSTR p = new WCHAR[LEN + 1];
	memset(p, 0, sizeof(WCHAR) * (LEN + 1));

	INT nIndex = 0;
	for(INT i = 0; i < LEN; i++)
	{
		if (lpwsz[i] == 0xF8F5 || lpwsz[i] == 0xFF)
			i += 2;
		else
			p[nIndex++] = lpwsz[i];
	}

	wcscpy(lpwsz, p);
	return (INT)wcslen(lpwsz);
}

BOOL Say(LPSTR Message, ...)
{
	CHAR szBuffer[0x400] = "";
	va_list Args;

	va_start(Args, Message);
	vsprintf_s(szBuffer, Message, Args);
	va_end(Args);

	if (strlen(Message) > 0)
	{
		if (Me)
		{
			LPBYTE Packet = new BYTE[(INT)strlen(szBuffer) + 6];
			memset(Packet, 0, (INT)strlen(szBuffer) + 6);

			Packet[0] = 0x15;
			*(LPWORD)&Packet[1] = 1;
			::memcpy(Packet + 3, szBuffer, (INT)strlen(szBuffer));

			D2NET_SendPacket((INT)strlen(szBuffer) + 6, 1, Packet);
			delete [] Packet;
		}

		else
		{
			::memcpy((CHAR *)p_D2MULTI_ChatBoxMsg, szBuffer, strlen(szBuffer) + 1);
			D2MULTI_DoChat();
		}
	}

	return TRUE;
}

VOID ScrollMap(CHAR X, CHAR Y)
{
	if (*p_D2CLIENT_AutomapOn)
	{
		p_D2CLIENT_PtAutomap->x += X * 8;
		p_D2CLIENT_PtAutomap->y += Y * 8;
	}
}