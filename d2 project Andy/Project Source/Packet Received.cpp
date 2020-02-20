#include "Hack.h"

BOOL FASTCALL OnGamePacketReceived(LPBYTE Packet, DWORD Length)
{
	if (Packet[0] == 0x0D)
	{
		if (!V_Players.IsEmpty())
		{
			for (INT i = 0; i < V_Players.GetSize(); i++)
			{
				if (*(LPDWORD)&Packet[2] == V_Players[i]->UnitId)
				{
					if (Packet[12] > 100)
						Packet[12] = 100;

					V_Players[i]->Life = Packet[12];
				}
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0x0E)
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], (DWORD)Packet[1]);

		if (Unit->dwType == UNIT_TYPE_OBJECT && Unit->dwTxtFileNo == 0x3B)
			return FALSE;

		return TRUE;
	}

	if (Packet[0] == 0x15)
	{
		if (*(LPDWORD)&Packet[2] == Me->dwUnitId)
		{
			//if (V_AutoBlind && V_Reblind) V_Reblind = FALSE;
			//if (V_AutoKB && V_KB) V_KB = FALSE;

			if (V_AntiFlash)
			{
				Packet[10] = NULL;

				if (Me->pPath->xPos == *(LPWORD)&Packet[6] && Me->pPath->yPos == *(LPWORD)&Packet[8])
					return FALSE;
			}

			if (V_FastTeleport)
			{
				/*if (Me->dwMode == PLAYER_MODE_DEATH || Me->dwMode == PLAYER_MODE_STAND_OUTTOWN ||
					Me->dwMode == PLAYER_MODE_WALK_OUTTOWN || Me->dwMode == PLAYER_MODE_RUN || 
					Me->dwMode == PLAYER_MODE_STAND_INTOWN || Me->dwMode == PLAYER_MODE_WALK_INTOWN ||
					Me->dwMode == PLAYER_MODE_DEAD || Me->dwMode == PLAYER_MODE_SEQUENCE ||
					Me->dwMode == PLAYER_MODE_BEING_KNOCKBACK)
					return TRUE;*/

				if (Me->dwMode == PLAYER_MODE_CAST)
					Me->dwFrameRemain = 0;
			}
		}

		if (!V_Players.IsEmpty())
		{
			if (Packet[1] == UNIT_TYPE_PLAYER && *(LPDWORD)&Packet[2] != Me->dwUnitId && !InTown(Me))
			{
				DWORD Flags = GetPvPFlags(*(LPDWORD)&Packet[2]);

				if (Flags & PVP_HOSTILED_YOU || Flags & PVP_HOSTILED_BY_YOU)
				{
					if (V_AutoBlind && !V_Reblind) V_Reblind = TRUE;
					if (V_AutoKB && !V_KB) V_KB = TRUE;
				}
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0x20)
	{
		if (V_AntiItemGlitch)
		{
			if (*(LPDWORD)&Packet[2] != Me->dwUnitId)
			{
				if (Packet[5] == STAT_STRENGTH || Packet[5] == STAT_DEXTERITY)
					if (Packet[6] < 255) Packet[6] = 255;
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0x26)
	{
		LPSTR lpPlayerName = (LPSTR)&Packet[10];
		LPSTR lpMessage = (LPSTR)&Packet[strlen(lpPlayerName) + 11];
		LPENCHANTINFO lpEnchInfo = new ENCHANTINFO;
		LPROSTERUNIT lpPlayer = FindPartyByName(lpPlayerName);

		if (V_Enchant && lpPlayer)
		{
			lpEnchInfo->dwPlayerId = lpPlayer->dwUnitId;
			lpEnchInfo->nEnchantType = NULL;

			if (!_stricmp(lpMessage, V_EnchantPlayer))
				lpEnchInfo->nEnchantType = PLAYER;

			if (!_stricmp(lpMessage, V_EnchantMercenary))
				lpEnchInfo->nEnchantType = MERCENARY;

			if (!_stricmp(lpMessage, V_EnchantAll))
				lpEnchInfo->nEnchantType = ALL;

			if (lpEnchInfo->nEnchantType)
				V_EnchantQueue.Add(lpEnchInfo);
			else
				delete lpEnchInfo;
		}

		return TRUE;
	}

	if (Packet[0] == 0x4C)
	{
		if (*(LPDWORD)&Packet[10] == Me->dwUnitId && !InTown(Me) && V_TPWhenAttacked)
		{
			D2SpellInfo Spell;
			GetSkillInfo(*(LPWORD)&Packet[8], &Spell);

			if (Spell.dwSpellInfoFlag & DSI_PARTYCAST)
				return TRUE;

			LPUNITANY pUnit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], UNIT_TYPE_PLAYER);

			if (!pUnit || pUnit->dwOwnerType != UNIT_TYPE_PLAYER)
				return TRUE;

			LPROSTERUNIT pRoster = FindPartyById(pUnit->dwUnitId);

			if (!pRoster)
				return TRUE;

			DWORD dwFlags = GetPvPFlags(pRoster->dwUnitId);

			if (dwFlags & PVP_HOSTILED_BY_YOU || dwFlags & PVP_HOSTILED_YOU)
			{
				Print(0, 4, "Escapeÿc0: ÿc1%s attacked you", pRoster->szName);

				if (Portal())
					V_TP = TRUE;
				else
					ExitGame();
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0x51)
	{
		if (Packet[6] == 0x3B)
		{
			if (V_FastTP)
			{
				if (Packet[12] < 2)
					Packet[12] = 2;
			}

			/*if (V_TP && !InTown(Me))
			{
				if (Interact(*(LPDWORD)&Packet[2], UNIT_TYPE_OBJECT, FALSE))
					V_TP = FALSE;
			}*/
		}

		return TRUE;
	}

	if (Packet[0] == 0x5A)
	{
		if (Packet[1] == 0x02)
		{
			LPROSTERUNIT Unit = FindPartyByName((LPSTR)&Packet[8]);

			if (!Unit || Unit->dwUnitId == Me->dwUnitId)
				return TRUE;

			if (V_Enchant && V_EnchantNote)
				V_EnchantNoteIteration = TRUE;

			if (V_ModifiedJoinMessage)
			{
				CHAR Class[40] = "";
				GetPlayerClassName(Unit->dwClassId, Class, 40);

				if (strlen(Class) <= 0)
					return TRUE;

				Print(0, 4, "%s(%s) [Level %d %s] has joined our world. Diablo's minions grow stronger.", (LPSTR)&Packet[8], (LPSTR)&Packet[24], Unit->wLevel, Class);
				return FALSE;
			}
		}

		if (Packet[1] == 0x06)
		{
			if (!_stricmp((LPSTR)&Packet[24], Me->pPlayerData->szName))
			{
				SetDeathMessage();
				strcpy_s(V_SlainName, (LPSTR)&Packet[8]);

				if (V_KillLog) KillLog();
				if (V_DeathMessage) V_DeathMessageIteration = TRUE;
			}

			if (!_stricmp((LPSTR)&Packet[8], Me->pPlayerData->szName))
			{
				V_InjectDeathCount++;
				V_InGameDeathCount++;

				sprintf_s(V_InjectDeaths, "%i", V_InjectDeathCount);
				sprintf_s(V_InGameDeaths, "%i", V_InGameDeathCount);
			}
		}

		return TRUE;
	}

	/*if (Packet[0] == 0x5C)
	{
		LPROSTERUNIT Roster = FindPartyById(*(LPDWORD)&Packet[1]);
		CHAR Class[0x40] = "";

		if (!Roster)
			return TRUE;

		GetPlayerClassName(Roster->dwClassId, Class, 0x40);
		
		if (V_ModifiedJoinLeave)
			Print(0, 4, "%s [Level %d %s] left our world. Diablo's minions weaken.", Roster->szName, Roster->wLevel, Class);

		return TRUE;
	}*/

	if (Packet[0] == 0x5D)
	{
		if (V_AntiQuestBox && !GetUIVar(UI_QUEST))
			return FALSE;

		return TRUE;
	}

	if (Packet[0] == 0x60)
	{
		if (V_TP && !InTown(Me) && IsTownLevel(Packet[2]))
		{
			LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[3], UNIT_TYPE_OBJECT);

			if (!_stricmp(Unit->pObjectData->szOwner, Me->pPlayerData->szName))
			{
				if (Interact(Unit->dwUnitId, Unit->dwType, TRUE))
					V_TP = FALSE;
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0x77)
	{
		if (Packet[1] == 0x0C)
			V_CubeOrStashOpened = FALSE;

		return TRUE;
	}

	/*if (Packet[0] == 0x82)
	{
		if (V_TP && !InTown(Me))
		{
			if (*(LPDWORD)&Packet[1] == Me->dwUnitId)
			{
				if (Interact(*(LPDWORD)&Packet[21], UNIT_TYPE_OBJECT, TRUE))
					V_TP = FALSE;
			}
		}

		return TRUE;
	}*/

	if (Packet[0] == 0x89)
	{
		if (V_Quests)
		{
			if (Packet[1] == 0) Print(0, 4, "Questÿc0: ÿc4Den of Evil");
			if (Packet[1] == 1) Print(0, 4, "Questÿc0: ÿc4Tristram");
			if (Packet[1] == 3) Print(0, 4, "Questÿc0: ÿc4Tal-Rasha's Tomb");
			if (Packet[1] == 6) Print(0, 4, "Questÿc0: ÿc4Claw-Viper Temple");
			if (Packet[1] == 7) Print(0, 4, "Questÿc0: ÿc4The Summoner");
			if (Packet[1] == 8) Print(0, 4, "Questÿc0: ÿc4Duriel");
			if (Packet[1] == 10) Print(0, 4, "Questÿc0: ÿc4Khalim's Will");
			if (Packet[1] == 11) Print(0, 4, "Questÿc0: ÿc4Mephisto");
			if (Packet[1] == 12) Print(0, 4, "Questÿc0: ÿc4Diablo");
			if (Packet[1] == 13) Print(0, 4, "Questÿc0: ÿc4Diablo");
			if (Packet[1] == 14) Print(0, 4, "Questÿc0: ÿc4Hell's Forge");
			if (Packet[1] == 15) Print(0, 4, "Questÿc0: ÿc4Shenk the Overseer");
			if (Packet[1] == 16) Print(0, 4, "Questÿc0: ÿc4Anya");
			if (Packet[1] == 17) Print(0, 4, "Questÿc0: ÿc4Nihlathak");
			if (Packet[1] == 18) Print(0, 4, "Questÿc0: ÿc4Ancients");
			if (Packet[1] == 19) Print(0, 4, "Questÿc0: ÿc4Baal");
		}

		return TRUE;
	}

	if (Packet[0] == 0x8C)
	{
		if (V_TPWhenHostiled && *(LPWORD)&Packet[9] >= 8)
		{
			if (*(LPDWORD)&Packet[1] == Me->dwUnitId && !InTown(Me))
			{
				LPROSTERUNIT Player = FindPartyById(*(LPDWORD)&Packet[5]);
				Print(0, 4, "Escapeÿc0: ÿc1%s hostiled you", Player->szName);

				if (Portal())
					V_TP = TRUE;
				else
					ExitGame();
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0xA7)
	{
		if (V_NoTPDelay)
		{
			if (Packet[6] == AFFECT_JUST_PORTALED)
				return FALSE;
		}

		return TRUE;
	}

	if (Packet[0] == 0xA8)
	{
		if (V_Potion && Packet[1] == UNIT_TYPE_PLAYER && *(LPDWORD)&Packet[2] != Me->dwUnitId)
		{
			LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[2], UNIT_TYPE_PLAYER);

			if (Unit)
			{
				if (Packet[7] == AFFECT_HEALTHPOT)
					Print(0, 4, "Potionÿc0: ÿc1%s", Unit->pPlayerData->szName);

				if (Packet[7] == AFFECT_MANAPOT)
					Print(0, 4, "Potionÿc0: ÿc3%s", Unit->pPlayerData->szName);
			}
		}

		if (Packet[1] == UNIT_TYPE_PLAYER && *(LPDWORD)&Packet[2] == Me->dwUnitId)
		{
			if (!GetUnitState(Me, AFFECT_BATTLEORDERS))
				if (Packet[7] == AFFECT_BATTLEORDERS)
					V_DelayedChicken = TRUE;

			if (!GetUnitState(Me, AFFECT_OAKSAGE))
				if (Packet[7] == AFFECT_OAKSAGE)
					V_DelayedChicken = TRUE;

			if (Packet[7] == AFFECT_AMPLIFYDAMAGE)
			{
				if (V_AmplifyDamageTimer)
					V_AmplifyDamageTimer = 0;
				
				V_AmplifyDamageTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_BATTLECOMMAND)
			{
				if (V_BattleCommandTimer)
					V_BattleCommandTimer = 0;
				
				V_BattleCommandTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_BATTLECRY)
			{
				if (V_BattleCryTimer)
					V_BattleCryTimer = 0;

				V_BattleCryTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_BATTLEORDERS)
			{
				if (V_BattleOrdersTimer)
					V_BattleOrdersTimer = 0;

				V_BattleOrdersTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_BONEARMOR)
			{
				if (V_BoneArmorTimer)
					V_BoneArmorTimer = 0;

				V_BoneArmorTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_BURSTOFSPEED)
			{
				if (V_BurstOfSpeedTimer)
					V_BurstOfSpeedTimer = 0;

				V_BurstOfSpeedTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_CHILLINGARMOR)
			{
				if (V_ChillingArmorTimer)
					V_ChillingArmorTimer = 0;

				V_ChillingArmorTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_COLD)
			{
				if (V_ColdTimer)
					V_ColdTimer = 0;
				
				V_ColdTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_CONVICTED)
			{
				if (V_ConvictedTimer)
					V_ConvictedTimer = 0;

				V_ConvictedTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_CYCLONEARMOR)
			{
				if (V_CycloneArmorTimer)
					V_CycloneArmorTimer = 0;

				V_CycloneArmorTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_DECREPIFY)
			{
				if (V_DecrepifyTimer)
					V_DecrepifyTimer = 0;

				V_DecrepifyTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_ENCHANT)
			{
				if (V_EnchantTimer)
					V_EnchantTimer = 0;

				V_EnchantTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_ENERGYSHIELD)
			{
				if (V_EnergyShieldTimer)
					V_EnergyShieldTimer = 0;

				V_EnergyShieldTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_FADE)
			{
				if (V_FadeTimer)
					V_FadeTimer = 0;

				V_FadeTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_FREEZE)
			{
				if (V_FrozenTimer)
					V_FrozenTimer = 0;

				V_FrozenTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_FROZENARMOR)
			{
				if (V_FrozenArmorTimer)
					V_FrozenArmorTimer = 0;

				V_FrozenArmorTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_HOLYFREEZECHILL)
			{
				if (V_HolyFrozenTimer)
					V_HolyFrozenTimer = 0;

				V_HolyFrozenTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_HOLYSHIELD)
			{
				if (V_HolyShieldTimer)
					V_HolyShieldTimer = 0;

				V_HolyShieldTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_INNERSIGHT)
			{
				if (V_InnerSightTimer)
					V_InnerSightTimer = 0;

				V_InnerSightTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_IRONMAIDEN)
			{
				if (V_IronMaidenTimer)
					V_IronMaidenTimer = 0;

				V_IronMaidenTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_LIFETAP)
			{
				if (V_LifeTapTimer)
					V_LifeTapTimer = 0;

				V_LifeTapTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_LOWERRESIST)
			{
				if (V_LowerResistTimer)
					V_LowerResistTimer = 0;

				V_LowerResistTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_OAKSAGE)
			{
				if (V_OakSageTimer)
					V_OakSageTimer = 0;

				V_OakSageTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_OPENWOUNDS)
			{
				if (V_OpenWoundsTimer)
					V_OpenWoundsTimer = 0;

				V_OpenWoundsTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_POISON)
			{
				if (V_PoisonedTimer)
					V_PoisonedTimer = 0;

				V_PoisonedTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_SHIVERARMOR)
			{
				if (V_ShiverArmorTimer)
					V_ShiverArmorTimer = 0;

				V_ShiverArmorTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_SHOUT)
			{
				if (V_ShoutTimer)
					V_ShoutTimer = 0;

				V_ShoutTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_SLOWED)
			{
				if (V_SlowedTimer)
					V_SlowedTimer = 0;

				V_SlowedTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_TERROR)
			{
				if (V_TerrorTimer)
					V_TerrorTimer = 0;

				V_TerrorTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_THUNDERSTORM)
			{
				if (V_ThunderStormTimer)
					V_ThunderStormTimer = 0;

				V_ThunderStormTimer = GetTickCount();
			}

			if (Packet[7] == AFFECT_VENOMCLAWS)
			{
				if (V_VenomTimer)
					V_VenomTimer = 0;

				V_VenomTimer = GetTickCount();
			}
		}

		return TRUE;
	}

	if (Packet[0] == 0xA9)
	{
		if (*(LPDWORD)&Packet[2] == Me->dwUnitId)
		{
			if (Packet[6] == AFFECT_AMPLIFYDAMAGE)
				V_AmplifyDamageTimer = NULL;

			if (Packet[6] == AFFECT_BATTLECOMMAND)
				V_BattleCommandTimer = NULL;

			if (Packet[6] == AFFECT_BATTLECRY)
				V_BattleCryTimer = NULL;

			if (Packet[6] == AFFECT_BATTLEORDERS)
				V_BattleOrdersTimer = NULL;

			if (Packet[6] == AFFECT_BONEARMOR)
				V_BoneArmorTimer = NULL;

			if (Packet[6] == AFFECT_BURSTOFSPEED)
				V_BurstOfSpeedTimer = NULL;

			if (Packet[6] == AFFECT_CHILLINGARMOR)
				V_ChillingArmorTimer = NULL;

			if (Packet[6] == AFFECT_COLD)
				V_ColdTimer = NULL;

			if (Packet[6] == AFFECT_CONVICTED)
				V_ConvictedTimer = NULL;

			if (Packet[6] == AFFECT_CYCLONEARMOR)
				V_CycloneArmorTimer = NULL;

			if (Packet[6] == AFFECT_DECREPIFY)
				V_DecrepifyTimer = NULL;

			if (Packet[6] == AFFECT_ENCHANT)
				V_EnchantTimer = NULL;

			if (Packet[6] == AFFECT_ENERGYSHIELD)
				V_EnergyShieldTimer = NULL;

			if (Packet[6] == AFFECT_FADE)
				V_FadeTimer = NULL;

			if (Packet[6] == AFFECT_FREEZE)
				V_FrozenTimer = NULL;

			if (Packet[6] == AFFECT_FROZENARMOR)
				V_FrozenArmorTimer = NULL;

			if (Packet[6] == AFFECT_HOLYFREEZECHILL)
				V_HolyFrozenTimer = NULL;

			if (Packet[6] == AFFECT_HOLYSHIELD)
				V_HolyShieldTimer = NULL;

			if (Packet[6] == AFFECT_INNERSIGHT)
				V_InnerSightTimer = NULL;

			if (Packet[6] == AFFECT_IRONMAIDEN)
				V_IronMaidenTimer = NULL;

			if (Packet[6] == AFFECT_LIFETAP)
				V_LifeTapTimer = NULL;

			if (Packet[6] == AFFECT_LOWERRESIST)
				V_LowerResistTimer = NULL;

			if (Packet[6] == AFFECT_OAKSAGE)
				V_OakSageTimer = NULL;

			if (Packet[6] == AFFECT_OPENWOUNDS)
				V_OpenWoundsTimer = NULL;

			if (Packet[6] == AFFECT_POISON)
				V_PoisonedTimer = NULL;

			if (Packet[6] == AFFECT_SHIVERARMOR)
				V_ShiverArmorTimer = NULL;

			if (Packet[6] == AFFECT_SHOUT)
				V_ShoutTimer = NULL;

			if (Packet[6] == AFFECT_SLOWED)
				V_SlowedTimer = NULL;

			if (Packet[6] == AFFECT_TERROR)
				V_TerrorTimer = NULL;

			if (Packet[6] == AFFECT_THUNDERSTORM)
				V_ThunderStormTimer = NULL;

			if (Packet[6] == AFFECT_VENOMCLAWS)
				V_VenomTimer = NULL;
		}
	}

	return TRUE;
}

BOOL FASTCALL OnMCPPacketReceived(LPBYTE Packet)
{
	if (Packet[0] == 0x05 && V_FilterBox)
	{		
		wstring wFilter(V_FilterBox->wText);
		string sFilter(wFilter.begin(), wFilter.end());

		LPGAMELISTENTRY Entry = new GameListEntry;

		Entry->wRequestId = *(LPWORD)&Packet[1];
		Entry->dwIndex = *(LPDWORD)&Packet[3];
		Entry->bPlayers = Packet[7];
		Entry->dwStatus = *(LPDWORD)&Packet[8];
		Entry->sGameName = (LPSTR)(Packet + 12);
		Entry->sGameDesc = (LPSTR)(Packet + 13 + Entry->sGameName.length());

		if (Entry->bPlayers == 0)
		{
			delete Entry;
			return FALSE;
		}

		if (!V_GameList.empty())
		{
			for (list<LPGAMELISTENTRY>::iterator ListEntry = V_GameList.begin(); ListEntry != V_GameList.end(); ListEntry++)
			{
				if (!(*ListEntry)->sGameName.compare(Entry->sGameName))
				{
					delete Entry;
					return FALSE;
				}
			}
		}

		string sGameName;

		for (UINT i = 0; i < sFilter.length(); i++)
			sFilter[i] = toupper(sFilter[i]);

		for (UINT i = 0; i < Entry->sGameName.length(); i++)
			sGameName += toupper(Entry->sGameName[i]);

		if (wFilter.empty() || sGameName.find(sFilter) != string::npos)
		{
			LPCONTROLTEXT pText = new ControlText;
			memset(pText, NULL, sizeof(ControlText));

			pText->dwColor = 4;
			pText->wText = new wchar_t[64];
			pText->wText2 = new wchar_t[12];

			memset(pText->wText, NULL, 64);
			memset(pText->wText2, NULL, 12);

			wstring wGameName(Entry->sGameName.begin(), Entry->sGameName.end());

			wcscpy_s(pText->wText, 64, wGameName.c_str());
			pText->wText2[0] = Entry->bPlayers + 0x30;

			if ((*p_D2MULTI_GameListControl)->pFirstText)
			{
				(*p_D2MULTI_GameListControl)->pLastText->pNext = pText;
				(*p_D2MULTI_GameListControl)->pLastText = pText;
			}
			
			else
			{
				(*p_D2MULTI_GameListControl)->pFirstText = pText;	
				(*p_D2MULTI_GameListControl)->pLastText = pText;
				(*p_D2MULTI_GameListControl)->pSelectedText = pText;
			}

			(*p_D2MULTI_GameListControl)->dwSelectEnd += 1;
			(*p_D2MULTI_GameListControl)->pChildControl->dwScrollEntries =
				(*p_D2MULTI_GameListControl)->dwSelectEnd > 10 ? (*p_D2MULTI_GameListControl)->dwSelectEnd - 10 : 0;			
		}

		V_GameList.push_back(Entry);
		return FALSE;
	}

	return TRUE;
}