#ifndef _ENCHANT_H
#define _ENCHANT_H

#define PLAYER		1
#define MERCENARY	2
#define ALL			3

typedef struct EnchantInfo_t
{
	INT nEnchantType;
	DWORD dwPlayerId;
} ENCHANTINFO, *LPENCHANTINFO;

VOID DestroyEnchantQueue();
BOOL ExecuteEnchantQueue(LPENCHANTINFO EnchInfo);

#endif