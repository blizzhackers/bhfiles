#include "Hack.h"

BOOL Health()
{
	LPUNITANY Item = NULL;
	DWORD ItemCodes[5] = {591, 590, 589, 588, 587};

	if (!ClientReady(TRUE) || InTown(Me) || GetUnitState(Me, AFFECT_WHIRLWIND) || Me->pInventory->pCursorItem)
		return FALSE;

	for (INT i = 0; i < ArraySize(ItemCodes); i++)
	{
		Item = FindItem(ItemCodes[i], STORAGE_STASH);

		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_BELT);
		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_INVENTORY);
		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_CUBE);
		if (!Item) continue;

		UseItem(Item);
		return TRUE;
	}

	return FALSE;
}

BOOL Mana()
{
	LPUNITANY Item = NULL;
	DWORD ItemCodes[5] = {596, 595, 594, 593, 592};

	if (!ClientReady(TRUE) || InTown(Me) || GetUnitState(Me, AFFECT_WHIRLWIND) || Me->pInventory->pCursorItem)
		return FALSE;

	for (INT i = 0; i < ArraySize(ItemCodes); i++)
	{
		Item = FindItem(ItemCodes[i], STORAGE_STASH);

		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_BELT);
		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_INVENTORY);
		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_CUBE);
		if (!Item) continue;

		UseItem(Item);
		return TRUE;
	}

	return FALSE;
}

BOOL Potion()
{
	if (!ClientReady(TRUE))
		return FALSE;

	DOUBLE LifePercent = CalcPercent((GetUnitStat(Me, STAT_HP) >> 8), (GetUnitStat(Me, STAT_MAXHP) >> 8));
	DOUBLE ManaPercent = CalcPercent((GetUnitStat(Me, STAT_MANA) >> 8), (GetUnitStat(Me, STAT_MAXMANA) >> 8));
	
	if (!V_DelayedChicken && !InTown(Me) && !GetUnitState(Me, AFFECT_WHIRLWIND))
	{
		if (V_LifePotPercent > 0 && LifePercent <= V_LifePotPercent)
		{
			if (!V_LifePotLast || V_LifePotLast > 0 && (GetTickCount() - V_LifePotLast) >= (V_LifePotDelay + (INT)Ping))
			{
				if (!GetUnitState(Me, AFFECT_HEALTHPOT))
				{
					if (Health())
					{
						if (V_LifePot)
							Print(0, 4, "Potionÿc0: ÿc1Health");

						V_LifePotLast = GetTickCount();
					}
				}
			}
		}

		if (V_LifeJuvPercent > 0 && LifePercent <= V_LifeJuvPercent)
		{
			if (!V_LifeJuvLast || V_LifeJuvLast > 0 && (GetTickCount() - V_LifeJuvLast) >= (V_LifeJuvDelay + (INT)Ping))
			{
				if (Rejuvenation())
				{
					if (V_LifeJuv)
						Print(0, 4, "Potionÿc0: ÿc;Rejuvenation ÿc0(ÿc1Lifeÿc0)");

					V_LifeJuvLast = GetTickCount();
				}
			}
		}

		if (V_ManaPotPercent > 0 && ManaPercent <= V_ManaPotPercent)
		{
			if (!V_ManaPotLast || V_ManaPotLast > 0 && (GetTickCount() - V_ManaPotLast) >= (V_ManaPotDelay + (INT)Ping))
			{
				if (!GetUnitState(Me, AFFECT_MANAPOT))
				{
					if (Mana())
					{
						if (V_ManaPot)
							Print(0, 4, "Potionÿc0: ÿc3Mana");

						V_ManaPotLast = GetTickCount();
					}
				}
			}
		}

		if (V_ManaJuvPercent > 0 && ManaPercent <= V_ManaJuvPercent)
		{
			if (!V_ManaJuvLast || V_ManaJuvLast > 0 && (GetTickCount() - V_ManaJuvLast) >= (V_ManaJuvDelay + (INT)Ping))
			{
				if (Rejuvenation())
				{
					if (V_ManaJuv)
						Print(0, 4, "Potionÿc0: ÿc;Rejuvenation ÿc0(ÿc3Manaÿc0)");

					V_ManaJuvLast = GetTickCount();
				}
			}
		}
	}

	return TRUE;
}

BOOL Rejuvenation()
{
	LPUNITANY Item = NULL;
	DWORD ItemCodes[2] = {516, 515};

	if (!ClientReady(TRUE) || InTown(Me) || GetUnitState(Me, AFFECT_WHIRLWIND) || Me->pInventory->pCursorItem)
		return FALSE;

	for (INT i = 0; i < ArraySize(ItemCodes); i++)
	{
		Item = FindItem(ItemCodes[i], STORAGE_STASH);

		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_BELT);
		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_INVENTORY);
		if (!Item) Item = FindItem(ItemCodes[i], STORAGE_CUBE);
		if (!Item) continue;

		UseItem(Item);
		return TRUE;
	}

	return FALSE;
}