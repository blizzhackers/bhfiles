#include "Hack.h"

VOID CheckEditBox()
{
	if (V_EditBox)
	{
		if (*(LPDWORD)p_D2CLIENT_ChatMessage != 0xAD5AFFFF)
		{
			D2WIN_DestroyEditBox(V_EditBox);
			V_EditBox = NULL;
			*p_D2WIN_FocusedControl = NULL;
		}
		
		D2WIN_SetTextSize(1);
	}
}

VOID DestroyEditBox()
{
	if (V_EditBox)
	{
		*(LPDWORD)p_D2CLIENT_ChatMessage = NULL;
		WCHAR * Text = wcscpy(p_D2CLIENT_ChatMessage, D2WIN_GetEditBoxText(V_EditBox));
		*p_D2CLIENT_TextLength = (INT)wcslen(Text);
		D2WIN_DestroyEditBox(V_EditBox);
		V_EditBox = NULL;
		*p_D2WIN_FocusedControl = NULL;
	}
}

BOOL STDCALL EditBoxCallBack(LPCONTROL HWND, DWORD Argument2, DWORD Argument3)
{
	return TRUE;
}

BOOL FUNCCALL InitEditBox()
{
	if (!V_CopyPaste)
		return FALSE;

	if (!V_EditBox)
	{
		static DWORD DW[] = {0x0D, 0};

		INT X = V_Design->GetScreenSizeX2();
		INT Y = V_Design->GetScreenSizeY2();

		if (*p_D2CLIENT_ScreenSizeX == 800 && *p_D2CLIENT_ScreenSizeY == 600)
			V_EditBox = D2WIN_CreateEditBox(0x83, Y + 62, X - 100, 0x2D, 0, 0, 0, (BOOL (STDCALL *)(LPCONTROL))0, (BOOL (STDCALL *)(LPCONTROL))0, sizeof(DW), (LPCONTROLPREFERENCES)DW);
		else if (*p_D2CLIENT_ScreenSizeX == 640 && *p_D2CLIENT_ScreenSizeY == 480)
			V_EditBox = D2WIN_CreateEditBox(0x83, Y - 58, X - 266, 0x2D, 0, 0, 0, (BOOL (STDCALL *)(LPCONTROL))0, (BOOL (STDCALL *)(LPCONTROL))0, sizeof(DW), (LPCONTROLPREFERENCES)DW);

		*p_D2WIN_FocusedControl = V_EditBox;

		if (V_EditBox)
		{
			D2WIN_SetEditBoxProc(V_EditBox, &EditBoxCallBack);
			D2WIN_SetEditBoxText(V_EditBox, p_D2CLIENT_ChatMessage);
		}

		*(LPDWORD)p_D2CLIENT_ChatMessage = 0x0AD5AFFFF;
	}
	
	if (V_EditBox)
		(V_EditBox->Initialize)(V_EditBox);
	
	return TRUE;
}

BOOL FUNCCALL InputLine(BYTE Key)
{
	if (V_CopyPaste && Key != VK_ESCAPE)
	{
		if (Key != VK_RETURN)
			return TRUE;
	
		if (V_EditBox)
			wcscpy(p_D2CLIENT_ChatMessage, D2WIN_GetEditBoxText(V_EditBox));
	}
	
	return FALSE;
}