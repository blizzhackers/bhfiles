#include "Hack.h"

LPSTR AddCommas(DOUBLE f, INT places)
{
	static CHAR tmp[40];
	LPSTR s;
	INT i = 0;

	if ((places < 0) || (places > 38)) 
	{
		tmp[0] = 0;
		return tmp;
	}

	sprintf_s(tmp, "%#.*f", places, f);
	s = strchr(tmp, '.');
	if (!places)
		*s = '\0';

	while (--s > tmp) 
	{
		if (i++ == 2) 
		{
			if (s[-1] == '-') break;
			i = 0;
			memmove(s + 1, s, strlen(s) + 1);
			*s = ',';
		}
	}

	return tmp;
}

LPBYTE AllocReadFile(LPSTR FileName)
{
	HANDLE hFile = OpenFileRead(FileName);
	INT FileSize = GetFileSize(hFile, 0);
	
	if (FileSize <= 0)
		return 0;

	LPBYTE Buffer = new BYTE[FileSize];
	ReadFile(hFile, Buffer, FileSize);

	CloseHandle(hFile);
	return Buffer;
}

VOID DeleteCellFile(LPCELLFILE File)
{
	if (File)
	{
		D2CMP_DeleteCellFile(File);
		delete File;
	}
}

BOOL DropGold(DWORD ID, INT Amount)
{
	if (Amount <= 0)
		return FALSE;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = 0x50;
	*(LPDWORD)&Packet[1] = ID;
	*(LPDWORD)&Packet[5] = Amount;
	D2NET_SendPacket(9, 1, Packet);
	delete [] Packet;
	return TRUE;
}

BOOL EnableDebugPrivileges()
{
	HANDLE Token;
	LUID DebugNameValue;
	TOKEN_PRIVILEGES TokenPrivilege;
	
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &Token);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &DebugNameValue);
	
	TokenPrivilege.PrivilegeCount = 1;
	TokenPrivilege.Privileges[0].Luid = DebugNameValue;
	TokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(Token, FALSE, &TokenPrivilege, sizeof(TokenPrivilege), NULL, NULL);

	CloseHandle(GetCurrentProcess());
	CloseHandle(Token);
	return TRUE;
}

DWORD GetExp(DWORD Level)
{
	INT Experience[] =
	{
		0, 500, 1500, 3750, 7875, 14175, 22680, 32886, 44396, 57715, 72144, 90180, 112725, 140906, 176132, 220165, 275207, 344008, 
		430010, 537513, 671891, 839864, 1049830, 1312287, 1640359, 2050449, 2563061, 3203826, 3902260, 4663553, 5493363, 
		6397855, 7383752, 8458379, 9629723, 10906488, 12298162, 13815086, 15468534, 17270791, 19235252, 21376515, 23710491, 
		26254525, 29027522, 32050088, 35344686, 38935798, 42850109, 47116709, 51767302, 56836449, 62361819, 68384473, 74949165, 
		82104680, 89904191, 98405658, 107672256, 117772849, 128782495, 140783010, 153863570, 168121381, 183662396, 200602101, 
		219066380, 239192444, 261129853, 285041630, 311105466, 339515048, 370481492, 404234916, 441026148, 481128591, 524840254, 
		572485967, 624419793, 681027665, 742730244, 809986056, 883294891, 963201521, 1050299747, 1145236814, 1248718217, 
		1361512946, 1484459201, 1618470619, 1764543065, 1923762030, 2097310703, 2286478756, 2492671933, 2717422497, 2962400612, 
		3229426756, 3520485254
	};
	
	return Experience[Level];
}

BOOL GetMapName(BYTE iMapID, LPSTR lpszBuffer, DWORD dwMaxChars)
{
	if(lpszBuffer == NULL)
		return FALSE;

	lpszBuffer[0] = '\0';
	::memset(lpszBuffer, 0, sizeof(TCHAR) * dwMaxChars);
	if(dwMaxChars == 0)
		return FALSE;

	switch (iMapID)
	{	
		///////////////////////////////////////////////////
		// Act 1 Maps
		///////////////////////////////////////////////////
	case MAP_A1_ROGUE_ENCAMPMENT:
		strncpy(lpszBuffer, "Rogue Encampment", dwMaxChars);
		break;

	case MAP_A1_BLOOD_MOOR:
		strncpy(lpszBuffer, "Blood Moor", dwMaxChars);
		break;

	case MAP_A1_COLD_PLAINS:
		strncpy(lpszBuffer, "Cold Plains", dwMaxChars);
		break;

	case MAP_A1_STONY_FIELD:
		strncpy(lpszBuffer, "Stony Field", dwMaxChars);
		break;

	case MAP_A1_DARK_WOOD:
		strncpy(lpszBuffer, "Dark Wood", dwMaxChars);
		break;

	case MAP_A1_BLACK_MARSH:
		strncpy(lpszBuffer, "Black Marsh", dwMaxChars);
		break;

	case MAP_A1_DEN_OF_EVIL:
		strncpy(lpszBuffer, "Den of Evil", dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_1:
		strncpy(lpszBuffer, "Cave Level 1", dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_1:
		strncpy(lpszBuffer, "Underground Passage Level 1", dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_1:
		strncpy(lpszBuffer, "Hole Level 1", dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_1:
		strncpy(lpszBuffer, "Pit Level 1", dwMaxChars);
		break;

	case MAP_A1_CAVE_LEVEL_2:
		strncpy(lpszBuffer, "Cave Level 2", dwMaxChars);
		break;

	case MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2:
		strncpy(lpszBuffer, "Underground Passage Level 2", dwMaxChars);
		break;

	case MAP_A1_HOLE_LEVEL_2:
		strncpy(lpszBuffer, "Hole Level 2", dwMaxChars);
		break;

	case MAP_A1_PIT_LEVEL_2:
		strncpy(lpszBuffer, "Pit Level 2", dwMaxChars);
		break;

	case MAP_A1_BURIAL_GROUNDS:
		strncpy(lpszBuffer, "Burial Grounds", dwMaxChars);
		break;

	case MAP_A1_CRYPT:
		strncpy(lpszBuffer, "Crypt", dwMaxChars);
		break;

	case MAP_A1_MAUSOLEUM:
		strncpy(lpszBuffer, "Mausoleum", dwMaxChars);
		break;

	case MAP_A1_FORGOTTEN_TOWER:
		strncpy(lpszBuffer, "Forgotten Tower", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_1:
		strncpy(lpszBuffer, "Tower Cellar Level 1", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_2:
		strncpy(lpszBuffer, "Tower Cellar Level 2", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_3:
		strncpy(lpszBuffer, "Tower Cellar Level 3", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_4:
		strncpy(lpszBuffer, "Tower Cellar Level 4", dwMaxChars);
		break;

	case MAP_A1_TOWER_CELLAR_LEVEL_5:
		strncpy(lpszBuffer, "Tower Cellar Level 5", dwMaxChars);
		break;

	case MAP_A1_TAMOE_HIGHLAND:
		strncpy(lpszBuffer, "Tamoe Highland", dwMaxChars);
		break;

	case MAP_A1_MONASTERY_GATE:
		strncpy(lpszBuffer, "Monastery Gate", dwMaxChars);
		break;

	case MAP_A1_OUTER_CLOISTER:
		strncpy(lpszBuffer, "Outer Cloister", dwMaxChars);
		break;

	case MAP_A1_BARRACKS:
		strncpy(lpszBuffer, "Barracks", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_1:
		strncpy(lpszBuffer, "Jail Level 1", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_2:
		strncpy(lpszBuffer, "Jail Level 2", dwMaxChars);
		break;

	case MAP_A1_JAIL_LEVEL_3:
		strncpy(lpszBuffer, "Jail Level 3", dwMaxChars);
		break;

	case MAP_A1_INNER_CLOISTER:
	case MAP_A1_INNER_CLOISTER_2:
		strncpy(lpszBuffer, "Inner Cloister", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_1:
		strncpy(lpszBuffer, "Catacombs Level 1", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_2:
		strncpy(lpszBuffer, "Catacombs Level 2", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_3:
		strncpy(lpszBuffer, "Catacombs Level 3", dwMaxChars);
		break;

	case MAP_A1_CATACOMBS_LEVEL_4:
		strncpy(lpszBuffer, "Catacombs Level 4", dwMaxChars);
		break;

	case MAP_A1_TRISTRAM:
		strncpy(lpszBuffer, "Tristram", dwMaxChars);
		break;

	case MAP_A1_THE_SECRET_COW_LEVEL:
		strncpy(lpszBuffer, "The Secret Cow Level", dwMaxChars);
		break;


		///////////////////////////////////////////////////
		// Act 2 Maps
		///////////////////////////////////////////////////
	case MAP_A2_LUT_GHOLEIN:
		strncpy(lpszBuffer, "Lut Gholein", dwMaxChars);
		break;

	case MAP_A2_ROCKY_WASTE:
		strncpy(lpszBuffer, "Rocky Waste", dwMaxChars);
		break;

	case MAP_A2_DRY_HILLS:
		strncpy(lpszBuffer, "Dry Hills", dwMaxChars);
		break;

	case MAP_A2_FAR_OASIS:
		strncpy(lpszBuffer, "Far Oasis", dwMaxChars);
		break;

	case MAP_A2_LOST_CITY:
		strncpy(lpszBuffer, "Lost City", dwMaxChars);
		break;

	case MAP_A2_VALLEY_OF_SNAKES:
		strncpy(lpszBuffer, "Valley of Snakes", dwMaxChars);
		break;

	case MAP_A2_CANYON_OF_THE_MAGI:
		strncpy(lpszBuffer, "Cayon of the Magi", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_1:
		strncpy(lpszBuffer, "Sewers Level 1", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_2:
		strncpy(lpszBuffer, "Sewers Level 2", dwMaxChars);
		break;

	case MAP_A2_SEWERS_LEVEL_3:
		strncpy(lpszBuffer, "Sewers Level 3", dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_1:
		strncpy(lpszBuffer, "Harem Level 1", dwMaxChars);
		break;

	case MAP_A2_HAREM_LEVEL_2:
		strncpy(lpszBuffer, "Harem Level 2", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_1:
		strncpy(lpszBuffer, "Palace Cellar Level 1", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_2:
		strncpy(lpszBuffer, "Palace Cellar Level 2", dwMaxChars);
		break;

	case MAP_A2_PALACE_CELLAR_LEVEL_3:
		strncpy(lpszBuffer, "Palace Cellar Level 3", dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_1:
		strncpy(lpszBuffer, "Stony Tomb Level 1", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_1:
		strncpy(lpszBuffer, "Halls of the Dead Level 1", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2:
		strncpy(lpszBuffer, "Halls of the Dead Level 2", dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_1:
		strncpy(lpszBuffer, "Claw Viper Temple Level 1", dwMaxChars);
		break;

	case MAP_A2_STONY_TOMB_LEVEL_2:
		strncpy(lpszBuffer, "Stony Tomb Level 2", dwMaxChars);
		break;

	case MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3:
		strncpy(lpszBuffer, "Halls of the Dead Level 3", dwMaxChars);
		break;

	case MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_2:
		strncpy(lpszBuffer, "Claw Viper Temple Level 2", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_1:
		strncpy(lpszBuffer, "Maggot Lair Level 1", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_2:
		strncpy(lpszBuffer, "Maggot Lair Level 2", dwMaxChars);
		break;

	case MAP_A2_MAGGOT_LAIR_LEVEL_3:
		strncpy(lpszBuffer, "Maggot Lair Level 3", dwMaxChars);
		break;

	case MAP_A2_ANCIENT_TUNNELS:
		strncpy(lpszBuffer, "Ancient Tunnels", dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_TOMB_1:
	case MAP_A2_TAL_RASHAS_TOMB_2:
	case MAP_A2_TAL_RASHAS_TOMB_3:
	case MAP_A2_TAL_RASHAS_TOMB_4:
	case MAP_A2_TAL_RASHAS_TOMB_5:
	case MAP_A2_TAL_RASHAS_TOMB_6:
	case MAP_A2_TAL_RASHAS_TOMB_7:
		strncpy(lpszBuffer, "Tal Rasha's Tomb", dwMaxChars);
		break;

	case MAP_A2_TAL_RASHAS_CHAMBER:
		strncpy(lpszBuffer, "Tal Rasha's Chamber", dwMaxChars);
		break;

	case MAP_A2_ARCANE_SANCTUARY:
		strncpy(lpszBuffer, "Arcane Sanctuary", dwMaxChars);
		break;


		///////////////////////////////////////////////////
		// Act 3 Maps
		///////////////////////////////////////////////////
	case MAP_A3_KURAST_DOCKS:
		strncpy(lpszBuffer, "Kurast Docks", dwMaxChars);
		break;

	case MAP_A3_SPIDER_FOREST:
		strncpy(lpszBuffer, "Spider Forest", dwMaxChars);
		break;

	case MAP_A3_GREAT_MARSH:
		strncpy(lpszBuffer, "Great Marsh", dwMaxChars);
		break;

	case MAP_A3_FLAYER_JUNGLE:
		strncpy(lpszBuffer, "Flayer Jungle", dwMaxChars);
		break;

	case MAP_A3_LOWER_KURAST:
		strncpy(lpszBuffer, "Lower Kurast", dwMaxChars);
		break;

	case MAP_A3_KURAST_BAZAAR:
		strncpy(lpszBuffer, "Kurast Bazaar", dwMaxChars);
		break;

	case MAP_A3_UPPER_KURAST:
		strncpy(lpszBuffer, "Upper Kurast", dwMaxChars);
		break;

	case MAP_A3_KURAST_CAUSEWAY:
		strncpy(lpszBuffer, "Kurast Causeway", dwMaxChars);
		break;

	case MAP_A3_TRAVINCAL:
		strncpy(lpszBuffer, "Travincal", dwMaxChars);
		break;

	case MAP_A3_ARCHNID_LAIR:
		strncpy(lpszBuffer, "Arachnid Lair", dwMaxChars);
		break;

	case MAP_A3_SPIDER_CAVERN:
		strncpy(lpszBuffer, "Spider Cavern", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_1:
		strncpy(lpszBuffer, "Swampy Pit Level 1", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_2:
		strncpy(lpszBuffer, "Swampy Pit Level 2", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_1:
		strncpy(lpszBuffer, "Flayer Dungeon Level 1", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_2:
		strncpy(lpszBuffer, "Flayer Dungeon Level 2", dwMaxChars);
		break;

	case MAP_A3_SWAMPY_PIT_LEVEL_3:
		strncpy(lpszBuffer, "Swampy Pit Level 3", dwMaxChars);
		break;

	case MAP_A3_FLAYER_DUNGEON_LEVEL_3:
		strncpy(lpszBuffer, "Flayer Dungeon Level 3", dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_1:
		strncpy(lpszBuffer, "Sewers Level 1", dwMaxChars);
		break;

	case MAP_A3_SEWERS_LEVEL_2:
		strncpy(lpszBuffer, "Sewers Level 2", dwMaxChars);
		break;

	case MAP_A3_RUINED_TEMPLE:
		strncpy(lpszBuffer, "Ruined Temple", dwMaxChars);
		break;

	case MAP_A3_DISUSED_FANE:
		strncpy(lpszBuffer, "Disused Fane", dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_RELIQUARY:
		strncpy(lpszBuffer, "Forgotten Reliquary", dwMaxChars);
		break;

	case MAP_A3_FORGOTTEN_TEMPLE:
		strncpy(lpszBuffer, "Forgotten Temple", dwMaxChars);
		break;

	case MAP_A3_RUINED_FANE:
		strncpy(lpszBuffer, "Ruined Fane", dwMaxChars);
		break;

	case MAP_A3_DISUSED_RELIQUARY:
		strncpy(lpszBuffer, "Disused Reliquary", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_1:
		strncpy(lpszBuffer, "Durance of Hate Level 1", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_2:
		strncpy(lpszBuffer, "Durance of Hate Level 2", dwMaxChars);
		break;

	case MAP_A3_DURANCE_OF_HATE_LEVEL_3:
		strncpy(lpszBuffer, "Durance of Hate Level 3", dwMaxChars);
		break;

		///////////////////////////////////////////////////
		// Act 4 Maps
		///////////////////////////////////////////////////
	case MAP_A4_THE_PANDEMONIUM_FORTRESS:
		strncpy(lpszBuffer, "The Pandemonium Fortress", dwMaxChars);
		break;

	case MAP_A4_OUTER_STEPPES:
		strncpy(lpszBuffer, "Outer Steppes", dwMaxChars);
		break;

	case MAP_A4_PLAINS_OF_DESPAIR:
		strncpy(lpszBuffer, "Plains of Despair", dwMaxChars);
		break;

	case MAP_A4_CITY_OF_THE_DAMNED:
		strncpy(lpszBuffer, "City of the Damned", dwMaxChars);
		break;

	case MAP_A4_RIVER_OF_FLAME:
		strncpy(lpszBuffer, "River of Flame", dwMaxChars);
		break;

	case MAP_A4_THE_CHAOS_SANCTUARY:
		strncpy(lpszBuffer, "The Chaos Sanctuary", dwMaxChars);
		break;

		///////////////////////////////////////////////////
		// Act 5 Maps
		///////////////////////////////////////////////////
	case MAP_A5_HARROGATH:
		strncpy(lpszBuffer, "Harrogath", dwMaxChars);
		break;

	case MAP_A5_THE_BLOODY_FOOTHILLS:
		strncpy(lpszBuffer, "The Bloody Foothills", dwMaxChars);
		break;

	case MAP_A5_FRIGID_HIGHLANDS:
		strncpy(lpszBuffer, "Frigid Highlands", dwMaxChars);
		break;

	case MAP_A5_ARREAT_PLATEAU:
		strncpy(lpszBuffer, "Arreat Plateau", dwMaxChars);
		break;

	case MAP_A5_CRYSTALLINE_PASSAGE:
		strncpy(lpszBuffer, "Crystalline Passage", dwMaxChars);
		break;

	case MAP_A5_FROZEN_RIVER:
		strncpy(lpszBuffer, "Frozen River", dwMaxChars);
		break;

	case MAP_A5_GLACIAL_TRAIL:
		strncpy(lpszBuffer, "Glacial Trail", dwMaxChars);
		break;

	case MAP_A5_DRIFTER_CAVERN:
		strncpy(lpszBuffer, "Drifter Cavern", dwMaxChars);
		break;

	case MAP_A5_FROZEN_TUNDRA:
		strncpy(lpszBuffer, "Frozen Tundra", dwMaxChars);
		break;

	case MAP_A5_THE_ANCIENTS_WAY:
		strncpy(lpszBuffer, "The Ancients' Way", dwMaxChars);
		break;

	case MAP_A5_ICY_CELLAR:
		strncpy(lpszBuffer, "Icy Cellar", dwMaxChars);
		break;

	case MAP_A5_ARREAT_SUMMIT:
		strncpy(lpszBuffer, "Arreat Summit", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_PAIN:
		strncpy(lpszBuffer, "Halls of Pain", dwMaxChars);
		break;

	case MAP_A5_ABADDON:
		strncpy(lpszBuffer, "Abaddon", dwMaxChars);
		break;

	case MAP_A5_PIT_OF_ACHERON:
		strncpy(lpszBuffer, "Pit of Acheron", dwMaxChars);
		break;

	case MAP_A5_INFERNAL_PIT:
		strncpy(lpszBuffer, "Infernal Pit", dwMaxChars);
		break;

	case MAP_A5_NIHLATHAKS_TEMPLE:
		strncpy(lpszBuffer, "Nihlathak's Temple", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_ANGUISH:
		strncpy(lpszBuffer, "Halls of Anguish", dwMaxChars);
		break;

	case MAP_A5_HALLS_OF_VAUGHT:
		strncpy(lpszBuffer, "Halls of Vaught", dwMaxChars);
		break;	

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_1:
		strncpy(lpszBuffer, "Worldstone Keep Level 1", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_2:
		strncpy(lpszBuffer, "Worldstone Keep Level 2", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP_LEVEL_3:
		strncpy(lpszBuffer, "Worldstone Keep Level 3", dwMaxChars);
		break;

	case MAP_A5_THRONE_OF_DESTRUCTION:
		strncpy(lpszBuffer, "Throne of Destruction", dwMaxChars);
		break;

	case MAP_A5_WORLDSTONE_KEEP:
		strncpy(lpszBuffer, "The Worldstone Chamber", dwMaxChars);
		break;

	default:
		return 0;
		break;
	}

	return FALSE;
}

INT GetPlayerClassName(INT nClass, LPSTR szBuffer, DWORD dwMaxLen)
{
	switch(nClass)
	{
	case 0:
		sprintf(szBuffer, "Amazon");
		break;
	case 1:
		sprintf(szBuffer, "Sorceress");
		break;
	case 2:
		sprintf(szBuffer, "Necromancer");
		break;
	case 3:
		sprintf(szBuffer, "Paladin");
		break;
	case 4:
		sprintf(szBuffer, "Barbarian");
		break;
	case 5:
		sprintf(szBuffer, "Druid");
		break;
	case 6:
		sprintf(szBuffer, "Assassin");
		break;
	}

	return (INT)strlen(szBuffer);
}

INT GetPlayerClassNameShort(INT nClass, LPSTR szBuffer, DWORD dwMaxLen)
{
	switch(nClass)
	{
	case 0:
		sprintf(szBuffer, "Ama");
		break;
	case 1:
		sprintf(szBuffer, "Sor");
		break;
	case 2:
		sprintf(szBuffer, "Nec");
		break;
	case 3:
		sprintf(szBuffer, "Pal");
		break;
	case 4:
		sprintf(szBuffer, "Bar");
		break;
	case 5:
		sprintf(szBuffer, "Dru");
		break;
	case 6:
		sprintf(szBuffer, "Asn");
		break;
	}

	return (INT)strlen(szBuffer);
}

LPSTR GetShrinesFolder(LPSTR dest, CHAR ch)
{
	GetModuleFileNameA(V_DLL, dest, MAX_PATH);
	PathRemoveFileSpecA(dest);
	strcat(dest, "\\Shrines\\");
	return strrchr(dest, ch) + 1;
}

DWORD GetTextFileNo(WCHAR * str)
{
	DWORD width, fileno;
	D2WIN_GetTextWidthFileNo(str, &width, &fileno);
	return fileno;
}

DWORD GetTextWidth(WCHAR * str)
{
	DWORD width, fileno;
	D2WIN_GetTextWidthFileNo(str, &width, &fileno);
	return width;
}

BOOL HostilePlayer(DWORD dwUnitId)
{
	if (!ClientReady(TRUE))
		return FALSE;

	DWORD dwFlags = GetPvPFlags(dwUnitId);

	if (dwFlags & PVP_HOSTILED_BY_YOU || dwFlags & PVP_HOSTILED_YOU)
		return FALSE;

	if (!FindPartyById(dwUnitId))
		return FALSE;

	LPBYTE Packet = new BYTE[7];
	Packet[0] = 0x5D;
	*(LPWORD)&Packet[1] = 0x104;
	*(LPDWORD)&Packet[3] = dwUnitId;
	D2NET_SendPacket(7, 1, Packet);
	delete [] Packet;
	return TRUE;
}

LPCELLFILE InitCellFile(LPCELLFILE File)
{
	if (File) D2CMP_InitCellFile(File, &File, "?", 0, -1, "?");
	return File;
}

BOOL Interact(DWORD ID, DWORD Type, BOOL Check)
{
	if (!ClientReady(TRUE))
		return FALSE;

	if (Check && !GetUnit(ID, Type))
		return FALSE;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = 0x13;
	*(LPDWORD)&Packet[1] = Type;
	*(LPDWORD)&Packet[5] = ID;
	D2NET_SendPacket(9, 1, Packet);
	delete [] Packet;
	return TRUE;
}

LPCELLFILE LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height)
{
	/*BYTE *buf2 = new BYTE[(Width*Height*2)+Height], *dest = buf2;

	for (int i = 0; i < Height; i++) {
		BYTE *src = Buffer+(i*((Width+3)&-4)), *limit = src+Width;
		while (src < limit) {
			BYTE *start = src, *limit2 = min(limit, src+0x7f), trans = !*src;
			do src++; while ((trans == (BYTE)!*src) && (src < limit2));
			if (!trans || (src < limit)) *dest++ = (trans?0x80:0)+(src-start);
			if (!trans) while (start < src) *dest++ = *start++;
		}
		*dest++ = 0x80;
	}

	static DWORD dc6head[] = {6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c,  0, -1, -1, 0, 0, 0, -1, -1};
	dc6head[8] = Width;
	dc6head[9] = Height;
	BYTE *ret = new BYTE[dc6head[13] = sizeof(dc6head)+(dc6head[14] = dest-buf2)+3];
	memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3);
	delete buf2;

	return (CellFile *)ret;*/
	BYTE * Buffer2 = new BYTE[(Width * Height * 2) + Height], *Destination = Buffer2;

	for (INT i = 0; i < Height; i++)
	{
		BYTE * Source = Buffer + (i * ((Width + 3) & -4)), *Limit = Source + Width;
		
		while (Source < Limit)
		{
			BYTE * Start = Source, *Limit2 = min(Limit, Source + 0x7F), Transparency = !*Source;
			do Source++; while ((Transparency == (BYTE)!*Source) && (Source < Limit2));

			if (!Transparency || (Source < Limit)) *Destination++ = (BYTE)((Transparency ? 0x80 : 0) + (Source - Start));
			if (!Transparency) while (Start < Source) *Destination++ = *Start++;
		}
		
		*Destination++ = 0x80;
	}

	DWORD DC6[] = {6, 1, 0, 0xEEEEEEEE, 1, 1, 0x1C,  0, -1, -1, 0, 0, 0, -1, -1};
	DC6[8] = Width;
	DC6[9] = Height;

	BYTE * Return = new BYTE[DC6[13] = sizeof(DC6) + (DC6[14] = (DWORD)(Destination - Buffer2)) + 3];
	memset(memcpy2(memcpy2(Return, DC6, sizeof(DC6)), Buffer2, DC6[14]), 0xEE, 3);
	delete Buffer2;

	return (LPCELLFILE)Return;
}

LPCELLFILE LoadBMPCellFile(LPSTR FileName)
{
	/*BYTE *ret = 0;
		
	BYTE *buf1 = AllocReadFile(FileName);
	BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1;
	BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1+sizeof(BITMAPFILEHEADER));
	if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) {
		ret = (BYTE *)LoadBMPCellFile(buf1+bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight);
	}
	delete buf1;

	return (CellFile *)ret;*/
	LPBYTE Return = 0;
	LPBYTE Buffer = AllocReadFile(FileName);
	
	BITMAPFILEHEADER * BMP1 = (BITMAPFILEHEADER *)Buffer;
	BITMAPINFOHEADER * BMP2 = (BITMAPINFOHEADER *)(Buffer + sizeof(BITMAPFILEHEADER));
	
	if (Buffer && (BMP1->bfType == 'MB') && (BMP2->biBitCount == 8) && (BMP2->biCompression == BI_RGB))
		Return = (LPBYTE)LoadBMPCellFile(Buffer + BMP1->bfOffBits, BMP2->biWidth, BMP2->biHeight);

	delete Buffer;
	return (LPCELLFILE)Return;
}

VOID * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count)
{
	return (LPSTR)memcpy(Destination, Source, Count) + Count;
}

HANDLE OpenFileRead(LPSTR filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

HANDLE OpenFileWrite(LPSTR filename)
{
	return CreateFile(filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

DWORD ReadFile(HANDLE hFile, LPVOID buf, DWORD len)
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

BOOL RemovePEHeader(HMODULE DLL)
{
	PIMAGE_DOS_HEADER DOSHD = (PIMAGE_DOS_HEADER)(DLL);
	PIMAGE_NT_HEADERS NTHD;	

	if (DOSHD->e_magic == IMAGE_DOS_SIGNATURE)
	{
		NTHD = (PIMAGE_NT_HEADERS)((DWORD)DOSHD + DOSHD->e_lfanew);

		if (NTHD->Signature == IMAGE_NT_SIGNATURE)
		{
			DWORD OldProtect;
			VirtualProtect((LPVOID)NTHD, sizeof(IMAGE_NT_HEADERS), PAGE_READWRITE, &OldProtect);
			::memset(NTHD, NULL, sizeof(IMAGE_NT_HEADERS));

			VirtualProtect((LPVOID)NTHD, sizeof(IMAGE_NT_HEADERS), OldProtect, &OldProtect);
			VirtualProtect((LPVOID)DOSHD, sizeof(IMAGE_DOS_HEADER), PAGE_READWRITE, &OldProtect);

			::memset(DOSHD, NULL, sizeof(IMAGE_DOS_HEADER));
			VirtualProtect((LPVOID)DOSHD, sizeof(IMAGE_DOS_HEADER), OldProtect, &OldProtect);
			return TRUE;
		}
	}
	
	return FALSE;
}

LPSTR ReplaceString(CHAR * Source, CHAR * Old, CHAR * New)
{
	CHAR * Original = new CHAR[strlen(Source)];
	CHAR * Temporary = new CHAR[500];
	INT OldLength = (INT)strlen(Old);
	INT i, j, k, Location = -1;
	strcpy(Original, Source);
	
	for (i = 0; Source[i] && Location == -1; ++i)
		for (j = i, k = 0; Source[j] == Old[k]; j++, k++)
			if (!Old[k + 1])
				Location = i;
	
	if (Location != -1)
	{
		for (j = 0; j < Location; j++)
			Temporary[j] = Source[j];
		
		for (i = 0; New[i]; i++, j++)
			Temporary[j] = New[i];

		for (k = Location + OldLength; Source[k]; k++, j++)
			Temporary[j] = Source[k];
		
		Temporary[j] = NULL;
		
		for (i = 0; Source[i] = Temporary[i]; i++);
	}
	
	delete Original;
	return Temporary;
}

BOOL ReassignPlayer(LPUNITANY Unit, WORD X, WORD Y)
{
	if (!GetUnit(Unit->dwUnitId, Unit->dwType))
		return FALSE;

	LPBYTE Packet = new BYTE[11];
	Packet[0] = 0x15;
	*(LPDWORD)&Packet[1] = Unit->dwType;
	*(LPDWORD)&Packet[2] = Unit->dwUnitId;
	*(LPWORD)&Packet[6] = X;
	*(LPWORD)&Packet[8] = Y;
	D2NET_ReceivePacket(Packet, 11);
	delete [] Packet;
	return TRUE;
}

BOOL ResynchPlayer(DWORD ID, INT Type)
{
	if (!GetUnit(ID, Type))
		return FALSE;

	LPBYTE Packet = new BYTE[9];
	Packet[0] = 0x4B;
	*(LPDWORD)&Packet[1] = Type;
	*(LPDWORD)&Packet[5] = ID;
	D2NET_SendPacket(9, 1, Packet);
	delete [] Packet;
	return TRUE;
}

VOID SendClick(INT nX, INT nY)
{
	LPARAM lPos = ((SHORT)nY << 16) + (SHORT)nX;
	HWND hD2 = D2GFX_GetHwnd();
	HWND hFg = GetForegroundWindow();
	POINT ptPos;

	GetCursorPos(&ptPos);
	SetForegroundWindow(hD2);
	PostMessage(hD2, WM_MOUSEMOVE, NULL, lPos);
	PostMessage(hD2, WM_LBUTTONDOWN, MK_LBUTTON, lPos);
	PostMessage(hD2, WM_LBUTTONUP, MK_LBUTTON, lPos);
	SetCursorPos(ptPos.x, ptPos.y);
	SetForegroundWindow(hFg);
}

BOOL SpoofTownPortal(BYTE State, BYTE AreaID, DWORD UnitID)
{
	LPBYTE Packet = new BYTE[7];
	Packet[0] = 0x60;
	Packet[1] = State;
	Packet[2] = AreaID;
	*(LPDWORD)&Packet[3] = UnitID;
	D2NET_ReceivePacket(Packet, 7);
	delete [] Packet;
	return TRUE;
}

INT StringTokenize(CHAR * Input, CHAR Separator, LPSTR * TokenBuffer, INT MaxTokens)
{
	CHAR * P = Input;
	INT i = 0;
	
	do
	{
		TokenBuffer[i] = P;
		P = strchr(P, Separator);
		
		if (P)
			*(P++) = 0;
	}
	
	while (P && ++i < MaxTokens);
	return ++i;
}

BOOL ValidHostileMonsters(LPUNITANY Unit)
{
	if (!Unit)
		return FALSE;

	if (Unit->dwMode == NPC_MODE_DEATH || Unit->dwMode == NPC_MODE_DEAD)
		return FALSE;

	if (Unit->dwTxtFileNo >= 110 && Unit->dwTxtFileNo <= 113 || Unit->dwTxtFileNo == 608 && Unit->dwMode == NPC_MODE_USESKILL1)
		return FALSE;
	
	if (Unit->dwTxtFileNo == 68 && Unit->dwMode == NPC_MODE_SEQUENCE)
		return FALSE;

	if ((Unit->dwTxtFileNo == 258 || Unit->dwTxtFileNo == 261) && Unit->dwMode == NPC_MODE_SEQUENCE)
		return FALSE;

	if ((Unit->dwTxtFileNo == 356 || Unit->dwTxtFileNo == 357 || Unit->dwTxtFileNo == 424 || Unit->dwTxtFileNo == 425 ||
		Unit->dwTxtFileNo == 418 || Unit->dwTxtFileNo == 419 || Unit->dwTxtFileNo == 421))
		return FALSE;
	
	DWORD Bad[] =
	{
		146, 147, 148, 150, 154, 155, 175, 176, 177, 178, 198, 199, 200, 201, 202, 210, 244, 245, 246, 251, 252, 253, 254, 255, 257, 264,
		265, 266, 270, 283, 297, 326, 327, 328, 329, 330, 331, 338, 351, 352, 353, 359, 366, 367, 405, 406, 408, 410, 411, 412, 413, 414,
		415, 416, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 528, 535, 542, 543
	};

	for (DWORD i = 0; i < ArraySize(Bad) - 1; i++)
		if (Unit->dwTxtFileNo == Bad[i])
			return FALSE;

	WCHAR * Name1 = (WCHAR *)GetUnitName(Unit);
	CHAR MonsterName[50];

	WideCharToMultiByte(CP_ACP, 0, Name1, -1, MonsterName, (INT)sizeof(MonsterName), 0, 0);

	if ((strcmp(MonsterName, "an evil force") == 0) || (strcmp(MonsterName, "dummy") == 0))
		return FALSE;

	return TRUE;
}

BOOL ValidMonsters(LPUNITANY Unit)
{
	if (!Unit)
		return FALSE;

	if (Unit->dwMode == NPC_MODE_DEATH || Unit->dwMode == NPC_MODE_DEAD)
		return FALSE;

	if (Unit->dwTxtFileNo >= 110 && Unit->dwTxtFileNo <= 113 || Unit->dwTxtFileNo == 608 && Unit->dwMode == NPC_MODE_USESKILL1)
		return FALSE;
	
	if (Unit->dwTxtFileNo == 68 && Unit->dwMode == NPC_MODE_SEQUENCE)
		return FALSE;

	if ((Unit->dwTxtFileNo == 258 || Unit->dwTxtFileNo == 261) && Unit->dwMode == NPC_MODE_SEQUENCE)
		return FALSE;

	if ((Unit->dwTxtFileNo == 356 || Unit->dwTxtFileNo == 357 || Unit->dwTxtFileNo == 424 || Unit->dwTxtFileNo == 425 ||
		Unit->dwTxtFileNo == 418 || Unit->dwTxtFileNo == 419 || Unit->dwTxtFileNo == 421) && Unit->dwOwnerId == Me->dwUnitId)
		return FALSE;
	
	DWORD Bad[] =
	{
		146, 147, 148, 150, 154, 155, 175, 176, 177, 178, 198, 199, 200, 201, 202, 210, 244, 245, 246, 251, 252, 253, 254, 255, 257, 264,
		265, 266, 270, 283, 297, 326, 327, 328, 329, 330, 331, 338, 351, 352, 353, 359, 366, 367, 405, 406, 408, 410, 411, 412, 413, 414,
		415, 416, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 528, 535, 542, 543
	};

	for (DWORD i = 0; i < ArraySize(Bad) - 1; i++)
		if (Unit->dwTxtFileNo == Bad[i])
			return FALSE;

	WCHAR * Name = (WCHAR *)GetUnitName(Unit);
	CHAR MonsterName[50];

	WideCharToMultiByte(CP_ACP, 0, Name, -1, MonsterName, (INT)sizeof(MonsterName), 0, 0);

	if (!strcmp(MonsterName, "an evil force") || !strcmp(MonsterName, "dummy"))
		return FALSE;

	return TRUE;
}

VOID wcscpy(WCHAR * dest, CHAR * src)
{
	do *dest++ = *src;
	while (*src++);
}

WCHAR * wcsrcat(WCHAR * dest, WCHAR * src)
{
	memmove(dest + wcslen(src), dest, (wcslen(dest) + 1) * sizeof(WCHAR));
	memcpy(dest, src, wcslen(src) * sizeof(WCHAR));
	return dest;
}

WCHAR * FUNCCALL wsprintfW2(WCHAR * dest, LPSTR fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	INT len = wvsprintf((LPSTR)dest, fmt, va);

	for (INT i = len; i >= 0; i--)
		dest[i] = ((LPSTR)dest)[i];

	return dest;
}

WCHAR * FUNCCALL wsprintfW2Colour(WCHAR * dest, INT col, LPSTR fmt, ...)
{
	dest[0] = 0xff;
	dest[1] = 'c';
	dest[2] = '0'+(int)col;
	dest += 3;

	va_list va;
	va_start(va, fmt);

	INT len = wvsprintf((LPSTR)dest, fmt, va);

	for (INT i = len; i >= 0; i--)
		dest[i] = ((LPSTR)dest)[i];

	return dest;
}

DWORD WriteFile(HANDLE hFile, LPVOID buf, DWORD len)
{
	DWORD numdone = 0;
	WriteFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}