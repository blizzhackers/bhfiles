#include "Hack.h"

VOID Chicken()
{
	if (ClientReady(TRUE) && !InTown(Me) && !V_TP)
	{
		if (!V_TPLast || V_TPLast > 0 && (GetTickCount() - V_TPLast) >= (V_TPIntervalTime + (INT)Ping))
		{
			if (Portal())
			{
				if (!V_TPInteractLast || V_TPInteractLast > 0 && (GetTickCount() - V_TPInteractLast) >= (V_TPIntervalTime + (INT)Ping))
				{
					V_TP = TRUE;
					V_TPInteractLast = GetTickCount();
				}
			}

			else
				ExitGame();

			V_TPLast = GetTickCount();
		}
	}
}

BOOL Exit()
{
	if (!ClientReady(FALSE))
		return FALSE;

	DOUBLE LifePercent = CalcPercent((GetUnitStat(Me, STAT_HP) >> 8), (GetUnitStat(Me, STAT_MAXHP) >> 8));
	DOUBLE ManaPercent = CalcPercent((GetUnitStat(Me, STAT_MANA) >> 8), (GetUnitStat(Me, STAT_MAXMANA) >> 8));

	if (!V_DelayedChicken && !InTown(Me))
	{
		if (V_LifeExitPercent > 0 && LifePercent <= V_LifeExitPercent)
			ExitGame();

		if (V_ManaExitPercent > 0 && ManaPercent <= V_ManaExitPercent)
			ExitGame();
	}

	return TRUE;
}

VOID ExitGame()
{
	if (ClientReady(FALSE) && !InTown(Me))
	{
		D2CLIENT_ExitGame();
	}
}

BOOL Portal()
{
	BOOL Book = FALSE;
	INT Quantity = 0;
	LPUNITANY Item = NULL;

	if (!ClientReady(TRUE) || InTown(Me) || GetUnitState(Me, AFFECT_WHIRLWIND) || Me->pInventory->pCursorItem)
		return FALSE;

	Item = FindItem(518, STORAGE_STASH);

	if (!Item || GetUnitStat(Item, STAT_AMMOQUANTITY) == 0)
		Item = FindItem(518, STORAGE_INVENTORY);
	else
		Book = TRUE;

	if (!Item || GetUnitStat(Item, STAT_AMMOQUANTITY) == 0)
		Item = FindItem(518, STORAGE_CUBE);
	else
		Book = TRUE;

	if (!Item || GetUnitStat(Item, STAT_AMMOQUANTITY) == 0)
		Item = FindItem(529, STORAGE_BELT);
	else
		Book = TRUE;

	if (!Item)
		Item = FindItem(529, STORAGE_INVENTORY);

	if (!Item)
		Item = FindItem(529, STORAGE_STASH);

	if (!Item)
		Item = FindItem(529, STORAGE_CUBE);

	if (!Item)
	{
		Print(0, 4, "Escapeÿc0: ÿc1No town portal scrolls found");
		return FALSE;
	}

	if (Item)
		UseItem(Item);

	if (Book)
	{
		Quantity = GetUnitStat(Item, STAT_AMMOQUANTITY);

		if (Quantity <= 5)
			Print(0, 4, (Quantity == 2) ? "Escapeÿc0: ÿc1%d town portal scroll remaining" : "Escapeÿc0: ÿc1%d town portal scrolls remaining", Quantity - 1);
	}

	return TRUE;
}

BOOL Town()
{
	if (!ClientReady(TRUE))
		return FALSE;

	DOUBLE LifePercent = CalcPercent((GetUnitStat(Me, STAT_HP) >> 8), (GetUnitStat(Me, STAT_MAXHP) >> 8));
	DOUBLE ManaPercent = CalcPercent((GetUnitStat(Me, STAT_MANA) >> 8), (GetUnitStat(Me, STAT_MAXMANA) >> 8));

	if (!V_DelayedChicken && !InTown(Me) && !GetUnitState(Me, AFFECT_WHIRLWIND))
	{
		if (V_LifeTownPercent > 0 && LifePercent <= V_LifeTownPercent)
			Chicken();

		if (V_ManaTownPercent > 0 && ManaPercent <= V_ManaTownPercent)
			Chicken();
	}

	return TRUE;
}