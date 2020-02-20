#include "Hack.h"

DWORD NAKED FASTCALL DrawRectangleFrameSTUB(RECT * Rectangle)
{
	__asm
	{
		MOV EAX, ECX;
		JMP D2CLIENT_DrawRectFrame;
	}
}

DWORD NAKED FASTCALL GetInventoryIDSTUB(LPUNITANY Unit, DWORD ID, DWORD Unknown)
{
	__asm
	{
		PUSH ESI;
		PUSH EDI;
		TEST ECX, ECX;
		JZ SHORT LOC3;
		MOV EAX, p_D2CLIENT_pUnitList;
		MOV EAX, [EAX];
		MOV ECX, [ECX]Unit.dwUnitId;
		TEST EAX, EAX;
		JZ SHORT LOC3;
		MOV ESI, [ESP + 0x0C];

LOC1:
		CMP [EAX + 4], EDX;
		JNZ SHORT LOC2;
		CMP [EAX + 0CH], ECX;
		JNZ SHORT LOC2;
		TEST ESI, ESI;

		JNZ SHORT LOC4;
		MOV EDI, [EAX + 20H];
		TEST EDI, EDI;
		JNZ SHORT LOC4;

LOC2:
		MOV EAX, [EAX + 30H];
		TEST EAX, EAX;
		JNZ SHORT LOC1;

LOC3:
		POP EDI;
		OR EAX, 0FFFFFFFFH;
		POP ESI;
		RETN 4;

LOC4:
		MOV EAX, [EAX + 8];
		POP EDI;
		POP ESI;
		RETN 4;
	}
}

DWORD NAKED FASTCALL GetUIVarSTUB(DWORD UI)
{
	__asm
	{
		MOV EAX, ECX;
		JMP D2CLIENT_GetUiVar_I;
	}
}

DWORD NAKED FASTCALL GetUnitNameSTUB(DWORD Unit)
{
	__asm
	{
		MOV EAX, ECX;
		JMP D2CLIENT_GetUnitName_I;
	}
}

DWORD NAKED FASTCALL GetUnitSTUB(DWORD dwUnitId, DWORD dwUnitType)
{
	__asm
	{
		POP EAX;
		PUSH EDX;
		PUSH EAX;
		SHL EDX, 9;
		MOV EAX, D2CLIENT_GetUnitFromId_I;
		ADD EDX, EAX;
		MOV EAX, ECX;
		AND EAX, 0x7F;
		JMP D2CLIENT_GetUnitFromId_II;
	}
}

DWORD NAKED FASTCALL LoadUIImage(LPSTR szPath)
{
	__asm
	{
		MOV EAX, ECX;
		PUSH 0;
		CALL D2CLIENT_LoadUiImage_I;
		RETN;
	}
}

DWORD NAKED FASTCALL InitAutomapLayerSTUB(DWORD dwLayer)
{
	__asm
	{
		PUSH EAX;
		MOV EAX, ECX;
		CALL D2CLIENT_InitAutomapLayer_I;
		POP EAX;
		RET;
	}
}

DWORD NAKED FASTCALL TestPvPFlagSTUB(DWORD Argument1, DWORD Argument2, DWORD Flag)
{
	__asm
	{
		PUSH ESI;
		PUSH [ESP + 8];
		MOV ESI, EDX;
		MOV EDX, ECX;
		CALL D2CLIENT_TestPvPFlag_I;
		POP ESI;
		RET 4;
	}
}