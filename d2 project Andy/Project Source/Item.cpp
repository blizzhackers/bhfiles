#include "Hack.h"

LPUNITANY FASTCALL FindItem(DWORD dwCode, INT nLoc)
{
	LPUNITANY Item = Me->pInventory->pFirstItem;

	if (!Item)
		return FALSE;

	do
	{
		if (Item->dwType == UNIT_TYPE_ITEM)
		{
			if (Item->dwTxtFileNo == dwCode)
			{
				if (Item->dwTxtFileNo == 518 && GetUnitStat(Item, STAT_AMMOQUANTITY) == 0)
					continue;

				if (GetItemLocation(Item) == nLoc)
					return Item;
			}
		}
	}	while ((Item = Item->pItemData->pNextInvItem));

	/*for (LPUNITANY Item = Me->pInventory->pFirstItem; Item != NULL; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && Item->dwType == UNIT_TYPE_ITEM && Item->dwTxtFileNo == dwCode)
		{
			if (dwCode == 518 && Item->dwTxtFileNo == 518 && GetUnitStat(Item, STAT_AMMOQUANTITY) == 0)
				continue;

			if (GetItemLocation(Item) == nLoc)
				return Item;

			break;
		}
	}*/
	return FALSE;
}

INT GetItemLocation(LPUNITANY pItem)
{
	if(!pItem || !pItem->pItemData)
		return -1;

	switch(pItem->pItemData->ItemLocation)
	{
	case STORAGE_INVENTORY:
		return STORAGE_INVENTORY;

	case STORAGE_TRADE:
		if(pItem->pItemData->NodePage == NODEPAGE_STORAGE)
			return STORAGE_TRADE;

	case STORAGE_CUBE:
		return STORAGE_CUBE;

	case STORAGE_STASH:
		return STORAGE_STASH;

	case STORAGE_NULL:
		switch(pItem->pItemData->NodePage)
		{
		case NODEPAGE_EQUIP:
			return STORAGE_EQUIP;

		case NODEPAGE_BELTSLOTS:
			return STORAGE_BELT;
		}
	}

	return STORAGE_NULL;
}

LPINVENTORYINFO GetInventoryInfo(INT nLoc)
{
	static InventoryInfo Infos[] =
	{
		{0,	10, 4},
		{2,	10, 4},
		{3, 3,  4},
		{4, 6,  8},
	};

	for (INT i = 0; i < ArraySize(Infos); i++)
		if (Infos[i].nLocation == nLoc)
			return &Infos[i];

	return NULL;
}

BOOL GetItemCode(LPUNITANY pItem, LPSTR szBuffer, DWORD dwMax)
{
	if(!pItem)
		return FALSE;

	LPITEMTXT pTxt = D2COMMON_GetItemText(pItem->dwTxtFileNo);
	::memcpy(szBuffer, pTxt->szCode, min(3, dwMax));
	return (int)strlen(szBuffer);
}

DWORD UseItemIds(INT nLoc)
{
	if (Me->pInventory->dwLeftItemUid)
		return Me->pInventory->dwLeftItemUid;

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item->dwType == UNIT_TYPE_ITEM && Item->dwUnitId)
		{
			if (Item->pInventory)
			{
				if (GetItemLocation(Item) != nLoc)
					return Item->dwUnitId;
			}
		}
	}

	if (Me->pInventory->pLastItem)
		return Me->pInventory->pLastItem->dwUnitId;

	return NULL;
}

VOID UseItem(LPUNITANY Unit)
{
	if (!Unit)
		return;

	INT Location = GetItemLocation(Unit);

	if (Location == STORAGE_INVENTORY || Location == STORAGE_BELT)
	{
		LPBYTE Packet = new BYTE[13];
		Packet[0] = (Location == STORAGE_INVENTORY) ? 0x20 : 0x26;
		*(LPDWORD)&Packet[1] = Unit->dwUnitId;
		*(LPDWORD)&Packet[5] = (Location == STORAGE_INVENTORY) ? Me->pPath->xPos : NULL;
		*(LPDWORD)&Packet[9] = (Location == STORAGE_INVENTORY) ? Me->pPath->yPos : NULL;
		D2NET_SendPacket(13, 1, Packet);
		delete [] Packet;
	}

	if (Location == STORAGE_STASH || Location == STORAGE_CUBE)
	{
		LPBYTE Packet = new BYTE[9];
		Packet[0] = 0x27;
		*(LPDWORD)&Packet[1] = UseItemIds(Location);
		*(LPDWORD)&Packet[5] = Unit->dwUnitId;
		D2NET_SendPacket(9, 1, Packet);
		delete [] Packet;
	}
}

VOID DropItem(DWORD dwItemID)
{
	if (GetUIVar(UI_TRADE) || !dwItemID)
		return;

	BYTE aPacket[5];
	aPacket[0] = 0x17;
	*(LPDWORD)&aPacket[1] = dwItemID;
	D2NET_SendPacket(5, 1, aPacket);
}

INT GetItemQualityFromStr(LPSTR szQuality)
{
	LPSTR szValues[9] = {NULL, "Inferior", "Normal", "Superior", "Magic", "Set", "Rare", "Unique", "Crafted"};

	for(int x = 1; x < ArraySize(szValues); x++)
		if(!_stricmp(szValues[x], szQuality))
			return x;

	return 0;
}

LPUNITANY FindEquipItem(INT nLoc)
{
	for(LPUNITANY pItem = Me->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem)
		if(GetItemLocation(pItem) == STORAGE_EQUIP)
			if(pItem->pItemData->BodyLocation == nLoc)
				return pItem;

	return NULL;
}

INT GetTotalBeltHealth()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_BELT)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 2)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalCubeHealth()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_CUBE)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 2)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalInvHealth()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_INVENTORY)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 2)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalStashHealth()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_STASH)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 2)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalBeltMana()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_BELT)
		{
			GetItemCode(Item, Code, 3);
			
			if (D2IsPotion(Code) == 3)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalCubeMana()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_CUBE)
		{
			GetItemCode(Item, Code, 3);
			
			if (D2IsPotion(Code) == 3)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalInvMana()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_INVENTORY)
		{
			GetItemCode(Item, Code, 3);
			
			if (D2IsPotion(Code) == 3)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalStashMana()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_STASH)
		{
			GetItemCode(Item, Code, 3);
			
			if (D2IsPotion(Code) == 3)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalBeltRejuvenation()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_BELT)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 1)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalCubeRejuvenation()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_CUBE)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 1)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalInvRejuvenation()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_INVENTORY)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 1)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalStashRejuvenation()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_STASH)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 1)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalBeltTPs()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_BELT)
		{
			GetItemCode(Item, Code, 3);

			if (D2IsPotion(Code) == 5)
			{
				TotalItems++;
			}
		}
	}

	return TotalItems;
}

INT GetTotalCubeTPs()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_CUBE)
		{
			GetItemCode(Item, Code, 3);

			if (!_stricmp(Code, "tsc"))
				TotalItems++;

			if (!_stricmp(Code, "tbk"))
				TotalItems = TotalItems + (INT)GetUnitStat(Item, STAT_AMMOQUANTITY);
		}
	}

	return TotalItems;
}

INT GetTotalInvTPs()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_INVENTORY)
		{
			GetItemCode(Item, Code, 3);

			if (!_stricmp(Code, "tsc"))
				TotalItems++;

			if (!_stricmp(Code, "tbk"))
				TotalItems = TotalItems + (INT)GetUnitStat(Item, STAT_AMMOQUANTITY);
		}
	}

	return TotalItems;
}

INT GetTotalStashTPs()
{
	INT TotalItems = 0;
	CHAR Code[4] = {0};

	for (LPUNITANY Item = Me->pInventory->pFirstItem; Item; Item = Item->pItemData->pNextInvItem)
	{
		if (Item && GetItemLocation(Item) == STORAGE_STASH)
		{
			GetItemCode(Item, Code, 3);

			if (!_stricmp(Code, "tsc"))
				TotalItems++;

			if (!_stricmp(Code, "tbk"))
				TotalItems = TotalItems + (INT)GetUnitStat(Item, STAT_AMMOQUANTITY);
		}
	}

	return TotalItems;
}

INT GetTotalBeltItems()
{
	INT TotalItems = 0;

	for (LPUNITANY pItem = Me->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem)
		if (pItem && GetItemLocation(pItem) == STORAGE_BELT)
			TotalItems++;

	return TotalItems;
}

BOOL IsBeltFull()
{
	CHAR szCode[4] = {0};
	LPUNITANY pBelt = FindEquipItem(EQUIP_BELT);

	if(!pBelt)
		if(GetTotalBeltItems() == 4)
			return TRUE;

	GetItemCode(pBelt, szCode, 3);
	if(GetTotalBeltItems() >= D2IsBelt(szCode) * 4)
		return TRUE;

	return FALSE;
}

INT D2IsBelt(LPSTR lpszItemCode)
{
	if(lpszItemCode == NULL)
		return 0;

	if(!_stricmp(lpszItemCode, "lbl")
		|| !_stricmp(lpszItemCode, "vbl"))
		return 2;

	else if(!_stricmp(lpszItemCode, "mbl")
		|| !_stricmp(lpszItemCode, "tbl"))
		return 3;

	else if(!_stricmp(lpszItemCode, "hbl")
		|| !_stricmp(lpszItemCode, "zlb")
		|| !_stricmp(lpszItemCode, "zvb")
		|| !_stricmp(lpszItemCode, "zmb")
		|| !_stricmp(lpszItemCode, "ztb")
		|| !_stricmp(lpszItemCode, "zhb")
		|| !_stricmp(lpszItemCode, "ulc")
		|| !_stricmp(lpszItemCode, "uvc")
		|| !_stricmp(lpszItemCode, "umc")
		|| !_stricmp(lpszItemCode, "utc")
		|| !_stricmp(lpszItemCode, "uhc"))
		return 4;

	else
		return 0;
}

INT D2IsPotion(LPSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	if (!_stricmp(lpszItemCode, "rvl")
		|| !_stricmp(lpszItemCode, "rvs"))
		return 1;

	if (!_stricmp(lpszItemCode, "hp1")
		|| !_stricmp(lpszItemCode, "hp2")
		|| !_stricmp(lpszItemCode, "hp3")
		|| !_stricmp(lpszItemCode, "hp4")
		|| !_stricmp(lpszItemCode, "hp5"))
		return 2;

	if (!_stricmp(lpszItemCode, "mp1")
		|| !_stricmp(lpszItemCode, "mp2")
		|| !_stricmp(lpszItemCode, "mp3")
		|| !_stricmp(lpszItemCode, "mp4")
		|| !_stricmp(lpszItemCode, "mp5"))
		return 3;

	if (!_stricmp(lpszItemCode, "yps")
		|| !_stricmp(lpszItemCode, "vps")
		|| !_stricmp(lpszItemCode, "wms"))
		return 4;

	if (!_stricmp(lpszItemCode, "tsc"))
		return 5;

	return 0;
}

BOOL D2IsCirclets(LPSTR lpszItemCode)
{
	return !_stricmp(lpszItemCode, "ci0")
		|| !_stricmp(lpszItemCode, "ci1")
		|| !_stricmp(lpszItemCode, "ci2")
		|| !_stricmp(lpszItemCode, "ci3");
}

BOOL D2IsGloves(LPSTR lpszItemCode)
{
	return !_stricmp(lpszItemCode, "lgl")
		|| !_stricmp(lpszItemCode, "vgl")
		|| !_stricmp(lpszItemCode, "mgl")
		|| !_stricmp(lpszItemCode, "tgl")
		|| !_stricmp(lpszItemCode, "hgl")

		|| !_stricmp(lpszItemCode, "xlg")
		|| !_stricmp(lpszItemCode, "xvg")
		|| !_stricmp(lpszItemCode, "xmg")
		|| !_stricmp(lpszItemCode, "xtg")
		|| !_stricmp(lpszItemCode, "xhg")

		|| !_stricmp(lpszItemCode, "ulg")
		|| !_stricmp(lpszItemCode, "uvg")
		|| !_stricmp(lpszItemCode, "umg")
		|| !_stricmp(lpszItemCode, "utg")
		|| !_stricmp(lpszItemCode, "uhg");
}

BOOL D2IsBoots(LPSTR lpszItemCode)
{
	return !_stricmp(lpszItemCode, "lbt")
		|| !_stricmp(lpszItemCode, "vbt")
		|| !_stricmp(lpszItemCode, "mbt")
		|| !_stricmp(lpszItemCode, "tbt")
		|| !_stricmp(lpszItemCode, "hbt")

		|| !_stricmp(lpszItemCode, "xlb")
		|| !_stricmp(lpszItemCode, "xvb")
		|| !_stricmp(lpszItemCode, "xmb")
		|| !_stricmp(lpszItemCode, "xtb")
		|| !_stricmp(lpszItemCode, "xhb")

		|| !_stricmp(lpszItemCode, "ulb")
		|| !_stricmp(lpszItemCode, "uvb")
		|| !_stricmp(lpszItemCode, "umb")
		|| !_stricmp(lpszItemCode, "utb")
		|| !_stricmp(lpszItemCode, "uhb");
}

BOOL D2IsThrowItem(LPSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !_stricmp(szItemCode, "jav")
		|| !_stricmp(szItemCode, "9ja")
		|| !_stricmp(szItemCode, "7ja")

		|| !_stricmp(szItemCode, "tax")
		|| !_stricmp(szItemCode, "9ta")
		|| !_stricmp(szItemCode, "7ta")

		|| !_stricmp(szItemCode, "tkf")
		|| !_stricmp(szItemCode, "9tk")
		|| !_stricmp(szItemCode, "7tk")

		|| !_stricmp(szItemCode, "am5")
		|| !_stricmp(szItemCode, "ama")
		|| !_stricmp(szItemCode, "amf")

		|| !_stricmp(szItemCode, "pil")
		|| !_stricmp(szItemCode, "9pi")
		|| !_stricmp(szItemCode, "7pi")

		|| !_stricmp(szItemCode, "bkf")
		|| !_stricmp(szItemCode, "9bk")
		|| !_stricmp(szItemCode, "7bk")

		|| !_stricmp(szItemCode, "bal")
		|| !_stricmp(szItemCode, "9b8")
		|| !_stricmp(szItemCode, "7b8")

		|| !_stricmp(szItemCode, "glv")
		|| !_stricmp(szItemCode, "9gl")
		|| !_stricmp(szItemCode, "7gl")

		|| !_stricmp(szItemCode, "tsp")
		|| !_stricmp(szItemCode, "9ts")
		|| !_stricmp(szItemCode, "7ts");
}

BOOL D2IsBow(LPSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !_stricmp(szItemCode, "sbw")
		|| !_stricmp(szItemCode, "hbw")
		|| !_stricmp(szItemCode, "lbw")
		|| !_stricmp(szItemCode, "cbw")
		|| !_stricmp(szItemCode, "sbb")
		|| !_stricmp(szItemCode, "lbb")
		|| !_stricmp(szItemCode, "swb")
		|| !_stricmp(szItemCode, "lwb")

		|| !_stricmp(szItemCode, "8sb")
		|| !_stricmp(szItemCode, "8hb")
		|| !_stricmp(szItemCode, "8lb")
		|| !_stricmp(szItemCode, "8cb")
		|| !_stricmp(szItemCode, "8s8")
		|| !_stricmp(szItemCode, "8l8")
		|| !_stricmp(szItemCode, "8sw")
		|| !_stricmp(szItemCode, "8lw")

		|| !_stricmp(szItemCode, "6sb")
		|| !_stricmp(szItemCode, "6hb")
		|| !_stricmp(szItemCode, "6lb")
		|| !_stricmp(szItemCode, "6cb")
		|| !_stricmp(szItemCode, "6s7")
		|| !_stricmp(szItemCode, "6l7")
		|| !_stricmp(szItemCode, "6sw")
		|| !_stricmp(szItemCode, "6lw");
}

BOOL D2IsCrossBow(LPSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !_stricmp(szItemCode, "lxb")
		|| !_stricmp(szItemCode, "mxb")
		|| !_stricmp(szItemCode, "hxb")
		|| !_stricmp(szItemCode, "rxb")

		|| !_stricmp(szItemCode, "8lx")
		|| !_stricmp(szItemCode, "8mx")
		|| !_stricmp(szItemCode, "8hx")
		|| !_stricmp(szItemCode, "8rx")

		|| !_stricmp(szItemCode, "6lx")
		|| !_stricmp(szItemCode, "6mx")
		|| !_stricmp(szItemCode, "6hx")
		|| !_stricmp(szItemCode, "6rx");
}