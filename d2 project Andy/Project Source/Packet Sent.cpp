#include "Hack.h"

BOOL FASTCALL OnGamePacketSent(LPBYTE Packet, DWORD Length)
{
	if (Packet[0] <= 0x11)
	{
		if (V_TP)
			return FALSE;

		if (!V_TeleportQueue.IsEmpty())
			V_TeleportQueue.RemoveAll();

		if (V_TeleWalk)
		{
			Teleport(*(LPWORD)&Packet[1], *(LPWORD)&Packet[3]);
			V_TeleWalk = FALSE;
		}
	}

	if (Packet[0] == 0x13)
	{
		if (V_BlockTP && ClientReady(TRUE))
		{
			LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[5], UNIT_TYPE_OBJECT);
			DOUBLE LifePercent = CalcPercent((GetUnitStat(Me, STAT_HP) >> 8), (GetUnitStat(Me, STAT_MAXHP) >> 8));
			DOUBLE ManaPercent = CalcPercent((GetUnitStat(Me, STAT_MANA) >> 8), (GetUnitStat(Me, STAT_MAXMANA) >> 8));

			if (Unit && Unit->dwTxtFileNo == 0x3B && InTown(Me))
			{
				if (V_LifeTownPercent > 0 && LifePercent <= V_LifeTownPercent || V_ManaTownPercent > 0 && ManaPercent <= V_ManaTownPercent)
					return FALSE;
			}
		}
	}

	if (Packet[0] == 0x18)
	{
		if (V_CubeOrStashOpened)
			if ((*(LPDWORD)&Packet[13] == STORAGE_STASH || *(LPDWORD)&Packet[13] == STORAGE_CUBE) && !InTown(Me))
				return FALSE;
	}

	if (Packet[0] == 0x19)
	{
		if (V_CubeOrStashOpened)
		{
			LPUNITANY Unit = (LPUNITANY)GetUnit(*(LPDWORD)&Packet[1], UNIT_TYPE_ITEM);

			if (Unit && (GetItemLocation(Unit) == STORAGE_STASH || GetItemLocation(Unit) == STORAGE_CUBE))
				return FALSE;
		}
	}

	return TRUE;
}