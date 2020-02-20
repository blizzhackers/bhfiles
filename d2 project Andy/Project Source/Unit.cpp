#include "Hack.h"

LPUNITANY FindNearestUnit(POINT Mouse, INT DamageType, BOOL TestSelect, BOOL CheckLoS)
{
	LPUNITANY Target = NULL;
	INT Distance = 9999;

	if (V_Players.IsEmpty())
		return FALSE;

	for (LPROOM1 Room1 = Me->pAct->pRoom1; Room1; Room1 = Room1->pRoomNext)
	{
		for (LPUNITANY Unit = Room1->pUnitFirst; Unit; Unit = Unit->pListNext)
		{
			if (Unit && Unit->dwType == UNIT_TYPE_PLAYER)
			{
				if (Unit->dwUnitId == Me->dwUnitId)
					continue;

				if ((INT)GetUnitDist(Me, Unit) > Distance)
					continue;

				if (InTown(Unit))
					continue;

				if (Unit->dwMode == PLAYER_MODE_DEAD || Unit->dwMode == PLAYER_MODE_DEATH)
					continue;

				if (CheckLoS && D2COMMON_CheckUnitCollision(Me, Unit, 4))
					continue;

				if (GetPvPFlags(Unit->dwUnitId) & PVP_HOSTILED_YOU || GetPvPFlags(Unit->dwUnitId) & PVP_HOSTILED_BY_YOU)
				{
					Target = Unit;
					Distance = (INT)GetUnitDist(Me, Unit);
				}
			}
		}
	}

	return Target;
}

LPUNITANY FindMercUnit(LPUNITANY pOwner)
{
	for(LPROOM1 pRoom1 = Me->pAct->pRoom1; pRoom1; pRoom1 = pRoom1->pRoomNext)
		for(LPUNITANY pUnit = pRoom1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
			if(IsMercClassId(pUnit->dwTxtFileNo) && D2CLIENT_GetMonsterOwner(pUnit->dwUnitId) == pOwner->dwUnitId)
				return pUnit;

	return NULL;
}

LPROSTERUNIT FindPartyById(DWORD dwUnitId)
{
	for(LPROSTERUNIT pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if(pUnit->dwUnitId == dwUnitId)
			return pUnit;

	return NULL;
}

LPROSTERUNIT FindPartyByName(LPSTR szName)
{
	for(LPROSTERUNIT pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if(!_stricmp(szName, pUnit->szName))
			return pUnit;

	return NULL;
}

LPPRESETUNIT FindPresetUnit(DWORD dwClassId, DWORD dwType, LPROOM2 *pRoom)
{
	for(LPROOM2 pRoom2 = GetUnitLevel(Me)->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Next)
	{
		for(LPPRESETUNIT pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			if(pUnit->dwType == dwType && pUnit->dwTxtFileNo == dwClassId)
			{
				*pRoom = pRoom2;
				return pUnit;
			}
		}
	}

	return NULL;
}

LPUNITANY FindUnitFromUnitNo(DWORD unitno)
{
	LPUNITANY Unit = NULL;

	while (Unit ? Unit = Unit->pListNext : Unit = *p_D2CLIENT_pUnitList)
		if (Unit->dwMode == unitno)
			break;

	return Unit;
}

LPUNITANY GetSelectedUnit()
{
	return D2CLIENT_GetSelectedUnit();
}

DWORD GetPvPFlags(DWORD dwPlayerId)
{
	if (!ClientReady(FALSE))
		return FALSE;

	DWORD dwFlags = 0;

	for (LPROSTERUNIT pUnit = (*p_D2CLIENT_PlayerUnitList); pUnit; pUnit = pUnit->pNext)
	{
		if (pUnit->dwUnitId != dwPlayerId)
			continue;

		if (pUnit->dwUnitId != Me->dwUnitId)
		{
			if (TestPvPFlag(dwPlayerId, Me->dwUnitId, 8))
				dwFlags |= PVP_HOSTILED_YOU;

			if (TestPvPFlag(Me->dwUnitId, dwPlayerId, 8))
				dwFlags |= PVP_HOSTILED_BY_YOU;

			if (pUnit->dwPartyFlags == 2)
				dwFlags |= PVP_INVITED_YOU;

			if (pUnit->dwPartyFlags == 4)
				dwFlags |= PVP_INVITED_BY_YOU;

			if (pUnit->wPartyId != 0xFFFF && pUnit->dwPartyFlags == 1)
			{
				LPROSTERUNIT pMe = FindPartyById(Me->dwUnitId);

				if (pMe->wPartyId == 0xFFFF)
					dwFlags |= PVP_ALLIED;
				else if (pMe->wPartyId == pUnit->wPartyId)
					dwFlags |= PVP_ALLIED_WITH_YOU;
			}

			return dwFlags;
		}
	}

	return dwFlags;
}

LPROOM1 GetRoom(UINT X, UINT Y)
{
	for (UINT i = 0; i < Me->pPath->pRoom1->dwRoomsNear; i++)
	{
		LPROOM1 Room = Me->pPath->pRoom1->pRoomsNear[i];

		if (Room->dwXStart <= X && Room->dwYStart <= Y && Room->dwXStart + Room->dwXSize >= X && Room->dwYStart + Room->dwYSize >= Y)
			return Room;
	}

	return NULL;
}

DWORD GetUnitDist(LPUNITANY pUnit1, LPUNITANY pUnit2)
{
	DWORD dwDist[4] = {0};

	switch(pUnit1->dwType)
	{
	case UNIT_TYPE_PLAYER:
	case UNIT_TYPE_MISSILE:
	case UNIT_TYPE_ITEM:
		dwDist[0] = pUnit1->pPath->xPos;
		dwDist[1] = pUnit1->pPath->yPos;
		break;

	case UNIT_TYPE_OBJECT:
		dwDist[0] = pUnit1->pObjectPath->dwPosX;
		dwDist[1] = pUnit1->pObjectPath->dwPosY;
		break;
	}

	switch(pUnit2->dwType)
	{
	case UNIT_TYPE_PLAYER:
	case UNIT_TYPE_MISSILE:
	case UNIT_TYPE_ITEM:
		dwDist[2] = pUnit2->pPath->xPos;
		dwDist[3] = pUnit2->pPath->yPos;
		break;

	case UNIT_TYPE_OBJECT:
		dwDist[2] = pUnit2->pObjectPath->dwPosX;
		dwDist[3] = pUnit2->pObjectPath->dwPosY;
		break;
	}

	for(int x = 0; x < 4; x++)
		if(!dwDist[x])
			return INFINITE;

	return (DWORD)CalculateDistance(dwDist[0], dwDist[1], dwDist[2], dwDist[3]);
}

LPLEVEL GetUnitLevel(LPUNITANY pUnit)
{
	if(pUnit)
		if(pUnit->pPath)
			if(pUnit->pPath->pRoom1)
				if(pUnit->pPath->pRoom1->pRoom2)
					return pUnit->pPath->pRoom1->pRoom2->pLevel;

	return FALSE;
}

BOOL InTown(LPUNITANY Unit)
{
	LPLEVEL CurrentLevel = GetUnitLevel(Unit);

	if (!CurrentLevel)
		return TRUE;

	return IsTownLevel(CurrentLevel->dwLevelNo) || CurrentLevel->dwLevelNo == 0;
}

BOOL IsImmuneMonster(LPUNITANY Unit, D2SpellInfo Spell)
{
	if (!ClientReady(FALSE))
		return TRUE;

	if (Spell.dwSpellInfoFlag & DSI_COLD)
	{
		if ((INT)GetUnitStat(Unit, STAT_COLDRESIST) >= 100)
			return TRUE;
	}

	if (Spell.dwSpellInfoFlag & DSI_FIRE)
	{
		if ((INT)GetUnitStat(Unit, STAT_FIRERESIST) >= 100)
			return TRUE;
	}

	if (Spell.dwSpellInfoFlag & DSI_LIGHTNING)
	{
		if ((INT)GetUnitStat(Unit, STAT_LIGHTNINGRESIST) >= 100)
			return TRUE;
	}

	if (Spell.dwSpellInfoFlag & DSI_POISON)
	{
		if ((INT)GetUnitStat(Unit, STAT_POISONRESIST) >= 100)
			return TRUE;
	}

	if (Spell.dwSpellInfoFlag & DSI_PHYSICAL)
	{
		if (Spell.dwSpellInfoFlag & DSI_LIGHTNING)
			return FALSE;
		
		if ((INT)GetUnitStat(Unit, STAT_DAMAGEREDUCTION) >= 100)
			return TRUE;
	}

	if (Spell.dwSpellInfoFlag & DSI_MAGICAL)
	{
		if ((INT)GetUnitStat(Unit, STAT_MAGICRESIST) >= 100)
			return TRUE;
	}

	return FALSE;
}

BOOL IsMercClassId(DWORD dwClassID)
{
	return dwClassID == MERC_A1 || dwClassID == MERC_A2 || dwClassID == MERC_A3 || dwClassID == MERC_A5;
}

BOOL IsTownLevel(INT nLevel)
{
	if(nLevel == MAP_A1_ROGUE_ENCAMPMENT ||
		nLevel == MAP_A2_LUT_GHOLEIN || 
		nLevel == MAP_A3_KURAST_DOCKS ||
		nLevel == MAP_A4_THE_PANDEMONIUM_FORTRESS ||
		nLevel == MAP_A5_HARROGATH)
		return TRUE;

	return FALSE;
}

BOOL TestUnit(LPUNITANY Unit)
{
	if (Unit && Unit->pInventory)
	{
		if (Unit->dwType == UNIT_TYPE_PLAYER)
		{
			if (Unit->dwMode != PLAYER_MODE_DEAD || Unit->dwMode != PLAYER_MODE_DEATH)
				return Unit->dwMode;
		}
	}

	return FALSE;
}

BOOL UseSkillOnUnit(LPUNITANY Unit, WORD Skill, BOOL Left, BOOL Shift, BOOL Animation)
{
	if (!ClientReady(TRUE))
		return FALSE;

	if (!GetSkill(Skill))
		return FALSE;

	if (UsingSkill())
		return FALSE;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = Shift ? (Left ? 0x0A : 0x11) : (Left ? 0x06 : 0x0D);
	*(LPDWORD)&Packet[1] = Unit->dwType;
	*(LPDWORD)&Packet[5] = Unit->dwUnitId;
	D2NET_SendPacket(9, 1, Packet);
	delete [] Packet;

	if (Animation)
	{
		LPBYTE Packet = new BYTE[16];
		Packet[0] = 0x4C;
		*(LPDWORD)&Packet[1] = Me->dwType;
		*(LPDWORD)&Packet[2] = Me->dwUnitId;
		*(LPWORD)&Packet[6] = Skill;
		*(LPDWORD)&Packet[9] = Unit->dwType;
		*(LPDWORD)&Packet[10] = Unit->dwUnitId;
		D2NET_ReceivePacket(Packet, 16);
		delete [] Packet;
	}

	return TRUE;
}