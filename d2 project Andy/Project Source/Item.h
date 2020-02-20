#ifndef _ITEM_H
#define _ITEM_H

LPUNITANY FASTCALL FindItem(DWORD dwCode, INT nLoc);
INT GetItemLocation(LPUNITANY pItem);
BOOL GetItemCode(LPUNITANY pItem, LPSTR szBuffer, DWORD dwMax);
VOID DropItem(DWORD dwItemID);
INT GetItemQualityFromStr(LPSTR szQuality);
LPUNITANY FindEquipItem(INT nLoc);
INT GetTotalBeltItems();
DWORD UseItemIds(INT nLoc);
VOID UseItem(LPUNITANY Unit);
BOOL IsBeltFull();
LPINVENTORYINFO GetInventoryInfo(INT nLoc);
INT D2IsBelt(LPSTR lpszItemCode);
INT D2IsPotion(LPSTR lpszItemCode);
INT GetTotalBeltMana();
INT GetTotalCubeMana();
INT GetTotalInvMana();
INT GetTotalStashMana();
INT GetTotalBeltHealth();
INT GetTotalCubeHealth();
INT GetTotalInvHealth();
INT GetTotalStashHealth();
INT GetTotalBeltRejuvenation();
INT GetTotalCubeRejuvenation();
INT GetTotalInvRejuvenation();
INT GetTotalStashRejuvenation();
INT GetTotalBeltTPs();
INT GetTotalCubeTPs();
INT GetTotalInvTPs();
INT GetTotalStashTPs();
BOOL D2IsCirclets(LPSTR lpszItemCode);
BOOL D2IsGloves(LPSTR lpszItemCode);
BOOL D2IsBoots(LPSTR lpszItemCode);
BOOL D2IsThrowItem(LPSTR szItemCode);
BOOL D2IsBow(LPSTR szItemCode);
BOOL D2IsCrossBow(LPSTR szItemCode);

#endif