#include "Hack.h"

VOID DestroyEnchantQueue()
{
	if (!V_EnchantQueue.IsEmpty())
	{
		for (INT i = 0; i < V_EnchantQueue.GetSize(); i++)
		{
			if (V_EnchantQueue[i])
				delete V_EnchantQueue[i];
		}

		V_EnchantQueue.RemoveAll();
	}
}

BOOL ExecuteEnchantQueue(LPENCHANTINFO EnchInfo)
{
	if (!ClientReady(TRUE) || !EnchInfo)
		return FALSE;

	if (EnchInfo->dwPlayerId == Me->dwUnitId)
		return FALSE;

	if (!(GetPvPFlags(EnchInfo->dwPlayerId) & PVP_ALLIED_WITH_YOU))
	{
		LPUNITANY Unit = (LPUNITANY)GetUnit(EnchInfo->dwPlayerId, UNIT_TYPE_PLAYER);
		Print(0, 4, "Enchantÿc0: ÿc1%s is not in your party", Unit->pPlayerData->szName);

		if (V_EnchantNote)
			Say("You're not in my party.");

		return FALSE;
	}

	if (!GetSkill(D2S_ENCHANT))
	{
		Print(0, 4, "Enchantÿc0: You don't have the Enchant skill");
		return FALSE;
	}

	if (GetCurrentSkill(V_LeftAttack) != D2S_ENCHANT)
		SetSkill(D2S_ENCHANT, V_LeftAttack);

	while (GetCurrentSkill(V_LeftAttack) != D2S_ENCHANT)
		SleepEx(1, TRUE);

	if (EnchInfo->nEnchantType == PLAYER)
	{
		LPUNITANY lpTarget = (LPUNITANY)GetUnit(EnchInfo->dwPlayerId, UNIT_TYPE_PLAYER);

		if (!lpTarget)
		{
			Print(0, 4, "Enchantÿc0: ÿc1%s is out of your range", lpTarget->pPlayerData->szName);

			if (V_EnchantNote)
				Say("You're out of range.");

			return FALSE;
		}

		UseSkillOnUnit(lpTarget, D2S_ENCHANT, FALSE, FALSE, FALSE);
		return TRUE;
	}

	if (EnchInfo->nEnchantType == MERCENARY)
	{
		for (LPROOM1 pRoom1 = Me->pAct->pRoom1; pRoom1; pRoom1 = pRoom1->pRoomNext)
		{
			for (LPUNITANY pUnit = pRoom1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
			{
				if (pUnit->dwType == UNIT_TYPE_NPC && IsMercClassId(pUnit->dwTxtFileNo))
				{
					DWORD dwOwner = D2CLIENT_GetMonsterOwner(pUnit->dwUnitId);

					if (dwOwner == EnchInfo->dwPlayerId)
					{
						UseSkillOnUnit(pUnit, D2S_ENCHANT, FALSE, FALSE, FALSE);
						return TRUE;
					}
				}
			}
		}

		if (V_EnchantNote)
			Say("You don't have a mercenary.");

		return FALSE;
	}

	if (EnchInfo->nEnchantType == ALL)
	{
		for (LPROOM1 pRoom1 = Me->pAct->pRoom1; pRoom1; pRoom1 = pRoom1->pRoomNext)
		{
			for (LPUNITANY pUnit = pRoom1->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
			{
				if (pUnit->dwUnitId == EnchInfo->dwPlayerId)
				{
					UseSkillOnUnit(pUnit, D2S_ENCHANT, FALSE, FALSE, FALSE);
					SleepEx(500, TRUE);
				}

				if (pUnit->dwType == UNIT_TYPE_NPC)
				{
					DWORD dwOwner = D2CLIENT_GetMonsterOwner(pUnit->dwUnitId);

					if (dwOwner == EnchInfo->dwPlayerId)
					{
						UseSkillOnUnit(pUnit, D2S_ENCHANT, FALSE, FALSE, FALSE);
						SleepEx(500, TRUE);
					}
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}