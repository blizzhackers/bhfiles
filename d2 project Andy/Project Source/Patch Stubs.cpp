#include "Hack.h"

VOID NAKED AddShrineSTUB()
{
	__asm
	{
		MOV ECX, [ESI + 0x1BC]
		CMP ECX, CELLNO_SHRINE;
		JNE NotShrine;
		XOR ECX, ECX;
		TEST [ESI + 0x166], 1;
		JE NotShrine;
		MOV EAX, [EBX + 0x14];
		MOVZX ECX, [EAX] ObjectData.Type;
		ADD ECX, CELLNO_MYSHRINES;
NotShrine:
		RET;
	}
}

VOID NAKED AfterAttackSTUB()
{
	__asm
	{
		PUSH 0;
		PUSH EAX;
		PUSH EBX;
		PUSH EDI;
		CALL D2CLIENT_CreateAttack;
		MOV ECX, EDI;
		MOV EDX, EBX;
		CALL GameAfterAttack;
		RETN;
	}
}

VOID NAKED CreateGameBoxSTUB()
{
	__asm
	{
		PUSHAD;
		CALL CreateGameList;
		POPAD;
		MOV EDX, 0x10;
		RETN;
	}
}

VOID NAKED DrawSTUB()
{
	__asm
	{
		PUSHAD;
		CALL Design::GameDraw;
		POPAD;

		POP EBX;
		POP ECX;
		RETN 0x4;
	}
}

VOID NAKED DrawAutomapSTUB()
{
	__asm
	{
		PUSHAD;
		CALL Design::GameDrawAutomap;
		POPAD;
		POP EDI;
		POP ESI;
		RET;
	}
}

VOID NAKED DrawEnteringLevelTextSTUB()
{
	__asm
	{
		CALL D2CLIENT_DrawEnteringLevelText;
		CALL Design::GameDraw3;
		RETN;
	}
}

VOID NAKED DrawManaOrbSTUB()
{
	__asm
	{
		CALL D2CLIENT_DrawManaOrb;
		CALL Design::GameDraw2;
		RETN;
	}
}

VOID NAKED EndSTUB()
{
	__asm
	{
		CALL GameEnd;
		MOV ECX, 0xB4;
		RET;
	}
}

VOID NAKED FrameRemainSTUB()
{
	__asm
	{
		PUSHAD;
		PUSH EAX;
		PUSH ECX;
		PUSH EBP;
		CALL RemainingFrames;
		MOV [ESP + 0x1C], EAX;
		POPAD;
		RETN 4;
	}
}

VOID NAKED FullLightRadiusSTUB()
{
	__asm
	{
		CMP [V_FullLightRadius], 0;
		JE OldCode;
		
		MOV EAX, 0xFF;
		MOV BYTE PTR [ESP + 0x4], AL;
		MOV BYTE PTR [ESP + 0x5], AL;
		MOV BYTE PTR [ESP + 0x6], AL;

		ADD DWORD PTR [ESP], 0x82;
		RET;

OldCode:
		MOV BYTE PTR [ESP + 0x5], 0;
		RET;
	}
}

VOID NAKED InfraVisionSTUB()
{
	__asm
	{
		MOV ECX, ESI;
		CALL InfraVision;
		ADD DWORD PTR [ESP], 0x72;
		RET;
	}
}

VOID NAKED InputSTUB()
{
	__asm
	{
		PUSHAD;
		MOV ECX, EBX;
		CALL Input;
		CMP EAX, 0;
		POPAD;
		JE Block;
		CALL D2CLIENT_InputCall_I;
		RET;
Block:
		XOR EAX, EAX;
		RET;
	}
}

VOID NAKED InputLineSTUB1()
{
	__asm
	{
		MOV EBX, EAX;
		PUSH [EDI + 8]
		CALL InputLine;
		TEST EAX, EAX;
		POP EAX;
		JZ Quit;
		ADD DWORD PTR [ESP], 0x427;
Quit:
		RET;
	}
}

VOID NAKED InputLineSTUB2()
{
	__asm
	{
		CALL InitEditBox;
		TEST EAX, EAX;
		JZ Quit;
		ADD DWORD PTR [ESP], 0x154;
Quit:
		MOV ECX, p_D2CLIENT_ChatMessage;
		RET;
	}
}

VOID NAKED ItemBasicStatSTUB()
{
	__asm
	{
		CMP [V_ViewItemBasicStats], 0;
		JZ CallCode;
		XOR EAX, EAX;
		JMP QuitCode;
CallCode:
		PUSH DWORD PTR [ESP + 4];
		CALL D2COMMON_GetItemFromInventory;
QuitCode:
		RET 4;
	}
}

VOID NAKED ItemNameSTUB()
{
	__asm
	{
		MOV ECX, EDI;
		MOV EDX, EBX;
		CALL ItemName;
		MOV AL, [EBP + 0x12A];
		RET;
	}
}

VOID NAKED LifeBarSTUB()
{
	__asm
	{
		POP EAX;
		MOVZX EDX, [V_LifeBarTransparency];
		CMP DL, -1;
		JNE Default1;
		MOV DL, 1;
Default1:
		PUSH EDX;
		MOV DL, [V_LifeBarColor];
		CMP DL, -1;
		JNE Default2;
		MOV DL, 5;
Default2:
		PUSH EDX;
		SAR EDI, 7;
		JMP EAX;
	}
}

VOID NAKED LoadAutomapLayerFromFileSTUB()
{
	__asm
	{
		POP EAX;
		PUSH Reveal::LoadMyAutomapLayer;
		PUSH EBP;
		MOV EBP, ESP;
		AND ESP, 0xFFFFFFF8;
		JMP EAX;
	}
}

VOID NAKED LoopSTUB()
{
	__asm
	{
		PUSHAD;
		CALL GameLoop;
		POPAD;

		POP EAX;
		SUB ESP, 0x20;
		MOV [ESP + 0x0C], ECX;
		PUSH EAX;
		RET;
	}
}

VOID NAKED MessageLogSTUB1()
{
	__asm
	{
		CMP [V_LogInGameMessages], 0;
		JE OldCode;
		MOV ECX, ESI;
		CALL LogInGameMessages;
OldCode:
		MOV ECX, 0x0D;
		JMP D2WIN_SetTextSize;
	}
}

VOID NAKED MessageLogSTUB2()
{
	__asm
	{
		CMP [V_LogInGameMessages], 0;
		JE OldCode;
		MOV ECX, EBP;
		CALL LogInGameMessages;
OldCode:
		MOV ECX, 0x0D;
		JMP D2WIN_SetTextSize;
	}
}

VOID NAKED MonsterDeathSTUB()
{
	__asm
	{
		CMP [V_MonsterDeathAnimation + EAX], 0;
		JE NoAnimation;
		RET;
NoAnimation:
		POP EAX;
		POP ESI;
		RET 4;
	}
}

VOID NAKED MonsterDescCommaSTUB1()
{
	__asm
	{
		MOV EAX, OFFSET V_MonsterDescComma;
		RET;
	}
}

VOID NAKED MonsterDescCommaSTUB2()
{
	__asm
	{
		MOV EDX, OFFSET V_MonsterDescComma;
		RET;
	}
}

VOID NAKED MonsterDescCommaSTUB3()
{
	__asm
	{
		MOV EDX, [ESP + 0x20];
		CALL MonsterDescComma;
		MOV ECX, [ESP + 0x20];
		RET;
	}
}

VOID NAKED MonsterLifeBarNameSTUB()
{
	__asm
	{
		PUSH EDX;
		MOV EDX, ESP;
		PUSH [ESP + 0x14];
		LEA EAX, [ESP + 0x10];
		PUSH EAX;
		CALL MonsterLifeBarName;
		MOV ECX, EAX;
		POP EDX;
		POP EAX;
		PUSH EBX;
		PUSH ESI;
		MOV ESI, ECX;
		TEST ESI, ESI;
		JMP EAX;
	}
}

VOID NAKED OnGameKeyDownSTUB()
{
	__asm
	{
		MOV CL, [EDI + 0x08];
		MOV DL, [EDI + 0x0C + 3];
		AND DL, 0x40;
		CALL OnGameKeyDown;
		TEST BYTE PTR [EDI + 0x0C + 3], 0x40;
		RET;
	}
}

VOID NAKED OnGamePacketReceivedSTUB()
{
	__asm
	{
		PUSH ECX;
		PUSH EDX;
		CALL OnGamePacketReceived;
		TEST EAX, EAX;
		POP EDX;
		POP ECX;
		JNZ OldCode;
		XOR EDX, EDX;
OldCode:
		JMP D2NET_ReceivePacket_I;
	}
}

VOID NAKED OnGamePacketSentSTUB()
{
	__asm
	{
		PUSHAD;
		MOV ECX, [ESP + 0x20 + 0xC];
		MOV EDX, [ESP + 0x20 + 0x4];
		CALL OnGamePacketSent;
		TEST EAX, EAX;
		POPAD;
		JNZ OldCode;
		MOV [ESP + 0x4], 0;
OldCode:
		JMP D2NET_SendPacket;
	}
}

VOID NAKED OnMCPPacketReceivedSTUB()
{
	__asm
	{
		LEA ECX, DWORD PTR SS:[ESP + 4];
		PUSHAD;
		CALL OnMCPPacketReceived;
		CMP EAX, 0;
		POPAD;
		JE Block;
		CALL EAX;
Block:
		RET;
	}
}

VOID NAKED OnUseSkillSTUB()
{
	__asm
	{
		PUSHAD;
		MOV ECX, [ESP + 0x28];
		LEA EDX, [ESP + 0x2C];
		CALL OnUseSkill;
		TEST EAX, EAX;
		POPAD;
		JE Block;
		MOV EAX, [D2CLIENT_Attack_I];
		RETN;

Block:
		ADD ESP, 4;
		RETN 8;

		/*PUSH ECX;
		MOV ECX, [ESP + 0x0C];
		CALL OnUseSkill;
		POP ECX;

		CMP EAX, 1;
		JE OldCode;

		CALL D2CLIENT_GetSelectedUnit;
		
		CMP EAX, 0;
		JE OldCode;

		MOV [ESP + 0x0C], 1;

OldCode:
		MOV EAX, [D2CLIENT_Attack_I];
		MOV EAX, [EAX];
		RETN;*/
	}
}

VOID NAKED OutTownSelectSTUB1()
{
	__asm
	{
		PUSH [ESP + 0x14];
		PUSH [ESP + 0x14];
		PUSH [ESP + 0x14];
		PUSH [ESP + 0x14];
		PUSH OutCode;
		SUB ESP, 0x20;
		PUSH EBP;
		XOR EBP, EBP;
		JMP DWORD PTR [ESP + 4 + 0x20 + 0x14];
OutCode:
		MOV [V_PlayerInTown], 0;
		ADD ESP, 4;
		RET 0x10;
	}
}

VOID NAKED OutTownSelectSTUB2()
{
	__asm
	{
		SHR EAX, 0x15;
		AND EAX, 1;
		JNZ Nothing;
		MOV ECX, EBP;
		CALL OutTownSelect;
		CMP AL, 0xFF;
		JZ Quit;
		ADD DWORD PTR [ESP], 0xA2;
		MOV [ESP + 4 + 0x34], EAX;
		POP EAX;
		PUSH ESI;
		PUSH EDI;
		JMP EAX;
Nothing:
		RET;
Quit:
		XOR EAX, EAX;
		RET;
	}
}

VOID NAKED OutTownSelectSTUB3()
{
	__asm
	{
		CMP [V_PlayerInTown], 0;
		JZ OutCode;
		MOV ECX, EBP;
		CALL InfraVision;
		CMP AL, 1;
		RET;
OutCode:
		SHR EAX, 7;
		AND EAX, 1;
		RET;
	}
}

VOID NAKED OverrideShrineSTUB()
{
	__asm
	{
		PUSHAD;
		MOV EDX, ESI;
		CALL OverrideShrine;
		POPAD;
		MOV EDX, D2CLIENT_OverrideShrinePatchOriginal;
		MOV EDX, [4 * EAX + EDX];
		RET;
	}
}

VOID NAKED PassingActBugFixSTUB()
{
	__asm
	{
		TEST EDI, EDI;
		JZ Return;
		REP STOSD;
		MOV ECX, EDX;
		AND ECX, 3;
		REP STOSB;
Return:
		RET;
	}
}

VOID NAKED PlayerAssignmentSTUB()
{
	__asm
	{
		FNOP;
		CALL D2CLIENT_AssignPlayer_I;
		MOV ECX, EAX;
		CALL AssignPlayer;
		RETN;
	}
}

VOID NAKED PlayerBlobSTUB()
{
	__asm
	{
		MOV ECX, ESI;
		CALL PlayerBlobs;
		MOV CL, AL;
		RET;
	}
}

VOID NAKED RefreshGameListSTUB()
{
	__asm
	{
		CMP EAX, 0x5DC;
		RETN;
	}
}

VOID NAKED SaveAutomapLayerToFileSTUB()
{
	__asm
	{
		MOV EAX, [p_D2CLIENT_AutomapLayer];
		MOV EAX, [EAX];
		TEST EAX, EAX;
		JZ Return;
		PUSH EAX;
		MOV EAX, [EAX]AutomapLayer.pObjects;
		PUSH EAX;
		CALL Reveal::FilterOutMyAutomapCells;
		POP EAX;
Return:
		RET;
	}
}

VOID NAKED ShakeSTUB()
{
	__asm
	{
		CMP [V_AntiShake], 0;
		JE OldCode;

		ADD DWORD PTR [ESP], 0xC;
		RET;

OldCode:
		MOV [ECX], ESI;
		MOV ECX, [ESP + 0x1C];
		RET;
	}
}

VOID NAKED SocketProtectionSTUB()
{
	__asm
	{
		MOV EAX, p_D2CLIENT_PlayerUnit;
		MOV EAX, [EAX];
		PUSH 0x13;
		PUSH EAX;
		CALL D2CLIENT_SocketProtectionPatchOriginal;
		RET;
	}
}

VOID NAKED SocketProtectionSTUB1()
{
	__asm
	{
		CMP [V_ProtectItemSocket], 0;
		JZ OutCode;
		CALL SocketProtectionSTUB;
		ADD DWORD PTR [ESP], 0x47;
		RET;
OutCode:
		MOV EAX, [ESP + 4 + 0x18];
		TEST EAX, EAX;
		RET;
	}
}

VOID NAKED SocketProtectionSTUB2()
{
	__asm
	{
		CMP [V_ProtectItemSocket], 0;
		JZ OutCode;
		CALL SocketProtectionSTUB;
		ADD DWORD PTR [ESP], 0x36;
		RET;
OutCode:
		MOV EAX, [ESP + 4 + 0x44];
		TEST EAX, EAX;
		RET;
	}
}

VOID NAKED SocketViewtableSTUB1()
{
	__asm
	{
		POP EAX;
		POP ECX;
		PUSH EAX;
		PUSH ECX;
		JMP SocketViewtable;
	}
}

VOID NAKED SocketViewtableSTUB2()
{
	__asm
	{
		CMP [V_ViewSocketable], 0;
		JZ OutCode;
		POP ESI;
		POP ESI;
		POP EBX;
		RET;
OutCode:
		MOV ECX, 0xF9E;
		RET;
	}
}

VOID NAKED ViewInventorySTUB1()
{
	__asm
	{
		PUSH EAX;
		CALL ViewInventory;
		MOV ESI, EAX;
		POP EAX;
		RET;
	}
}

VOID NAKED ViewInventorySTUB2()
{
	__asm
	{
		PUSH EAX;
		CALL GetViewingUnit;
		MOV EBX, EAX;
		POP EAX;
		RET;
	}
}

VOID NAKED ViewInventorySTUB3()
{
	__asm
	{
		PUSH EAX;
		PUSH EBX;
		CALL GetViewingUnit;

		MOV EBX, [EDI];
		CMP EBX, 1;
		JE OldCode;

		MOV EDI, EAX;

OldCode:
		POP EBX;
		POP EAX;
		TEST EAX, EAX;
		MOV ECX, DWORD PTR [EDI + 0x60];
		RET;
	}
}

VOID NAKED ViewInventorySTUB4()
{
	__asm
	{
		CALL GetViewingUnit;
		TEST EAX, EAX;
		JZ OldCode;
		ADD DWORD PTR [ESP], 0xE1;
		RET 4;

OldCode:
		PUSH [ESP + 4];
		CALL D2COMMON_GetCursorItem;
		RET 4;
	}
}

VOID NAKED ViewStatTUB1()
{
	__asm
	{
		PUSH EAX;
		CALL GetViewingUnit;
		CMP EBX, EAX;
		POP EAX;
		RET;
	}
}

VOID NAKED ViewStatSTUB2()
{
	__asm
	{
		PUSH EAX;
		CALL GetViewingUnit;
		MOV ESI, EAX;
		POP EAX;
		RET;
	}
}

VOID NAKED WeatherSTUB()
{
	__asm
	{
		CMP [V_AntiWeather], 0;
		JE OldCode;
		XOR AL, AL;

OldCode:
		RETN 4;
	}
}

VOID NAKED WindowProcSTUB()
{
	__asm
	{
		PUSHAD;
		PUSH [ESP + 0x20 + 0x14];
		PUSH [ESP + 0x20 + 0x14];
		PUSH [ESP + 0x20 + 0x14];
		PUSH [ESP + 0x20 + 0x14];
		CALL WindowProc;
		TEST EAX, EAX;
		POPAD;
		JNZ OldCode;
		MOV EAX, 0;
		RET;

OldCode:
		MOV EAX, [D2WIN_WindowProc_I];
		RET;
	}
}

VOID NAKED WinDrawSTUB()
{
	__asm
	{
		PUSHAD;
		CALL Design::WinDraw;
		POPAD;
		MOV ESI,DWORD PTR DS:[ESI + 0x3C];
		TEST ESI, ESI;
		RETN;
	}
}