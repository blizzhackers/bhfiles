#include "Hack.h"

Design::Design()
{
	CharacterStats = FALSE;
	Hide = FALSE;
}

Design::~Design()
{
	V_Options = FALSE;
}

BOOL Design::AutomapInfo()
{
	if (!ClientReady(FALSE))
		return FALSE;

	if (V_LevelNames && !Hide)
	{
		if (!V_Reveal->m_LevelExits.IsEmpty())
		{
			for (INT i = 0; i < V_Reveal->m_LevelExits.GetSize(); i++)
			{
				if (V_Reveal->m_LevelExits[i]->dwAct != Me->pAct->dwAct)
					continue;

				TextHook(V_Reveal->m_LevelExits[i]->ptPos.x, V_Reveal->m_LevelExits[i]->ptPos.y,
					V_LevelNamesFontColor, TRUE, 6, 1, V_Reveal->m_LevelExits[i]->szName);
			}
		}
	}

	CHAR Difficulty[50], LevelName[50], GameTimer[50], FPSText[50], SkipText[50], PingText[50], LocalTime[50];
	INT Y = 0, Seconds = (GetTickCount() - V_GameTimer) / 1000;
	LPGAMESTRUCTINFO Info = D2CLIENT_GetGameInfo();
	GetMapName((INT)GetUnitLevel(Me)->dwLevelNo, LevelName, 50);

	if (D2CLIENT_GetDifficulty() == 0)
		sprintf_s(Difficulty, "");

	if (D2CLIENT_GetDifficulty() == 1)
		sprintf_s(Difficulty, "Nightmare");

	if (D2CLIENT_GetDifficulty() == 2)
		sprintf_s(Difficulty, "Hell");

	if (!Hide)
	{
		SYSTEMTIME Time;
		GetLocalTime(&Time);

		if (Time.wHour < 13 || Time.wHour > 23 && strlen(LocalTime) <= 0)
			sprintf_s(LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
		else if (Time.wHour < 24 || Time.wHour > 12 && strlen(LocalTime) <= 0)
			sprintf_s(LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

		sprintf_s(GameTimer, "%.2d:%.2d:%.2d", (Seconds / 3600), ((Seconds / 60) % 60), (Seconds % 60));
		sprintf_s(FPSText, "%d", FPS);
		sprintf_s(SkipText, "%d", Skip);
		sprintf_s(PingText, "%d", Ping);
	}

	AUTOMAP AutomapInfos[] =
	{
		{"%s", " ", 4, 1},
		{(Hide) ? "Game: %s" : V_AutomapInfoGameNameText, Info->szGameName, Hide ? 4 : V_AutomapInfoGameNameColor, Hide ? 1 : V_AutomapInfoGameNameFont},
		{(Hide) ? "Password: %s" : V_AutomapInfoGamePasswordText, Info->szGamePassword, Hide ? 4 : V_AutomapInfoGamePasswordColor, Hide ? 1 : V_AutomapInfoGamePasswordFont},
		{(Hide) ? "%s" : V_AutomapInfoLevelText, LevelName, Hide ? 4 : V_AutomapInfoLevelColor, Hide ? 1 : V_AutomapInfoLevelFont},
		{(Hide) ? "%s" : V_AutomapInfoDifficultyText, (Hide) ? "v 1.13" : Difficulty, Hide ? 4 : V_AutomapInfoDifficultyColor, Hide ? 1 : V_AutomapInfoDifficultyFont},
		{(Hide) ? "Difficulty: %s" : V_AutomapInfoIPText, (Hide) ? Difficulty : Info->szGameServerIp, Hide ? 4 : V_AutomapInfoIPColor, Hide ? 1 : V_AutomapInfoIPFont},
		{(Hide) ? "%s" : V_AutomapInfoLocalTimeText, (Hide) ? "EXPANSION" : LocalTime, Hide ? 4 : V_AutomapInfoLocalTimeColor, Hide ? 1 : V_AutomapInfoLocalTimeFont},
		{V_AutomapInfoGameTimerText, (Hide) ? "" : GameTimer, V_AutomapInfoGameTimerColor, Hide ? 1 : V_AutomapInfoGameTimerFont},
		{V_AutomapInfoFPSText, (Hide) ? "" : FPSText, V_AutomapInfoFPSColor, Hide ? 1 : V_AutomapInfoFPSFont},
		{V_AutomapInfoSkipText, (Hide) ? "" : SkipText, V_AutomapInfoSkipColor, Hide ? 1 : V_AutomapInfoSkipFont},
		{V_AutomapInfoPingText, (Hide) ? "" : PingText, V_AutomapInfoPingColor, Hide ? 1 : V_AutomapInfoPingFont}
	};

	for (INT i = 0; i < ArraySize(AutomapInfos); i++)
	{
		if (strlen(AutomapInfos[i].Data) <= 0)
			continue;

		if (AutomapInfos[i].FontType > 13)
			AutomapInfos[i].FontType = 13;

		if (strlen(AutomapInfos[i].Message) > 0)
		{
			if (i == 0)
				Y += 8;
			else
				Y += 16;

			TextHook(*p_D2CLIENT_ScreenSizeX - 20, Y, AutomapInfos[i].FontColor, 0, AutomapInfos[i].FontType, 0, AutomapInfos[i].Message, AutomapInfos[i].Data);
		}
	}

	return TRUE;
}

VOID Design::BoxHook(INT X, INT Y, INT XSize, INT YSize, DWORD Color, DWORD Transparency, BOOL Automap)
{
	POINT Start = {X, Y};
	POINT End = {XSize, YSize};

	if (Automap)
	{
		if (!p_D2CLIENT_AutomapOn)
			return;

		ScreenToAutomap(&Start, X * 32, Y * 32);
	}

	return D2GFX_DrawRectangle(Start.x, Start.y, End.x + Start.x, End.y + Start.y, Color, Transparency);
}

BOOL Design::ClearScreen(INT Screen)
{
	BOOL Clear = TRUE;

	if((Screen == 0 || Screen == 3) && Clear)
		Clear = (!GetUIVar(UI_PARTY) && !GetUIVar(UI_MERC) && !GetUIVar(UI_CUBE) && !GetUIVar(UI_WPMENU) && !GetUIVar(UI_CHARACTER));

	if((Screen == 1 || Screen == 3) && Clear)
		Clear = (!GetUIVar(UI_INVENTORY) && !GetUIVar(UI_SKILL));

	if((Screen == 2 || Screen == 3) && Clear)
		Clear = (!GetUIVar(UI_NPCSHOP) && !GetUIVar(UI_STASH) && !GetUIVar(UI_MSGS) && !GetUIVar(UI_TRADE) && !GetUIVar(UI_MAINMENU));

	return Clear;
}

VOID Design::DrawBox(INT X1, INT Y1, INT X2, INT Y2, INT LineColor, INT BackGroundColor, INT Transparency)
{
	D2GFX_DrawRectangle(X1, Y1, X2, Y2, BackGroundColor, Transparency);
	D2GFX_DrawLine(X1, Y1, X1 + (X2 - X1), Y1, LineColor, -1);
	D2GFX_DrawLine(X1 + (X2 - X1), Y1, X1 + (X2 - X1), Y1 + (Y2 - Y1), LineColor, -1);
	D2GFX_DrawLine(X1 + (X2 - X1), Y1 + (Y2 - Y1), X1, Y1 + (Y2 - Y1), LineColor, -1);
	D2GFX_DrawLine(X1, Y1 + (Y2 - Y1), X1, Y1, LineColor, -1);
}

VOID Design::DrawCross(INT X, INT Y, DWORD Color, BOOL Automap)
{
	POINT Position = {X, Y};

	if (Automap)
	{
		if (!p_D2CLIENT_AutomapOn)
			return;

		ScreenToAutomap(&Position, X * 32, Y * 32);
	}

	CHAR Lines[][2] = {0, -2, 4, -4, 8, -2, 4, 0, 8, 2, 4, 4, 0, 2, -4, 4, -8, 2, -4, 0, -8, -2, -4, -4, 0, -2};

	for (INT i = 0; i < ArraySize(Lines) - 1; i++)
		D2GFX_DrawLine(Position.x + Lines[i][0], Position.y + Lines[i][1], Position.x + Lines[i + 1][0], Position.y + Lines[i + 1][1], Color, -1);
}

VOID Design::DrawOut(INT X, INT Y, INT Color, INT Size, LPSTR Format, ...)
{
	WCHAR wTemp[0x400] = L"";
	CHAR szTemp[0x400] = "";
	DWORD dwOldSize;
	va_list Args;

	va_start(Args, Format);
	vsprintf_s(szTemp, Format, Args);
	va_end(Args);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szTemp, (INT)strlen(szTemp), wTemp, (INT)strlen(szTemp));

	dwOldSize = D2WIN_SetTextSize(Size);
	D2WIN_DrawText(wTemp, X, Y, Color, -1);
	D2WIN_SetTextSize(dwOldSize);
}

BOOL Design::DrawOutCenter(INT xPos, INT yPos, DWORD dwColor, DWORD dwSize, DWORD dwDiv, LPSTR lpFormat, ...)
{
	CHAR szOutput[0x400] = "";
	WCHAR wOutput[0x400] = L"";
	DWORD dwOldSize, dwWidth;
	va_list Args;

	if (!ClientReady(FALSE))
		return FALSE;

	va_start(Args, lpFormat);
	vsprintf(szOutput, lpFormat, Args);
	va_end(Args);

	MyMultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szOutput, (INT)strlen(szOutput), wOutput, (INT)strlen(szOutput));

	dwOldSize = D2WIN_SetTextSize(dwSize);
	dwWidth = GetTextWidth(szOutput, dwSize);
	D2WIN_DrawText(wOutput, (xPos - (GetTextWidth(szOutput, dwSize) >> dwDiv)), yPos, dwColor, 0);
	D2WIN_SetTextSize(dwOldSize);

	return TRUE;
}

BOOL Design::EscapeInfo()
{
	if (Hide)
		return FALSE;

	if (!V_EscapeInfo || !ClientReady(FALSE))
		return FALSE;

	if (GetUIVar(UI_CHAT) || V_Options || !ClearScreen(3))
		return FALSE;

	Rectangle(V_EscapeInfoX, V_EscapeInfoY, 100, 62, 0, 1);
	TextHook(V_EscapeInfoX + 46, V_EscapeInfoY + 12, V_EscapeInfoTitleColor, 0, 6, 1, "Escape Info");

	if (V_LifeExitPercent > 100)
		V_LifeExitPercent = 100;

	TextHook(V_EscapeInfoX + 46, V_EscapeInfoY + 27, V_EscapeInfoExitLifeColor, 0, 6, 1, V_LifeExitPercent ? "Exit Life: %d%%" : "Exit Life: Off", V_LifeExitPercent);

	if (V_ManaExitPercent > 100)
		V_ManaExitPercent = 100;

	TextHook(V_EscapeInfoX + 46, V_EscapeInfoY + 37, V_EscapeInfoExitManaColor, 0, 6, 1, V_ManaExitPercent ? "Exit Mana: %d%%" : "Exit Mana: Off", V_ManaExitPercent);

	if (V_LifeTownPercent > 100)
		V_LifeTownPercent = 100;

	TextHook(V_EscapeInfoX + 46, V_EscapeInfoY + 47, V_EscapeInfoTownLifeColor, 0, 6, 1, V_LifeTownPercent ? "Town Life: %d%%" : "Town Life: Off", V_LifeTownPercent);

	if (V_ManaTownPercent > 100)
		V_ManaTownPercent = 100;

	TextHook(V_EscapeInfoX + 46, V_EscapeInfoY + 57, V_EscapeInfoTownManaColor, 0, 6, 1, V_ManaTownPercent ? "Town Mana: %d%%" : "Town Mana: Off", V_ManaTownPercent);

	return TRUE;
}

BOOL Design::ExperienceInfo()
{
	if (Hide)
		return FALSE;

	if (!V_ExpInfo || !ClientReady(FALSE))
		return FALSE;

	if (GetUIVar(UI_CHAT) || V_Options || !ClearScreen(3))
		return FALSE;

	INT LevelExperience = GetExp(GetUnitStat(Me, STAT_LEVEL)) - GetExp(GetUnitStat(Me, STAT_LEVEL) - 1);
	INT PlayerExperience = GetUnitStat(Me, STAT_EXP) - GetExp(GetUnitStat(Me, STAT_LEVEL) - 1);

	if (!V_StartingExperience)
		V_StartingExperience = GetUnitStat(Me, STAT_EXP);

	Rectangle(V_ExpInfoX, V_ExpInfoY, 122, 62, 0, 1);
	DrawOutCenter(V_ExpInfoX + 61, V_ExpInfoY + 12, V_ExpInfoTitleColor, 6, 1, "Experience Info");

	DrawOutCenter(V_ExpInfoX + 61, V_ExpInfoY + 27, V_ExpInfoLevelColor, 6, 1, "Level: %d, Exp.: %d%%", GetUnitStat(Me, STAT_LEVEL), CalcPercent(PlayerExperience, LevelExperience));
	DrawOutCenter(V_ExpInfoX + 61, V_ExpInfoY + 37, V_ExpInfoExperienceColor, 6, 1, "Current: %s", AddCommas(GetUnitStat(Me, STAT_EXP), 0));
	DrawOutCenter(V_ExpInfoX + 61, V_ExpInfoY + 47, V_ExpInfoExperienceNeededColor, 6, 1, "Needed: %s", AddCommas(GetExp(GetUnitStat(Me, STAT_LEVEL)) - GetUnitStat(Me, STAT_EXP), 0));
	DrawOutCenter(V_ExpInfoX + 61, V_ExpInfoY + 57, V_ExpInfoExperienceGainedColor, 6, 1, "Gained: %s", AddCommas(GetUnitStat(Me, STAT_EXP) - V_StartingExperience, 0));

	return TRUE;
}

VOID Design::GameDraw()
{
	V_Design->Options();
	V_Design->Stats();
	V_Design->Targets();
}

VOID Design::GameDraw2()
{
	V_Design->Orbs();
	V_Design->MusicPlayer();
	V_Design->States();
}

VOID Design::GameDraw3()
{
	V_Design->EscapeInfo();
	V_Design->ExperienceInfo();
	V_Design->PotionInfo();
	V_Design->Mouse();
	V_Design->Supplies();
}

VOID Design::GameDrawAutomap()
{
	V_Design->AutomapInfo();
	V_Design->Missiles();
	V_Design->Monsters();
	V_Design->PlayerLifePercent();
	V_Design->Vectors();
}

BOOL Design::GameFilter()
{
	if (!V_GameFilter)
		return FALSE;

	if (V_FilterBox && (*p_D2MULTI_GameListControl))
	{
		wstringstream wFilterStream;
		wstring wFilterString = L"Game Filter: ";

		wFilterStream << (INT)(*p_D2MULTI_GameListControl)->dwSelectEnd;
		wFilterString += wFilterStream.str().c_str();
		wFilterString += L"/";
		wFilterStream.str(L"");
		wFilterStream << (INT)V_GameList.size();
		wFilterString += wFilterStream.str().c_str();

		DWORD dwOldSize = D2WIN_SetTextSize(1);
		D2WIN_DrawText(wFilterString.c_str(), 424, 179, 4, -1);
		D2WIN_SetTextSize(dwOldSize);
	}

	return TRUE;
}

VOID Design::GetPlayerPosition(LPPOINT Player)
{
	if (!V_Design)
		return;

	ScreenToAutomapRelative(Player, GetScreenSizeX() / 2, (GetScreenSizeY() - 48) / 2);
}

INT Design::GetScreenSizeX()
{
	if (!V_Design)
		return FALSE;

	INT Sizes[2] = {640, 800};
	return Sizes[D2GFX_GetScreenSize() / 2];
}

INT Design::GetScreenSizeX2()
{
	if (!V_Design)
		return FALSE;

	INT Sizes[2] = {640, 800};
	return Sizes[D2GFX_GetScreenSize2() / 2];
}

INT Design::GetScreenSizeY()
{
	if (!V_Design)
		return FALSE;

	INT Sizes[2] = {480, 600};
	return Sizes[D2GFX_GetScreenSize() / 2];
}

INT Design::GetScreenSizeY2()
{
	if (!V_Design)
		return FALSE;

	INT Sizes[2] = {480, 600};
	return Sizes[D2GFX_GetScreenSize2() / 2];
}

VOID Design::MapToScreenCoords(LPPOINT Position)
{
	D2COMMON_MapToAbsScreen(&Position->x, &Position->y);
	Position->x -= D2CLIENT_GetPlayerXOffset();
	Position->y -= D2CLIENT_GetPlayerYOffset();
}

BOOL Design::Missiles()
{
	if (!V_Design)
		return FALSE;

	if (Hide)
		return FALSE;

	if (!V_Missiles || !ClientReady(FALSE))
		return FALSE;

	for (LPROOM1 Room = Me->pAct->pRoom1; Room; Room = Room->pRoomNext)
	{
		for (LPUNITANY Unit = Room->pUnitFirst; Unit; Unit = Unit->pListNext)
		{
			if (Unit && Unit->dwType == UNIT_TYPE_MISSILE)
			{
				INT Color = 0;

				if (!Unit->dwOwnerId)
					continue;

				if (Unit->dwOwnerId == Me->dwUnitId)
					Color = V_MyColor;

				if (!Color && Unit->dwOwnerType == UNIT_TYPE_PLAYER)
				{
					DWORD Flags = GetPvPFlags(Unit->dwOwnerId);

					if (Flags & PVP_HOSTILED_YOU || Flags & PVP_HOSTILED_BY_YOU)
						Color = V_HostileColor;

					else if (Flags & PVP_ALLIED_WITH_YOU)
						Color = V_PartyColor;

					else
						Color = V_NeutralColor;
				}

				if (!Color && Unit->dwOwnerType == UNIT_TYPE_NPC)
				{
					DWORD Flags = GetPvPFlags(D2CLIENT_GetMonsterOwner(Unit->dwUnitId));

					if (Flags & PVP_HOSTILED_YOU || Flags & PVP_HOSTILED_BY_YOU)
						Color = V_HostileColor;

					else if (Flags & PVP_ALLIED_WITH_YOU)
						Color = V_PartyColor;

					else if (Flags & PVP_ALLIED)
						Color = V_NeutralColor;

					else
						Color = V_HostileColor;
				}

				if (Color)
					BoxHook(Unit->pPath->xPos - 3, Unit->pPath->yPos - 3, 2, 2, Color, 255, TRUE);
			}
		}
	}

	return TRUE;
}

BOOL Design::Monsters()
{
	if (!V_Design)
		return FALSE;

	if (Hide)
		return FALSE;

	if (!V_Monsters || !ClientReady(FALSE))
		return FALSE;

	for (LPROOM1 Room = Me->pAct->pRoom1; Room; Room = Room->pRoomNext)
	{
		for (LPUNITANY Unit = Room->pUnitFirst; Unit; Unit = Unit->pListNext)
		{
			if (!ValidMonsters(Unit))
				continue;

			if (Unit && Unit->dwType == UNIT_TYPE_NPC)
			{
				INT Color = -1;

				if (Unit->dwOwnerId == Me->dwUnitId)
					continue;

				if (Unit->pMonsterData->fBoss & 1 && !Unit->pMonsterData->fChamp && Color == -1)
					Color = V_BossColor;

				else if (Unit->pMonsterData->fChamp & 1)
					Color = V_ChampionColor;

				else if (Unit->pMonsterData->fMinion & 1)
					Color = V_MinionColor;

				else
					Color = V_NormalColor;

				if (Color)
				{
					DrawCross(Unit->pPath->xPos, Unit->pPath->yPos, Color, 1);

					if (V_MonsterResists)
					{
						CHAR Text[50] = "";

						if ((INT)GetUnitStat(Unit, STAT_DAMAGEREDUCTION) >= 100)
							strcat_s(Text, "ÿc4i");

						if ((INT)GetUnitStat(Unit, STAT_COLDRESIST) >= 100)
							strcat_s(Text, "ÿc3i");

						if ((INT)GetUnitStat(Unit, STAT_FIRERESIST) >= 100)
							strcat_s(Text, "ÿc1i");

						if ((INT)GetUnitStat(Unit, STAT_LIGHTNINGRESIST) >= 100)
							strcat_s(Text, "ÿc9i");

						if ((INT)GetUnitStat(Unit, STAT_POISONRESIST) >= 100)
							strcat_s(Text, "ÿc2i");

						if ((INT)GetUnitStat(Unit, STAT_MAXMAGICRESIST) >= 100)
							strcat_s(Text, "ÿc8i");

						if (strlen(Text) > 0)
							TextHook(Unit->pPath->xPos - 5, Unit->pPath->yPos, 0, 1, 6, 1, Text);
					}

					if (V_MonsterNames && Unit->pMonsterData->fBoss & 1 && !Unit->pMonsterData->fChamp & 1)
					{
						WCHAR * MonsterName = (WCHAR *)GetUnitName(Unit);
						CHAR Name[50];

						WideCharToMultiByte(CP_ACP, 0, MonsterName, -1, Name, (INT)sizeof(Name), 0, 0);
						TextHook(Unit->pPath->xPos - 8, Unit->pPath->yPos - 8, V_BossNameColor, 1, 6, 1, Name);
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL Design::Mouse()
{
	if (!ClientReady(FALSE) || !V_Mouse)
		return FALSE;

	Rectangle(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY, 40, 50, 0, 1);
	DrawOut(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY + 35, 0, 6, "X: %d", *p_D2CLIENT_MouseX);
	DrawOut(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY + 45, 0, 6, "Y: %d", *p_D2CLIENT_MouseY);

	return TRUE;
}

BOOL Design::MusicPlayer()
{
	if (!V_Design)
		return FALSE;

	if (Hide)
		return FALSE;

	if (!V_MusicPlayer || !ClientReady(FALSE))
		return FALSE;

	if (GetUIVar(UI_CHAT) || V_Options || !ClearScreen(3))
		return FALSE;

	BOOL Extend = (strlen(CurrentSong()) > 6) ? TRUE : FALSE;
	INT SongWidth = GetTextWidth(CurrentSong(), 6);

	Rectangle(V_MusicPlayerX, V_MusicPlayerY, Extend ? SongWidth + 10 : 50, strlen(CurrentSong()) > 0 ? 42 : 22, 0, 1);

	DrawOutCenter(Extend ? V_MusicPlayerX + 5 + (SongWidth / 2) : V_MusicPlayerX + 25, strlen(CurrentSong()) > 0 ? V_MusicPlayerY + 12 :
		V_MusicPlayerY + 15, V_MusicPlayerTitleColor, 6, 1, "Winamp");

	if (strlen(CurrentSong()) > 0)
	{
		DrawOutCenter(Extend ? V_MusicPlayerX + 5 + (SongWidth / 2) : V_MusicPlayerX + 25, V_MusicPlayerY + 27, V_MusicPlayerSongColor, 6, 1, CurrentSong());

		if (MaxTime() != INFINITE)
			DrawOutCenter(Extend ? V_MusicPlayerX + 5 + (SongWidth / 2) : V_MusicPlayerX + 25, V_MusicPlayerY + 37, V_MusicPlayerTimeColor, 6, 1, "%.2d:%.2d:%.2d / %.2d:%.2d:%.2d",
			(CurrentTime() / 3600000) % 60, (CurrentTime() / 60000) % 60, (CurrentTime() / 1000) % 60, (MaxTime() / 3600) % 60, (MaxTime() / 60) % 60, MaxTime() % 60);
		else
			DrawOutCenter(Extend ? V_MusicPlayerX + 5 + (SongWidth / 2) : V_MusicPlayerX + 25, V_MusicPlayerY + 37, V_MusicPlayerTimeColor, 6, 1, "%.2d:%.2d:%.2d / --:--:--",
			(CurrentTime() / 3600000) % 60, (CurrentTime() / 60000) % 60, (CurrentTime() / 1000) % 60);
	}

	return TRUE;
}

BOOL Design::Options()
{
	if (!ClientReady(FALSE) || !V_Options)
		return FALSE;

	if (GetUIVar(UI_MSGS) || GetUIVar(UI_CHARACTER) || GetUIVar(UI_SKILL) || GetUIVar(UI_INVENTORY) || GetUIVar(UI_MAINMENU))
		if (V_Options)
			V_Options = FALSE;

	Rectangle(25, 10, 750, 495, 0, 1);
	Rectangle(25, 30, 750, 0, 0, 1);
	Rectangle(25, 480, 750, 0, 0, 1);

	DrawOutCenter(400, 26, V_OptionTitleColor, V_OptionTitleFont, 1, "Options");
	DrawOutCenter(400, 498, V_OptionCloseColor, V_OptionCloseFont, 1, "Close");

	if (V_LifeTownPercent > 0)
		DrawOut(26, 45, V_OptionVariableColor, V_OptionVariableFont, "Town Life: %d%%", V_LifeTownPercent);
	else
		DrawOut(26, 45, V_OptionVariableColor, V_OptionVariableFont, "Town Life: Off");

	if (V_LifeExitPercent > 0)
		DrawOut(26, 60, V_OptionVariableColor, V_OptionVariableFont, "Exit Life: %d%%", V_LifeExitPercent);
	else
		DrawOut(26, 60, V_OptionVariableColor, V_OptionVariableFont, "Exit Life: Off");

	if (V_ManaTownPercent > 0)
		DrawOut(26, 75, V_OptionVariableColor, V_OptionVariableFont, "Town Mana: %d%%", V_ManaTownPercent);
	else
		DrawOut(26, 75, V_OptionVariableColor, V_OptionVariableFont, "Town Mana: Off");

	if (V_ManaExitPercent > 0)
		DrawOut(26, 90, V_OptionVariableColor, V_OptionVariableFont, "Exit Mana: %d%%", V_ManaExitPercent);
	else
		DrawOut(26, 90, V_OptionVariableColor, V_OptionVariableFont, "Exit Mana: Off");

	if (V_LifePotPercent > 0)
		DrawOut(26, 125, V_OptionVariableColor, V_OptionVariableFont, "Life Potion: %d%%", V_LifePotPercent);
	else
		DrawOut(26, 125, V_OptionVariableColor, V_OptionVariableFont, "Life Potion: Off");

	if (V_ManaPotPercent > 0)
		DrawOut(26, 140, V_OptionVariableColor, V_OptionVariableFont, "Mana Potion: %d%%", V_ManaPotPercent);
	else
		DrawOut(26, 140, V_OptionVariableColor, V_OptionVariableFont, "Mana Potion: Off");

	if (V_LifeJuvPercent > 0)
		DrawOut(26, 155, V_OptionVariableColor, V_OptionVariableFont, "Life Rejuvenation: %d%%", V_LifeJuvPercent);
	else
		DrawOut(26, 155, V_OptionVariableColor, V_OptionVariableFont, "Life Rejuvenation: Off");

	if (V_ManaJuvPercent > 0)
		DrawOut(26, 170, V_OptionVariableColor, V_OptionVariableFont, "Mana Rejuvenation: %d%%", V_ManaJuvPercent);
	else
		DrawOut(26, 170, V_OptionVariableColor, V_OptionVariableFont, "Mana Rejuvenation: Off");

	if (V_Aim > 0)
		DrawOut(26, 205, V_OptionVariableColor, V_OptionVariableFont, "Aim: On");
	else
		DrawOut(26, 205, V_OptionVariableColor, V_OptionVariableFont, "Aim: Off");

	if (V_BlockFakes > 0)
		DrawOut(26, 220, V_OptionVariableColor, V_OptionVariableFont, "Block Fakes: On");
	else
		DrawOut(26, 220, V_OptionVariableColor, V_OptionVariableFont, "Block Fakes: Off");

	if (V_Blind >= 1 && V_Blind <= 4)
		DrawOut(26, 235, V_OptionVariableColor, V_OptionVariableFont, "Aim Type: Blind %d", V_Blind);
	else if (V_Blind == 0)
		DrawOut(26, 235, V_OptionVariableColor, V_OptionVariableFont, "Aim Type: Normal");
	else if (V_Blind == 5)
		DrawOut(26, 235, V_OptionVariableColor, V_OptionVariableFont, "Aim Type: Click");

	if (V_Screenshot)
		DrawOut(26, 270, V_OptionVariableColor, V_OptionVariableFont, "Clean Screenshots");
	else
		DrawOut(26, 270, V_OptionVariableColor, V_OptionVariableFont, "Dirty Screenshots");

	if (V_Orbs)
		DrawOut(26, 285, V_OptionVariableColor, V_OptionVariableFont, "Orbs: On");
	else
		DrawOut(26, 285, V_OptionVariableColor, V_OptionVariableFont, "Orbs: Off");

	if (V_Mouse)
		DrawOut(26, 300, V_OptionVariableColor, V_OptionVariableFont, "Mouse: On");
	else
		DrawOut(26, 300, V_OptionVariableColor, V_OptionVariableFont, "Mouse: Off");

	if (V_PlayerLifePercent)
		DrawOut(26, 315, V_OptionVariableColor, V_OptionVariableFont, "Life %%: On");
	else
		DrawOut(26, 315, V_OptionVariableColor, V_OptionVariableFont, "Life %%: Off");

	if (V_PlayerLifeBar)
		DrawOut(26, 330, V_OptionVariableColor, V_OptionVariableFont, "Life Bar: On");
	else
		DrawOut(26, 330, V_OptionVariableColor, V_OptionVariableFont, "Life Bar: Off");

	if (V_Vectors)
		DrawOut(26, 345, V_OptionVariableColor, V_OptionVariableFont, "Vectors: On");
	else
		DrawOut(26, 345, V_OptionVariableColor, V_OptionVariableFont, "Vectors: Off");

	if (V_ExpInfo)
		DrawOut(26, 360, V_OptionVariableColor, V_OptionVariableFont, "Experience Info: On");
	else
		DrawOut(26, 360, V_OptionVariableColor, V_OptionVariableFont, "Experience Info: Off");

	if (V_StateList)
		DrawOut(26, 375, V_OptionVariableColor, V_OptionVariableFont, "State List: On");
	else
		DrawOut(26, 375, V_OptionVariableColor, V_OptionVariableFont, "State List: Off");

	if (V_Supplies)
		DrawOut(26, 390, V_OptionVariableColor, V_OptionVariableFont, "Supplies: On");
	else
		DrawOut(26, 390, V_OptionVariableColor, V_OptionVariableFont, "Supplies: Off");

	if (V_MusicPlayer)
		DrawOut(26, 405, V_OptionVariableColor, V_OptionVariableFont, "Music Player: On");
	else
		DrawOut(26, 405, V_OptionVariableColor, V_OptionVariableFont, "Music Player: Off");

	if (V_TargetInfo)
		DrawOut(26, 420, V_OptionVariableColor, V_OptionVariableFont, "Target Info: On");
	else
		DrawOut(26, 420, V_OptionVariableColor, V_OptionVariableFont, "Target Info: Off");

	if (V_TargetList)
		DrawOut(26, 435, V_OptionVariableColor, V_OptionVariableFont, "Target List: On");
	else
		DrawOut(26, 435, V_OptionVariableColor, V_OptionVariableFont, "Target List: Off");

	if (V_EscapeInfo)
		DrawOut(26, 450, V_OptionVariableColor, V_OptionVariableFont, "Escape Info: On");
	else
		DrawOut(26, 450, V_OptionVariableColor, V_OptionVariableFont, "Escape Info: Off");

	if (V_PotionInfo)
		DrawOut(26, 465, V_OptionVariableColor, V_OptionVariableFont, "Potion Info: On");
	else
		DrawOut(26, 465, V_OptionVariableColor, V_OptionVariableFont, "Potion Info: Off");

	if (V_DeathMessage)
		DrawOutCenter(400, 45, V_OptionVariableColor, V_OptionVariableFont, 1, "Death Message: On");
	else
		DrawOutCenter(400, 45, V_OptionVariableColor, V_OptionVariableFont, 1, "Death Message: Off");

	DrawOutCenter(400, 60, V_OptionVariableColor, V_OptionVariableFont, 1, "Death Message Delay: %d", V_DeathMessageDelay);

	if (V_Enchant)
		DrawOutCenter(400, 125, V_OptionVariableColor, V_OptionVariableFont, 1, "Enchant Bot: On");
	else
		DrawOutCenter(400, 125, V_OptionVariableColor, V_OptionVariableFont, 1, "Enchant Bot: Off");

	if (V_EnchantNote)
		DrawOutCenter(400, 140, V_OptionVariableColor, V_OptionVariableFont, 1, "Enchant Note: On");
	else
		DrawOutCenter(400, 140, V_OptionVariableColor, V_OptionVariableFont, 1, "Enchant Note: Off");

	if (V_AntiFlash)
		DrawOutCenter(400, 205, V_OptionVariableColor, V_OptionVariableFont, 1, "Anti Flash: On");
	else
		DrawOutCenter(400, 205, V_OptionVariableColor, V_OptionVariableFont, 1, "Anti Flash: Off");

	DrawOutCenter(400, 220, V_OptionVariableColor, V_OptionVariableFont, 1, "Flash Delay: %d", V_FlashDelay);

	if (V_AntiQuestBox)
		DrawOutCenter(400, 270, V_OptionVariableColor, V_OptionVariableFont, 1, "Anti Quest Box: On");
	else
		DrawOutCenter(400, 270, V_OptionVariableColor, V_OptionVariableFont, 1, "Anti Quest Box: Off");

	if (V_CopyPaste)
		DrawOutCenter(400, 285, V_OptionVariableColor, V_OptionVariableFont, 1, "Copy & Paste: On");
	else
		DrawOutCenter(400, 285, V_OptionVariableColor, V_OptionVariableFont, 1, "Copy & Paste: Off");

	if (V_KillLog)
		DrawOutCenter(400, 300, V_OptionVariableColor, V_OptionVariableFont, 1, "Kill Log: On");
	else
		DrawOutCenter(400, 300, V_OptionVariableColor, V_OptionVariableFont, 1, "Kill Log: Off");

	if (V_ModifiedJoinMessage)
		DrawOutCenter(400, 315, V_OptionVariableColor, V_OptionVariableFont, 1, "Modified Join Message: On");
	else
		DrawOutCenter(400, 315, V_OptionVariableColor, V_OptionVariableFont, 1, "Modified Join Message: Off");

	if (V_OutTownSelect)
		DrawOutCenter(400, 330, V_OptionVariableColor, V_OptionVariableFont, 1, "Select Non-Hostile Players Out Of Town: On");
	else
		DrawOutCenter(400, 330, V_OptionVariableColor, V_OptionVariableFont, 1, "Select Non-Hostile Players Out Of Town: Off");

	if (V_AutomapOnJoin)
		DrawOutCenter(400, 345, V_OptionVariableColor, V_OptionVariableFont, 1, "Start Game With Automap: On");
	else
		DrawOutCenter(400, 345, V_OptionVariableColor, V_OptionVariableFont, 1, "Start Game With Automap: Off");

	if (V_RememberGameName)
		DrawOutCenter(400, 360, V_OptionVariableColor, V_OptionVariableFont, 1, "Remember Game Name: On");
	else
		DrawOutCenter(400, 360, V_OptionVariableColor, V_OptionVariableFont, 1, "Remember Game Name: Off");

	if (V_RememberGamePassword)
		DrawOutCenter(400, 375, V_OptionVariableColor, V_OptionVariableFont, 1, "Remember Game Password: On");
	else
		DrawOutCenter(400, 375, V_OptionVariableColor, V_OptionVariableFont, 1, "Remember Game Password: Off");

	DrawOutCenter(400, 410, V_OptionVariableColor, V_OptionVariableFont, 1, "Play");
	DrawOutCenter(400, 425, V_OptionVariableColor, V_OptionVariableFont, 1, "Pause");
	DrawOutCenter(400, 440, V_OptionVariableColor, V_OptionVariableFont, 1, "Stop");
	DrawOutCenter(400, 455, V_OptionVariableColor, V_OptionVariableFont, 1, "Next");
	DrawOutCenter(400, 470, V_OptionVariableColor, V_OptionVariableFont, 1, "Previous");

	if (V_Potion)
		TextHook(768, 45, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Potion Notifications: On");
	else
		TextHook(768, 45, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Potion Notifications: Off");

	if (V_Quests)
		TextHook(768, 60, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Quest Notifications: On");
	else
		TextHook(768, 60, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Quest Notifications: Off");

	if (V_LifePot)
		TextHook(768, 75, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Life Potion Notifications: On");
	else
		TextHook(768, 75, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Life Potion Notifications: Off");

	if (V_ManaPot)
		TextHook(768, 90, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Mana Potion Notifications: On");
	else
		TextHook(768, 90, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Mana Potion Notifications: Off");

	if (V_LifeJuv)
		TextHook(768, 105, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Life Rejuv. Notifications: On");
	else
		TextHook(768, 105, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Life Rejuv. Notifications: Off");

	if (V_ManaJuv)
		TextHook(768, 120, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Mana Rejuv. Notifications: On");
	else
		TextHook(768, 120, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Mana Rejuv. Notifications: Off");

	if (V_AntiShake)
		TextHook(768, 155, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Anti Shake: On");
	else
		TextHook(768, 155, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Anti Shake: Off");

	if (V_AntiWeather)
		TextHook(768, 170, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Anti Weather: On");
	else
		TextHook(768, 170, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Anti Weather: Off");

	if (V_AutoRevealAutomap)
		TextHook(768, 185, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Automatically Reveal Act: On");
	else
		TextHook(768, 185, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Automatically Reveal Act: Off");

	if (V_Ethereal)
		TextHook(768, 200, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Ethereal Items: On");
	else
		TextHook(768, 200, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Ethereal Items: Off");

	if (V_FullLightRadius)
		TextHook(768, 215, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Full Light Radius: On");
	else
		TextHook(768, 215, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Full Light Radius: Off");

	if (V_ItemLevel)
		TextHook(768, 230, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Item Level: On");
	else
		TextHook(768, 230, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Item Level: Off");

	if (V_ProtectItemSocket)
		TextHook(768, 245, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Item Socket Protection: On");
	else
		TextHook(768, 245, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Item Socket Protection: Off");

	if (V_Sockets)
		TextHook(768, 260, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Item Sockets: On");
	else
		TextHook(768, 260, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Item Sockets: Off");

	if (V_LevelNames)
		TextHook(768, 275, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Level Names: On");
	else
		TextHook(768, 275, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Level Names: Off");

	if (V_ShrineImages)
		TextHook(768, 290, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Shrine Images: On");
	else
		TextHook(768, 290, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Shrine Images: Off");

	if (V_ViewItemBasicStats)
		TextHook(768, 305, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Basic Stats Of Items: On");
	else
		TextHook(768, 305, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Basic Stats Of Items: Off");

	if (V_Missiles)
		TextHook(768, 320, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Missiles: On");
	else
		TextHook(768, 320, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Missiles: Off");

	if (V_Monsters)
		TextHook(768, 335, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Monsters: On");
	else
		TextHook(768, 335, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Monsters: Off");

	if (V_MonsterResists)
		TextHook(768, 350, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Monster Resists: On");
	else
		TextHook(768, 350, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Monster Resists: Off");

	if (V_MonsterNames)
		TextHook(768, 365, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Boss Monster Names: On");
	else
		TextHook(768, 365, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Boss Monster Names: Off");

	if (V_SpeedAttack)
		TextHook(768, 400, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Attack Speed: On");
	else
		TextHook(768, 400, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Attack Speed: Off");

	if (V_SpeedCast)
		TextHook(768, 415, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Cast Rate: On");
	else
		TextHook(768, 415, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Cast Rate: Off");

	if (V_FastTeleport)
		TextHook(768, 450, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Fast Teleport: On");
	else
		TextHook(768, 450, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Fast Teleport: Off");

	if (V_FarcastTeleport)
		TextHook(768, 465, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Farcast Teleport: On");
	else
		TextHook(768, 465, V_OptionVariableColor, 0, V_OptionVariableFont, 0, "Farcast Teleport: Off");

	return TRUE;
}

BOOL Design::Orbs()
{
	if (!ClientReady(FALSE))
		return FALSE;

	LONG LifePercent = CalcPercent((GetUnitStat(Me, STAT_HP) >> 8), (GetUnitStat(Me, STAT_MAXHP) >> 8));
	LONG ManaPercent = CalcPercent((GetUnitStat(Me, STAT_MANA) >> 8), (GetUnitStat(Me, STAT_MAXMANA) >> 8));

	if (Hide || !V_Orbs)
		return FALSE;

	if (!V_Orbs)
		return FALSE;

	Rectangle(49, *p_D2CLIENT_ScreenSizeY - 65, 42, 18, 0, 4);
	Rectangle(*p_D2CLIENT_ScreenSizeX - 92, *p_D2CLIENT_ScreenSizeY - 65, 42, 18, 0, 4);

	TextHook(67, *p_D2CLIENT_ScreenSizeY - 50, V_LifeOrbColor, 0, V_LifeOrbFont, 1, "%d%%", LifePercent);
	TextHook(*p_D2CLIENT_ScreenSizeX - 74, *p_D2CLIENT_ScreenSizeY - 50, V_ManaOrbColor, 0, V_ManaOrbFont, 1, "%d%%", ManaPercent);

	return TRUE;
}

BOOL Design::PlayerLifePercent()
{
	if (Hide)
		return FALSE;

	if (!ClientReady(FALSE))
		return FALSE;

	if (V_Players.IsEmpty())
		return FALSE;

	V_Players.Lock();

	for (INT i = 0; i < V_Players.GetSize(); i++)
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(V_Players[i]->UnitId, UNIT_TYPE_PLAYER);

		if (Unit && ClearScreen(3) && !GetUIVar(UI_CUBE))
		{
			INT Color = 2, LifeBar = (INT)V_Players[i]->Life / 5, LifeBarColor = 0x7A;

			if ((INT)V_Players[i]->Life > 100)
				V_Players[i]->Life = 100;

			if (V_Players[i]->Life < 50 && V_Players[i]->Life >= 25)
			{
				Color = 8;
				LifeBarColor = 0x60;
			}

			if (V_Players[i]->Life < 25)
			{
				Color = 1;
				LifeBarColor = 0x62;
			}

			if (Color && V_PlayerLifePercent)
				TextHook(Unit->pPath->xPos + 8, Unit->pPath->yPos + 8, Color, 1, 6, 1, "%d%%", V_Players[i]->Life);

			if (LifeBarColor && V_PlayerLifeBar)
			{
				if (V_PlayerLifePercent)
				{
					BoxHook(Unit->pPath->xPos + 6, Unit->pPath->yPos + 11, 20, 3, 0x00, 255, TRUE);
					BoxHook(Unit->pPath->xPos + 6, Unit->pPath->yPos + 11, LifeBar, 3, LifeBarColor, 255, TRUE);
				}

				else
				{
					BoxHook(Unit->pPath->xPos + 1, Unit->pPath->yPos + 7, 20, 3, 0x00, 255, TRUE);
					BoxHook(Unit->pPath->xPos + 1, Unit->pPath->yPos + 7, LifeBar, 3, LifeBarColor, 255, TRUE);
				}
			}
		}
	}

	V_Players.Unlock();
	return TRUE;
}

BOOL Design::PotionInfo()
{
	if (Hide)
		return FALSE;

	if (!V_PotionInfo || !ClientReady(FALSE))
		return FALSE;

	if (GetUIVar(UI_CHAT) || V_Options || !ClearScreen(3))
		return FALSE;

	Rectangle(V_PotionInfoX, V_PotionInfoY, 106, 62, 0, 1);
	TextHook(V_PotionInfoX + 50, V_PotionInfoY + 12, V_PotionInfoTitleColor, 0, 6, 1, "Potion Info");

	if (V_LifePotPercent > 100)
		V_LifePotPercent = 100;

	TextHook(V_PotionInfoX + 50, V_PotionInfoY + 27, V_PotionInfoLifePotColor, 0, 6, 1, V_LifePotPercent ? "Life Potion: %d%%" : "Life Potion: Off", V_LifePotPercent);

	if (V_ManaPotPercent > 100)
		V_ManaPotPercent = 100;

	TextHook(V_PotionInfoX + 50, V_PotionInfoY + 37, V_PotionInfoManaPotColor, 0, 6, 1, V_ManaPotPercent ? "Mana Potion: %d%%" : "Mana Potion: Off", V_ManaPotPercent);

	if (V_LifeJuvPercent > 100)
		V_LifeJuvPercent = 100;

	TextHook(V_PotionInfoX + 50, V_PotionInfoY + 47, V_PotionInfoLifeJuvColor, 0, 6, 1, V_LifeJuvPercent ? "Life Rejuv.: %d%%" : "Life Rejuv.: Off", V_LifeJuvPercent);

	if (V_ManaJuvPercent > 100)
		V_ManaJuvPercent = 100;

	TextHook(V_PotionInfoX + 50, V_PotionInfoY + 57, V_PotionInfoManaJuvColor, 0, 6, 1, V_ManaJuvPercent ? "Mana Rejuv.: %d%%" : "Mana Rejuv.: Off", V_ManaJuvPercent);

	return TRUE;
}

VOID Design::Rectangle(INT X, INT Y, INT X2, INT Y2, INT Color, INT Transparency)
{
	RECT RECTANGLE;
	RECTANGLE.left = X;
	RECTANGLE.top = Y;
	RECTANGLE.right = X2 + X;
	RECTANGLE.bottom = Y2 + Y;
	BoxHook(X, Y, X2, Y2, Color, Transparency, FALSE);
	DrawRectangleFrameSTUB(&RECTANGLE);
}

VOID Design::ScreenToAutomap(LPPOINT Position, INT X, INT Y)
{
	Position->x = ((X - Y) / 2 / (*(INT*)p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).x + 8;
	Position->y = ((X + Y) / 4 / (*(INT*)p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).y - 8;

	if (D2CLIENT_GetAutomapSize())
	{
		--Position->x;
		Position->y += 5;
	}
}

VOID Design::ScreenToAutoMap(LPPOINT Point)
{
	POINT Mouse = {(*p_D2CLIENT_MouseX), (*p_D2CLIENT_MouseY)};
	POINT MouseOffset = {(*p_D2CLIENT_MouseOffsetX), (*p_D2CLIENT_MouseOffsetY)};

	Mouse.x += MouseOffset.x;
	Mouse.y += MouseOffset.y;

	D2COMMON_AbsScreenToMap(&Mouse.x, &Mouse.y);

	Point->x = Mouse.x;
	Point->y = Mouse.y;

	/*LONG MX = (*p_D2CLIENT_MouseX);
	LONG MY = (*p_D2CLIENT_MouseY);
	DWORD New = MX;

	LONG MZ = (*p_D2CLIENT_MouseOffsetX);
	MY += (*p_D2CLIENT_MouseOffsetY);
	MZ -= (*p_D2CLIENT_MouseOffsetZ);
	MZ += New;

	D2COMMON_AbsScreenToMap(&MZ, &MY);

	Point->x = MZ;
	Point->y = MY;*/
}

VOID Design::ScreenToAutomapRelative(LPPOINT ptPos, INT x, INT y)
{
	x += D2CLIENT_GetPlayerXOffset();
	y += D2CLIENT_GetPlayerYOffset();
	ScreenToAutomap(ptPos, y * 2 + x, y * 2 - x);

	if (!ClearScreen(1))
		ptPos->x += 8;
}

BOOL Design::States()
{
	if (Hide)
		return FALSE;

	if (!V_StateList || !ClientReady(FALSE))
		return FALSE;

	if (GetUIVar(UI_CHAT) || V_Options || !ClearScreen(3))
		return FALSE;

	STATE States [] =
	{
		{"Amplify Damage: %d", AFFECT_AMPLIFYDAMAGE, (GetTickCount() - V_AmplifyDamageTimer) / 1000},
		{"Battle Command: %d", AFFECT_BATTLECOMMAND, (GetTickCount() - V_BattleCommandTimer) / 1000},
		{"Battle Cry: %d", AFFECT_BATTLECRY, (GetTickCount() - V_BattleCryTimer) / 1000},
		{"Battle Orders: %d", AFFECT_BATTLEORDERS, (GetTickCount() - V_BattleOrdersTimer) / 1000},
		{"Bone Armor: %d", AFFECT_BONEARMOR, (GetTickCount() - V_BoneArmorTimer) / 1000},
		{"Burst of Speed: %d", AFFECT_BURSTOFSPEED, (GetTickCount() - V_BurstOfSpeedTimer) / 1000},
		{"Chilling Armor: %d", AFFECT_CHILLINGARMOR, (GetTickCount() - V_ChillingArmorTimer) / 1000},
		{"Cold: %d", AFFECT_COLD, (GetTickCount() - V_ColdTimer) / 1000},
		{"Convicted: %d", AFFECT_CONVICTED, (GetTickCount() - V_ConvictedTimer) / 1000},
		{"Cyclone Armor: %d", AFFECT_CYCLONEARMOR, (GetTickCount() - V_CycloneArmorTimer) / 1000},
		{"Decrepify: %d", AFFECT_DECREPIFY, (GetTickCount() - V_DecrepifyTimer) / 1000},
		{"Enchant: %d", AFFECT_ENCHANT, (GetTickCount() - V_EnchantTimer) / 1000},
		{"Energy Shield: %d", AFFECT_ENERGYSHIELD, (GetTickCount() - V_EnergyShieldTimer) / 1000},
		{"Fade: %d", AFFECT_FADE, (GetTickCount() - V_FadeTimer) / 1000},
		{"Frozen: %d", AFFECT_FREEZE, (GetTickCount() - V_FrozenTimer) / 1000},
		{"Frozen Armor: %d", AFFECT_FROZENARMOR, (GetTickCount() - V_FrozenArmorTimer) / 1000},
		{"Holy Frozen: %d", AFFECT_HOLYFREEZECHILL, (GetTickCount() - V_HolyFrozenTimer) / 1000},
		{"Holy Shield: %d", AFFECT_HOLYSHIELD, (GetTickCount() - V_HolyShieldTimer) / 1000},
		{"Inner Sight: %d", AFFECT_INNERSIGHT, (GetTickCount() - V_InnerSightTimer) / 1000},
		{"Iron Maiden: %d", AFFECT_IRONMAIDEN, (GetTickCount() - V_IronMaidenTimer) / 1000},
		{"Life Tap: %d", AFFECT_LIFETAP, (GetTickCount() - V_LifeTapTimer) / 1000},
		{"Lower Resist: %d", AFFECT_LOWERRESIST, (GetTickCount() - V_LowerResistTimer) / 1000},
		{"Oak Sage: %d", AFFECT_OAKSAGE, (GetTickCount() - V_OakSageTimer) / 1000},
		{"Open Wounds: %d", AFFECT_OPENWOUNDS, (GetTickCount() - V_OpenWoundsTimer) / 1000},
		{"Poisoned: %d", AFFECT_POISON, (GetTickCount() - V_PoisonedTimer) / 1000},
		{"Shiver Armor: %d", AFFECT_SHIVERARMOR, (GetTickCount() - V_ShiverArmorTimer) / 1000},
		{"Shout: %d", AFFECT_SHOUT, (GetTickCount() - V_ShoutTimer) / 1000},
		{"Slowed: %d", AFFECT_SLOWED, (GetTickCount() - V_SlowedTimer) / 1000},
		{"Terror: %d", AFFECT_TERROR, (GetTickCount() - V_TerrorTimer) / 1000},
		{"Thunder Storm: %d", AFFECT_THUNDERSTORM, (GetTickCount() - V_ThunderStormTimer) / 1000},
		{"Venom: %d", AFFECT_VENOMCLAWS, (GetTickCount() - V_VenomTimer) / 1000}
	};

	INT Y = 0;
	INT Y2 = 0;
	INT X = 10;
	CHAR S[20];
	strcpy(S, "State List");

	for (INT i = 0; i < ArraySize(States); i++)
	{
		if (!GetUnitState(Me, States[i].State))
			continue;

		Y += 10;

		if ((INT)strlen(States[i].Text) > X)
		{
			X = (INT)strlen(States[i].Text);
			strcpy(S, States[i].Text);
		}
	}

	Rectangle(V_StateListX, V_StateListY, strlen(S) > 10 ? GetTextWidth(S, 6) + 10 : 60, Y + 22, 0, 1);
	DrawOutCenter(strlen(S) > 10 ? V_StateListX + (GetTextWidth(S, 6) / 2) + 5 : V_StateListX + 30, Y ? V_StateListY + 12 : V_StateListY + 15, V_StateListTitleColor, 6, 1, "State List");

	for (INT i = 0; i < ArraySize(States); i++)
	{
		if (!GetUnitState(Me, States[i].State))
			continue;

		Y2 += 10;
		DrawOutCenter(strlen(S) > 10 ? V_StateListX + (GetTextWidth(S, 6) / 2) + 5 : V_StateListX + 30, V_StateListY + 17 + Y2, V_StateListStateColor, 6, 1, States[i].Text, States[i].Timer);
	}

	return TRUE;
}

BOOL Design::Stats()
{
	if (!ClientReady(FALSE) || !CharacterStats)
		return FALSE;

	INT LevelExperience = GetExp(GetUnitStat(Me, STAT_LEVEL)) - GetExp(GetUnitStat(Me, STAT_LEVEL) - 1);
	INT PlayerExperience = GetUnitStat(Me, STAT_EXP) - GetExp(GetUnitStat(Me, STAT_LEVEL) - 1);
	INT Resists[3] = {0, -40, -100};

	if (!V_StartingExperience)
		V_StartingExperience = GetUnitStat(Me, STAT_EXP);

	Rectangle(5, 100, 220, 355, 0, 4);
	TextHook(105, 115, 18, 0, 0, 1, "%s", Me->pPlayerData->szName);

	DrawOut(5, 130, 19, 6, "Attack Speedÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_IAS));
	DrawOut(5, 140, 19, 6, "Block Rateÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_FASTERBLOCK));
	DrawOut(5, 150, 19, 6, "Cast Rateÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_FASTERCAST));
	DrawOut(5, 160, 19, 6, "Hit Recoveryÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_FASTERHITRECOVERY));
	DrawOut(5, 170, 19, 6, "Run/Walk Speedÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_FASTERRUNWALK));
	DrawOut(5, 180, 19, 6, "Velocityÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_VELOCITYPERCENT));

	DrawOut(5, 200, 8, 6, "Experienceÿc0: ÿc8%d%%", CalcPercent(PlayerExperience, LevelExperience));
	DrawOut(5, 210, 8, 6, "Experience gainedÿc0: ÿc8%s", AddCommas(GetUnitStat(Me, STAT_EXP) - V_StartingExperience, 0));
	DrawOut(5, 220, 8, 6, "Experience neededÿc0: ÿc8%s", AddCommas(GetExp(GetUnitStat(Me, STAT_LEVEL)) - GetUnitStat(Me, STAT_EXP), 0));

	DrawOut(5, 240, 4, 6, "Gold Findÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_GOLDFIND));
	DrawOut(5, 250, 3, 6, "Magic Findÿc0: ÿc3%d%%", GetUnitStat(Me, STAT_MAGICFIND));

	DrawOut(5, 270, 1, 6, "Fire Resistÿc0: ÿc1%d ÿc0/ ÿc1%d", GetUnitStat(Me, STAT_FIRERESIST) + Resists[D2CLIENT_GetDifficulty()], GetUnitStat(Me, STAT_MAXFIRERESIST));
	DrawOut(5, 280, 3, 6, "Cold Resistÿc0: ÿc3%d ÿc0/ ÿc3%d", GetUnitStat(Me, STAT_COLDRESIST) + Resists[D2CLIENT_GetDifficulty()], GetUnitStat(Me, STAT_MAXCOLDRESIST));
	DrawOut(5, 290, 9, 6, "Lightning Resistÿc0: ÿc9%d ÿc0/ ÿc9%d", GetUnitStat(Me, STAT_LIGHTNINGRESIST) + Resists[D2CLIENT_GetDifficulty()], GetUnitStat(Me, STAT_MAXLIGHTNINGRESIST));
	DrawOut(5, 300, 2, 6, "Poison Resistÿc0: ÿc2%d ÿc0/ ÿc2%d", GetUnitStat(Me, STAT_POISONRESIST) + Resists[D2CLIENT_GetDifficulty()], GetUnitStat(Me, STAT_MAXPOISONRESIST));
	DrawOut(5, 310, 8, 6, "Magic Resistÿc0: ÿc8%d", GetUnitStat(Me, STAT_MAGICRESIST) + Resists[D2CLIENT_GetDifficulty()], GetUnitStat(Me, STAT_MAXMAGICRESIST));

	DrawOut(5, 330, 4, 6, "Damage Reductionÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_DAMAGEREDUCTION));
	DrawOut(5, 340, 8, 6, "Magic Damage Reductionÿc0: ÿc8%d%%", GetUnitStat(Me, STAT_MAGICDAMAGEREDUCTION));

	DrawOut(5, 360, 1, 6, "Fire Absorbÿc0: ÿc1%d ÿc0/ ÿc1%d%%", GetUnitStat(Me, STAT_FIREABSORB), GetUnitStat(Me, STAT_FIREABSORBPERCENT));
	DrawOut(5, 370, 3, 6, "Cold Absorbÿc0: ÿc3%d ÿc0/ ÿc3%d%%", GetUnitStat(Me, STAT_COLDABSORB), GetUnitStat(Me, STAT_COLDABSORBPERCENT));
	DrawOut(5, 380, 9, 6, "Lightning Absorbÿc0: ÿc9%d ÿc0/ ÿc9%d%%", GetUnitStat(Me, STAT_LIGHTNINGABSORB), GetUnitStat(Me, STAT_LIGHTNINGABSORBPERCENT));
	DrawOut(5, 390, 8, 6, "Magic Absorbÿc0: ÿc8%d ÿc0/ ÿc8%d%%", GetUnitStat(Me, STAT_MAGICABSORB), GetUnitStat(Me, STAT_MAGICABSORBPERCENT));

	DrawOut(5, 410, 4, 6, "Crushing Blowÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_CRUSHINGBLOW));
	DrawOut(5, 420, 4, 6, "Deadly Strikeÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_DEADLYSTRIKE));
	DrawOut(5, 430, 4, 6, "Open Woundsÿc0: ÿc4%d%%", GetUnitStat(Me, STAT_OPENWOUNDS));

	DrawOut(5, 450, 4, 6, "Goldÿc0: ÿc4%s", AddCommas(GetUnitStat(Me, STAT_GOLD) + GetUnitStat(Me, STAT_GOLDBANK), 0));

	return TRUE;
}

BOOL Design::Supplies()
{
	if (GetUIVar(UI_CHAT) || !V_Supplies || V_Options || !ClearScreen(3) || Hide)
		return FALSE;

	INT HP = GetTotalBeltHealth() + GetTotalCubeHealth() + GetTotalInvHealth() + GetTotalStashHealth();
	INT MP = GetTotalBeltMana() + GetTotalCubeMana() + GetTotalInvMana() + GetTotalStashMana();
	INT RP = GetTotalBeltRejuvenation() + GetTotalCubeRejuvenation() + GetTotalInvRejuvenation() + GetTotalStashRejuvenation();
	INT TP = GetTotalBeltTPs() + GetTotalCubeTPs() + GetTotalInvTPs() + GetTotalStashTPs();

	Rectangle(V_SuppliesX, V_SuppliesY, 137, 72, 0, 1);
	TextHook(V_SuppliesX + 65, V_SuppliesY + 12, V_SuppliesTitleColor, 0, 6, 1, "Supplies");

	TextHook(V_SuppliesX + 65, V_SuppliesY + 27, V_SuppliesHealPotColor, 0, 6, 1, HP == 1 ? "Heal Potion: %d" : "Heal Potions: %d", HP);
	TextHook(V_SuppliesX + 65, V_SuppliesY + 37, V_SuppliesManaPotColor, 0, 6, 1, MP == 1 ? "Mana Potion: %d" : "Mana Potions: %d", MP);
	TextHook(V_SuppliesX + 65, V_SuppliesY + 47, V_SuppliesJuvPotColor, 0, 6, 1, RP == 1 ? "Rejuvenation Potion: %d" : "Rejuvenation Potions: %d", RP);
	TextHook(V_SuppliesX + 65, V_SuppliesY + 57, V_SuppliesTPColor, 0, 6, 1, TP == 1 ? "Town Portal Scroll: %d" : "Town Portal Scrolls: %d", TP);
	TextHook(V_SuppliesX + 65, V_SuppliesY + 67, V_SuppliesGoldColor, 0, 6, 1, "Gold: %s", AddCommas(GetUnitStat(Me, STAT_GOLD) + GetUnitStat(Me, STAT_GOLDBANK), 0));

	return TRUE;
}

BOOL Design::Targets()
{
	if (Hide)
		return FALSE;

	if (!ClientReady(FALSE))
		return FALSE;

	if (GetUIVar(UI_CHAT) || V_Options || !ClearScreen(3))
		return FALSE;

	if (V_TargetList)
	{
		Rectangle(V_TargetListX, V_TargetListY, 68 + (LongestCharName(11) * 2), 22 + (10 * V_Players.GetSize()), 0, 1);
		TextHook(V_TargetListX + 30 + ((LongestCharName(11) * 2) / 2), V_Players.IsEmpty() ? V_TargetListY + 15 : V_TargetListY + 12, V_TargetListTitleColor, 0, 6, 1, "Target List");
	}

	if (V_TargetInfo)
	{
		Rectangle(V_TargetInfoX, V_TargetInfoY, V_Players.IsEmpty() ? 68 : strlen(V_Players[V_Target]->PlayerName) > 11 ? 68 + ((INT)strlen(V_Players[V_Target]->PlayerName) * 2) : 68, V_Players.IsEmpty() ? 22 : 62, 0, 1);
		TextHook(V_Players.IsEmpty() ? V_TargetInfoX + 30 : strlen(V_Players[V_Target]->PlayerName) > 11 ? V_TargetInfoX + 30 + ((INT)strlen(V_Players[V_Target]->PlayerName) * 2 / 2) : V_TargetInfoX + 30, V_Players.IsEmpty() ? V_TargetInfoY + 15 : V_TargetInfoY + 12, V_TargetInfoTitleColor, 0, 6, 1, "Target Info");
	}

	if (V_Players.IsEmpty())
		return FALSE;

	V_Players.Lock();

	for (INT i = 0; i < V_Players.GetSize(); i++)
	{
		if (V_TargetList)
			TextHook(V_TargetListX + 30 + ((LongestCharName(11) * 2) / 2), V_TargetListY + 27 + (10 * i), (i == V_Target) ? V_TargetListTargettedColor : V_TargetListNonTargettedColor, 0, 12, 1, V_Players[i]->PlayerName);

		if (V_TargetInfo && V_Target == i)
		{
			CHAR Class[20];
			GetPlayerClassName(V_Players[V_Target]->ClassId, Class, 20);

			TextHook(strlen(V_Players[V_Target]->PlayerName) > 11 ? V_TargetInfoX + 30 + ((INT)strlen(V_Players[V_Target]->PlayerName) * 2 / 2) : V_TargetInfoX + 30, V_TargetInfoY + 27, V_TargetInfoNameColor, 0, 6, 1, V_Players[V_Target]->PlayerName);
			TextHook(strlen(V_Players[V_Target]->PlayerName) > 11 ? V_TargetInfoX + 30 + ((INT)strlen(V_Players[V_Target]->PlayerName) * 2 / 2) : V_TargetInfoX + 30, V_TargetInfoY + 37, V_TargetInfoClassColor, 0, 6, 1, Class);
			TextHook(strlen(V_Players[V_Target]->PlayerName) > 11 ? V_TargetInfoX + 30 + ((INT)strlen(V_Players[V_Target]->PlayerName) * 2 / 2) : V_TargetInfoX + 30, V_TargetInfoY + 47, V_TargetInfoLevelColor, 0, 6, 1, "Level %d", V_Players[V_Target]->Level);
			TextHook(strlen(V_Players[V_Target]->PlayerName) > 11 ? V_TargetInfoX + 30 + ((INT)strlen(V_Players[V_Target]->PlayerName) * 2 / 2) : V_TargetInfoX + 30, V_TargetInfoY + 57, V_TargetInfoLevelColor, 0, 6, 1, "%d%% Life", V_Players[V_Target]->Life);
		}
	}

	V_Players.Unlock();

	return TRUE;
}

VOID Design::TextHook(INT X, INT Y, DWORD Color, BOOL Automap, INT Font, INT Center, LPSTR Text, ...)
{
	POINT nPos = {X, Y};
	DWORD dwOldSize, wWidth, dwFileNo;

	if (Automap)
	{
		if (!*p_D2CLIENT_AutomapOn)
			return;

		ScreenToAutomap(&nPos, X * 32, Y * 32);
	}

	CHAR szBuffer[800] = "";
	va_list Args;
	va_start(Args, Text);
	vsprintf_s(szBuffer, Text, Args);
	va_end(Args);

	WCHAR wBuffer[0x130];
	MultiByteToWideChar(0, 1, szBuffer, 100, wBuffer, 100);

	dwOldSize = D2WIN_SetTextSize(Font);

	if (Center != -1)
	{
		D2WIN_GetTextWidthFileNo(wBuffer, &wWidth, &dwFileNo);
		nPos.x -= (wWidth >> Center);
	}

	D2WIN_DrawText(wBuffer, nPos.x, nPos.y, Color, -1);
	D2WIN_SetTextSize(dwOldSize);
}

BOOL Design::Vectors()
{
	if (Hide)
		return FALSE;

	if (!ClientReady(FALSE))
		return FALSE;

	POINT Player, Position, Last = {0, 0};
	INT Colors[] = {V_NextLevelVectorColor, V_PreviousLevelVectorColor, V_QuestVectorColor, V_WaypointVectorColor};
	ScreenToAutomap(&Player, Me->pPath->xPos * 32, Me->pPath->yPos * 32);

	if (!V_Players.IsEmpty())
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(V_Players[V_Target]->UnitId, UNIT_TYPE_PLAYER);

		if (Unit)
		{
			if (V_TargetVector)
			{
				ScreenToAutomap(&Position, Unit->pPath->xPos * 32, Unit->pPath->yPos * 32);
				D2GFX_DrawLine(Player.x, Player.y, Position.x, Position.y, V_TargetVectorColor, -1);
			}

			if (V_TargetClick)
				DrawCross(Unit->pPath->xTarget, Unit->pPath->yTarget, V_TargetClickColor, TRUE);
		}
	}

	if (!V_TeleportQueue.IsEmpty())
		TextHook(GetScreenSizeX() / 2, (GetScreenSizeY() - 48) / 2, 5, 0, 8, 1, "Teleporting...");

	if (V_Vectors)
	{
		if (!V_CurrentVectors.IsEmpty())
		{
			V_CurrentVectors.Lock();

			for (INT i = 0; i < V_CurrentVectors.GetSize(); i++)
			{
				ScreenToAutomap(&Position, V_CurrentVectors[i]->Position.x, V_CurrentVectors[i]->Position.y);
				D2GFX_DrawLine(Player.x, Player.y, Position.x, Position.y, Colors[V_CurrentVectors[i]->Type - 1], -1);
				DrawCross(Position.x, Position.y, Colors[V_CurrentVectors[i]->Type - 1], FALSE);
			}

			V_CurrentVectors.Unlock();
		}

		if (!V_TeleportQueue.IsEmpty())
		{
			for (INT i = 0; i < V_TeleportQueue.GetSize(); i++)
			{
				POINT Calculation = {V_TeleportQueue[i].x, V_TeleportQueue[i].y};

				MapToScreenCoords(&Calculation);
				ScreenToAutomapRelative(&Position, Calculation.x, Calculation.y);
				DrawCross(Position.x, Position.y, V_ActiveVectorColor, FALSE);

				if (Last.x && Last.y)
					D2GFX_DrawLine(Last.x, Last.y, Position.x, Position.y, V_ActiveVectorColor, -1);
				else
					D2GFX_DrawLine(Player.x, Player.y, Position.x, Position.y, V_ActiveVectorColor, -1);

				Last.x = Position.x;
				Last.y = Position.y;
			}
		}
	}

	return TRUE;
}

VOID Design::WinDraw()
{
	GameFilter();
}

/*
BOOL Design::ProjectConsole(LPSTR Message, int COLOR)
{
    ConsoleMessages.push_back(Message);

    if(ConsoleMessages.empty())
        return true;

    SYSTEMTIME Time;
    GetLocalTime(&Time);
    char LocalTime[50];

    if (Time.wHour < 13 || Time.wHour > 23 && strlen(LocalTime) <= 0)
        sprintf_s(LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
    else if (Time.wHour < 24 || Time.wHour > 12 && strlen(LocalTime) <= 0)
        sprintf_s(LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

    int ConsoleHeigth = (ConsoleMessages.size() - 1) * 15;
    int ConsoleWidth  = (GetTextWidth(Message, 6) + 20);

    Rectangle(V_ConsoleX, V_ConsoleY, ConsoleWidth, ConsoleHeigth, 3, 3);

    DWORD dwSize = D2WIN_SetTextSize(D2FONT_SMALL);
    for(int I = 0; I < ConsoleMessages.size(); I++)
    {
        DrawOut(V_ConsoleX - 10, V_ConsoleY + (I * 16) + 4, COLOR, 6, "[%.2d:%.2d:%.2d] %s", Time.wHour, Time.wMinute, Time.wSecond, ConsoleMessages[I]);
    }

    D2WIN_SetTextSize(dwSize);
      /*
    for(int I = (List.size() > 8) ? List.size() - 8 : 0); I < List.size(); I++)
    {
        if(v_ConsoleDelay == 0 || (GetTickCount - Stamp) < v_ConsoleDelay)
        {
                //
        }
    }

    return true;
}
*/
