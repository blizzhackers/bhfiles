#ifndef _D2STRUCTS_H
#define _D2STRUCTS_H

#pragma once
#include <string>
using namespace std;

struct AttackStruct;
struct AutomapCell;
struct AutomapLayer;
struct AutomapLayer2;
struct CollMap;
struct UnitAny;
struct Room1;
struct Room2;
struct Level;
struct LevelTxt;
struct Act;
struct ActMisc;
struct RosterUnit;
struct PresetUnit;
struct OverheadMsg;
struct ObjectData;
struct ObjectTxt;
struct RoomTile;
struct ItemTxt;
struct Inventory;
struct InventoryInfo;
struct Room1;
struct Skill;
struct GameListEntry;
struct GameStructInfo;
struct CellFile;
struct Control;
struct ControlPreferences;
struct ControlText;
struct LevelNameInfo;
struct CellContext;
struct Path;
struct MonsterTxt;
struct SuperUniqueTxt;
struct TreasureClassItem;
struct TreasureClass;

typedef AttackStruct *LPATTACKSTRUCT;
typedef AutomapLayer *LPAUTOMAPLAYER;
typedef AutomapLayer2 *LPAUTOMAPLAYER2;
typedef AutomapCell *LPAUTOMAPCELL;
typedef Act *LPACT;
typedef CellFile *LPCELLFILE;
typedef CellContext *LPCELLCONTEXT;
typedef Control *LPCONTROL;
typedef ControlPreferences *LPCONTROLPREFERENCES;
typedef ControlText *LPCONTROLTEXT;
typedef PresetUnit *LPPRESETUNIT;
typedef GameListEntry *LPGAMELISTENTRY;
typedef GameStructInfo *LPGAMESTRUCTINFO;
typedef CollMap *LPCOLLMAP;
typedef ActMisc *LPACTMISC;
typedef UnitAny *LPUNITANY;
typedef ObjectData *LPOBJECTDATA;
typedef ObjectTxt *LPOBJECTTXT;
typedef ItemTxt *LPITEMTXT;
typedef Level *LPLEVEL;
typedef LevelTxt *LPLEVELTXT;
typedef LevelNameInfo *LPLEVELNAMEINFO;
typedef Inventory *LPINVENTORY;
typedef InventoryInfo *LPINVENTORYINFO;
typedef Path *LPPATH;
typedef Room1 *LPROOM1;
typedef Room2 *LPROOM2;
typedef RoomTile *LPROOMTILE;
typedef RosterUnit *LPROSTERUNIT;
typedef Skill *LPSKILL;
typedef MonsterTxt *LPMONSTERTXT;
typedef SuperUniqueTxt *LPSUPERUNIQUETXT;
typedef TreasureClassItem *LPTREASURECLASSITEM;
typedef TreasureClass *LPTREASURECLASS;

struct LevelNameInfo
{
	int nX;
	int nY;
	int nLevelId;
	int nAct;
};

struct InventoryInfo 
{
	int nLocation;
	int nMaxXCells;
	int nMaxYCells;
};

struct GameStructInfo
{
	BYTE _1[0x1B];					//0x00
	char szGameName[0x18];			//0x1B
	char szGameServerIp[0x56];		//0x33
	char szAccountName[0x30];		//0x89
	char szCharName[0x18];			//0xB9
	char szRealmName[0x18];			//0xD1
	BYTE _2[0x158];					//0xE8
	char szGamePassword[0x18];		//0x23F
};

struct AutomapCell
{
	DWORD fSaved;					//0x00
	WORD nCellNo;					//0x04
	WORD xPixel;					//0x06
	WORD yPixel;					//0x08
	WORD wWeight;					//0x0A
	AutomapCell *pLess;				//0x0C
	AutomapCell *pMore;				//0x10
};

struct GfxCell
{
	DWORD flags;					//0x00
	DWORD width;					//0x04
	DWORD height;					//0x08
	DWORD xoffs;					//0x0C
	DWORD yoffs;					//0x10
	DWORD _2;						//0x14
	DWORD lpParent;					//0x18
	DWORD length;					//0x1C
	BYTE cols;						//0x20
};


struct InteractStruct
{
	DWORD dwMoveType;			//0x00
	UnitAny* lpPlayerUnit;		//0x04
	UnitAny* lpTargetUnit;		//0x08
	DWORD dwTargetX;			//0x0C
	DWORD dwTargetY;			//0x10
	DWORD _1;					//0x14
	DWORD _2;					//0x18
};

struct CellFile
{
	DWORD dwVersion;				//0x00
	struct {
		WORD dwFlags;
		BYTE mylastcol;
		BYTE mytabno:1;
	};								//0x04
	DWORD eFormat;					//0x08
	DWORD termination;				//0x0C
	DWORD numdirs;					//0x10
	DWORD numcells;					//0x14
	GfxCell *cells[1];				//0x18
};

struct ControlPreferences
{
	DWORD dwType;//0x00
};

struct CellContext
{
	DWORD nCellNo;
	DWORD _1[12];			//0x00
	LPCELLFILE pCellFile;	//0x34
	DWORD _2[4];			//0x38
};

struct TreasureClassItem
{
	DWORD _1[2];
	DWORD thingno; //+08
	struct
	{
		DWORD _3:2;
		DWORD tcflag:1;
	}; //+0c
	DWORD _2[3];
};

struct TreasureClass
{
	DWORD _1a;
	DWORD numitems; //+04
	DWORD _1[8];
	LPTREASURECLASSITEM items; //+28
};

struct AutomapLayer
{
	DWORD nLayerNo;					//0x00
	DWORD fSaved;					//0x04
	AutomapCell *pFloors;			//0x08
	AutomapCell *pWalls;			//0x0C
	AutomapCell *pObjects;			//0x10
	AutomapCell *pExtras;			//0x14
	AutomapLayer *pNextLayer;		//0x18
};

struct AutomapLayer2
{
	DWORD _1[2];					//0x00
	DWORD nLayerNo;					//0x08
};

struct LevelTxt
{
	DWORD dwLevelNo;				//0x00
	DWORD _1[60];					//0x04
	BYTE _2;						//0xF4
	char szName[40];				//0xF5
	char szEntranceText[40];		//0x11D
	char szLevelDesc[41];			//0x145
	wchar_t wName[40];				//0x16E
	wchar_t wEntranceText[40];		//0x1BE
	BYTE nObjGroup[8];				//0x196
	BYTE nObjPrb[8];				//0x19E
};

struct ControlText
{
	wchar_t* wText; //0x00
	wchar_t* wText2;//0x04
	DWORD _1[3];	//0x08
	DWORD dwColor;	//0x14
	DWORD _2;		//0x18
	ControlText* pNext;//0x1C
};

struct Control {
   DWORD dwType;					//0x00
   CellFile *pCellFile;				//0x04
   DWORD dwDisabled;				//0x08
   DWORD dwPosX;					//0x0C
   DWORD dwPosY;					//0x10
   DWORD dwSizeX;					//0x14
   DWORD dwSizeY;					//0x18
   VOID (FASTCALL * Initialize)(LPCONTROL pControl);//0x1c
   DWORD _3;						//0x20
   DWORD *_4;						//0x24
   DWORD *_5;						//0x28
   DWORD _6;						//0x2C
   DWORD *_7;						//0x30
   DWORD *_8;						//0x34
   DWORD _9;						//0x38
   Control* pNext;					//0x3C
   DWORD _10;						//0x40
   DWORD dwMaxLength;				//0x44
   union {
	   ControlText* pFirstText;		//0x48
	   DWORD dwScrollEntries;		//0x48
   };
   union {
       ControlText* pLastText;		//0x4C
	   DWORD dwScrollPosition;		//0x4C
   };
   ControlText* pSelectedText;		//0x50
   DWORD dwSelectEnd;				//0x54
   DWORD dwSelectStart;				//0x58
   wchar_t wText[0x1A];				//0x5C
   Control* pChildControl;			//0x90
};

struct GameListEntry
{
	WORD wRequestId;
	DWORD dwIndex;
	BYTE bPlayers;
	DWORD dwStatus;
	string sGameName;
	string sGameDesc;
};

struct RoomTile
{
	Room2* pRoom2;            //0x00
	RoomTile* pNext;          //0x04
	DWORD _2[2];            //0x08
	DWORD *nNum;             //0x10 
};

struct RosterUnit
{
	char szName[16];				//0x00
	DWORD dwUnitId;					//0x10
	DWORD dwPartyLife;				//0x14
	DWORD _1;						//0x18
	DWORD dwClassId;                //0x1C
	WORD wLevel;					//0x20
	WORD wPartyId;  				//0x22
	DWORD dwLevelId;                //0x24
	DWORD Xpos;						//0x28
	DWORD Ypos;						//0x2C
	DWORD dwPartyFlags;				//0x30
	BYTE * _5;						//0x34
	DWORD _6[11];					//0x38
	WORD _7;						//0x64
	char szName2[16];				//0x66
	WORD _8;						//0x76
	DWORD _9[2];					//0x78
	RosterUnit * pNext;				//0x80
};

struct QuestInfo
{
	void *pBuffer;					//0x00
	DWORD _1;						//0x04
};

struct Waypoint
{
	BYTE flags;						//0x00
};

struct PlayerData
{
	char szName[0x10];				//0x00
	QuestInfo *pNormalQuest;		//0x10
	QuestInfo *pNightmareQuest;		//0x14
	QuestInfo *pHellQuest;			//0x18
	Waypoint *pNormalWaypoint;		//0x1c
	Waypoint *pNightmareWaypoint;	//0x20
	Waypoint *pHellWaypoint;		//0x24
};

struct CollMap
{
	DWORD dwPosGameX;				//0x00
	DWORD dwPosGameY;				//0x04
	DWORD dwSizeGameX;				//0x08
	DWORD dwSizeGameY;				//0x0C
	DWORD dwPosRoomX;				//0x10
	DWORD dwPosRoomY;				//0x14
	DWORD dwSizeRoomX;				//0x18
	DWORD dwSizeRoomY;				//0x1C
	WORD *pMapStart;				//0x20
	WORD *pMapEnd;					//0x22
};


struct PresetUnit
{
	DWORD _1;					//0x00
	DWORD dwTxtFileNo;			//0x04
	DWORD dwPosX;				//0x08
	LPPRESETUNIT pPresetNext;	//0x0C
	DWORD _2;					//0x10
	DWORD dwType;				//0x14
	DWORD dwPosY;				//0x18
};

struct Level
{
	DWORD _1[4];					//0x00
	Room2 *pRoom2First;				//0x10
	DWORD _2[2];					//0x14
	DWORD dwPosX;					//0x1C
	DWORD dwPosY;					//0x20
	DWORD dwSizeX;					//0x24
	DWORD dwSizeY;					//0x28
	BYTE _3[0x180];					//0x3C
	Level *pNextLevel;				//0x1AC
	DWORD _4;						//0x1B0
	ActMisc *pMisc;					//0x1B4
	DWORD _5[3];					//0x1B8
	DWORD dwSeed[2];				//0x1C4
	DWORD _6;						//0x1C8
	DWORD dwLevelNo;				//0x1D0
};

struct Room1
{
	Room1** pRoomsNear; 	//0x00
	DWORD _1[3];			//0x04
	Room2* pRoom2;			//0x10
	DWORD _2[3];			//0x14
	CollMap* Coll;			//0x20
	DWORD dwRoomsNear;		//0x24
	DWORD _3[9];			//0x28
	DWORD dwXStart;			//0x4C
	DWORD dwYStart;			//0x50
	DWORD dwXSize;			//0x54
	DWORD dwYSize;			//0x58
	DWORD _4[6];			//0x5C
	UnitAny* pUnitFirst;	//0x74
	DWORD _5;				//0x78
	Room1* pRoomNext;		//0x7C
};

struct Room2
{
	DWORD _1[2];			//0x00
	LPROOM2 * pRoom2Near;	//0x08
	DWORD _2[5];			//0x0C
	LPDWORD pType2Info;		//0x20
	LPROOM2 pRoom2Next;		//0x24
	DWORD dwRoomFlags;		//0x28
	DWORD dwRoomsNear;		//0x2C
	LPROOM1 pRoom1;			//0x30
	DWORD dwPosX;			//0x34
	DWORD dwPosY;			//0x38
	DWORD dwSizeX;			//0x3C
	DWORD dwSizeY;			//0x40
	DWORD _3;				//0x44
	DWORD dwPresetType;		//0x48
	LPROOMTILE pRoomTiles;	//0x4C
	DWORD _4[2];			//0x50
	LPLEVEL pLevel;			//0x58
	LPPRESETUNIT pPreset;	//0x5C
};

struct ActMisc
{
	BYTE _1[0x94];
	DWORD dwStaffTombLevel;
	BYTE _2[0x3D4];
	Act *pAct;
	DWORD _3[3];
	Level *pLevelFirst;
};

struct Act
{
	DWORD _1[3];         //0x00
	DWORD dwMapSeed;      //0x0C
	Room1* pRoom1;         //0x10
	DWORD dwAct;         //0x14
	DWORD _2[12];         //0x18
	ActMisc* pMisc;         //0x48 
};

struct Path
{
	WORD xOffset;					//0x00
	WORD xPos;						//0x02
	WORD yOffset;					//0x04
	WORD yPos;						//0x06
	DWORD _1[2];					//0x08
	WORD xTarget;					//0x10
	WORD yTarget;					//0x12
	DWORD _2[2];					//0x14
	Room1 *pRoom1;					//0x1C
	Room1 *pRoomUnk;				//0x20
	DWORD _3[3];					//0x24
	UnitAny *pUnit;					//0x30
	DWORD dwFlags;					//0x34
	DWORD _4;						//0x38
	DWORD dwPathType;				//0x3C
	DWORD dwPrevPathType;			//0x40
	DWORD dwUnitSize;				//0x44
	DWORD _5[4];					//0x48
	UnitAny *pTargetUnit;			//0x58
	DWORD dwTargetType;				//0x5C
	DWORD dwTargetId;				//0x60
	BYTE bDirection;				//0x64
};

struct Pseudo 
{ 
   CHAR Game[16];        //0x00
   CHAR Password[16];    //0x10
   BYTE _1[0x26];        //0x20
   BYTE MaxPlayers;      //0x46
};

struct ItemPath
{
	DWORD _1[3];					//0x00
	DWORD dwPosX;					//0x0C
	DWORD dwPosY;					//0x10
	//Use Path for the rest
};

struct Stat
{
	WORD wSubIndex;					//0x00
	WORD wStatIndex;				//0x02
	DWORD dwStatValue;				//0x04
};

struct StatList
{
	DWORD _1[9];					//0x00
	Stat *pStat;					//0x24
	WORD wStatCount1;				//0x28
	WORD wStatCount2;				//0x2A
	DWORD _2[2];					//0x2C
	BYTE *_3;						//0x34
	DWORD _4;						//0x38
	StatList *pNext;				//0x3C
};

struct Inventory
{
	DWORD dwSignature;				//0x00
	BYTE *bGame1C;					//0x04
	UnitAny *pOwner;				//0x08
	UnitAny *pFirstItem;			//0x0C
	UnitAny *pLastItem;				//0x10
	DWORD _1[2];					//0x14
	DWORD dwLeftItemUid;			//0x1C
	UnitAny *pCursorItem;			//0x20
	DWORD dwOwnerId;				//0x24
	DWORD dwItemCount;				//0x28
};

struct Light
{
	DWORD _1[3];					//0x00
	DWORD dwType;					//0x0C
	DWORD _2[7];					//0x10
	DWORD dwStaticValid;			//0x2C
	int *pnStaticMap;				//0x30
};

struct SkillInfo
{
	WORD wSkillId;					//0x00
};

struct Skill
{
	SkillInfo *pSkillInfo;			//0x00
	Skill *pNextSkill;				//0x04
	DWORD _1[8];					//0x08
	DWORD dwSkillLevel;				//0x28
	DWORD _2[2];					//0x2C
	DWORD dwFlags;					//0x30
};

struct Info
{
	BYTE *pGame1C;					//0x00
	
	union
	{
		Skill *pFirstSkill;				//0x04
		Skill *pStartSkill;				//0x04
	};

	Skill *pLeftSkill;				//0x08
	Skill *pRightSkill;				//0x0C
	Skill *pCurrentSkill;			//0x10
	DWORD dwUnk;					//0x14
};

struct ItemData
{
	DWORD dwQuality;				//0x00
	DWORD _1[2];					//0x04
	DWORD dwItemFlags;				//0x0C 1 = Owned by player, 0xFFFFFFFF = Not owned
	DWORD _2[2];					//0x10
	DWORD dwFlags;					//0x18
	DWORD _3[3];					//0x1C
	DWORD dwQuality2;				//0x28
	DWORD dwItemLevel;				//0x2C
	WORD _4;						//0x30
	WORD wRarePrefix;				//0x32
	WORD wRareSuffix;				//0x34
	WORD _14;
	WORD wPrefix;					//0x38
	WORD _5[2];						//0x3A
	WORD wSuffix;					//0x3E
	DWORD _6;						//0x40
	BYTE BodyLocation;				//0x44
	BYTE ItemLocation;				//0x45 Non-body/belt location (Body/Belt == 0xFF)
	BYTE _7;						//0x46
	WORD _8;						//0x47
	DWORD _9[4];					//0x48
	Inventory *pOwnerInventory;		//0x5C
	DWORD _10;						//0x60
	UnitAny *pNextInvItem;			//0x64
	BYTE _11;						//0x68
	BYTE NodePage;					//0x69 Actual location, this is the most reliable by far
	WORD _12;						//0x6A
	DWORD _13[6];					//0x6C
	UnitAny *pOwner;				//0x84
};

struct ItemTxt
{
	wchar_t szName2[0x40];			//0x00
	union
	{
		DWORD dwCode;
		char szCode[4];
	};								//0x40
	BYTE _2[0x70];					//0x84
	WORD nLocaleTxtNo;				//0xF4
	BYTE _2a[0x19];					//0xF7
	BYTE xSize;						//0xFC
	BYTE ySize;						//0xFD
	BYTE _2b[13];					//0xFE
	BYTE nType;						//0x11E
	BYTE _3[0x0d];					//0x11F
	BYTE fQuest;					//0x12A
};

struct MonsterTxt
{
	BYTE _1[0x6];					//0x00
	WORD nLocaleTxtNo;				//0x06
	WORD flag;						//0x08
	WORD _1a;						//0x0A
	union
	{
		DWORD flag1;				//0x0C
		struct
		{
			BYTE flag1a;			//0x0C
			BYTE flag1b;			//0x0D
			BYTE flag1c[2];			//0x0E
		};
	};
	BYTE _2[0x22];					//0x10
	WORD velocity;					//0x32
	BYTE _2a[0x52];					//0x34
	WORD tcs[3][4];					//0x86
	BYTE _2b[0x52];					//0x9E
	wchar_t szDescriptor[0x3c];		//0xF0
	BYTE _3[0x1a0];					//0x12C
};

struct SuperUniqueTxt
{
	WORD nIndex;
	WORD nLocaleTxtNo;
	DWORD _1[9];
	BYTE _1a[4];
	WORD tcs[4];
};

struct MonsterData
{
	BYTE _1[22];					//0x00
	struct
	{
		BYTE fUnk:1;
		BYTE fNormal:1;
		BYTE fChamp:1;
		BYTE fBoss:1;
		BYTE fMinion:1;
	};								//0x16
	WORD _2;						//0x17
	DWORD _3;						//0x18
	BYTE anEnchants[9];				//0x1C
	BYTE _4;						//0x25
	WORD wUniqueNo;					//0x26
	DWORD _5;						//0x28
	struct
	{
		wchar_t wName[28];
	};								//0x2C
};

struct ObjectTxt
{
	char szName[0x40];				//0x00
	wchar_t wszName[0x40];			//0x40
	BYTE _1[4];						//0xC0
	BYTE nSelectable0;				//0xC4
	BYTE _2[0x87];					//0xC5
	BYTE nOrientation;				//0x14C
	BYTE _2b[0x19];					//0x14D
	BYTE nSubClass;					//0x166
	BYTE _3[0x11];					//0x167
	BYTE nParm0;					//0x178
	BYTE _4[0x39];					//0x179
	BYTE nPopulateFn;				//0x1B2
	BYTE nOperateFn;				//0x1B3
	BYTE _5[8];						//0x1B4
	DWORD nAutoMap;					//0x1BB
};

struct ObjectData
{
	ObjectTxt *pTxt;				//0x00
	union
	{
		BYTE Type;					//0x04 (0x0F would be a Exp Shrine)
		struct
		{
			BYTE _1:7;
			BYTE ChestLocked:1;
		};
	};
	DWORD _2[8];					//0x08
	char szOwner[0x10];				//0x28

};

struct ObjectPath
{
	Room1 *pRoom1;					//0x00
	DWORD _1[2];					//0x04
	DWORD dwPosX;					//0x0C
	DWORD dwPosY;					//0x10
	//Leaving rest undefined, use Path
};

struct UnitAny
{
	DWORD dwType;					//0x00
	DWORD dwTxtFileNo;				//0x04
	DWORD _1;						//0x08
	DWORD dwUnitId;					//0x0C
	DWORD dwMode;					//0x10
	union
	{
		PlayerData *pPlayerData;
		ItemData *pItemData;
		MonsterData *pMonsterData;
		ObjectData *pObjectData;
		//TileData *pTileData doesn't appear to exist anymore
	};								//0x14
	DWORD dwAct;					//0x18
	Act *pAct;						//0x1C
	DWORD dwSeed[2];				//0x20
	DWORD _2;						//0x28
	union
	{
		Path *pPath;
		ItemPath *pItemPath;
		ObjectPath *pObjectPath;
	};								//0x2C
	DWORD _3[5];					//0x30
	DWORD dwGfxFrame;				//0x44
	DWORD dwFrameRemain;			//0x48
	WORD wFrameRate;				//0x4C
	WORD _4;						//0x4E
	BYTE *pGfxUnk;					//0x50
	DWORD *pGfxInfo;				//0x54
	DWORD _5;						//0x58
	StatList *pStats;				//0x5C
	Inventory *pInventory;			//0x60
	Light *ptLight;					//0x64
	DWORD _6[9];					//0x68
	WORD wX;						//0x8C
	WORD wY;						//0x8E
	DWORD _7;						//0x90
	DWORD dwOwnerType;				//0x94
	DWORD dwOwnerId;				//0x98
	DWORD _8[2];					//0x9C
	OverheadMsg* pOMsg;				//0xA4
	Info *pInfo;					//0xA8
	DWORD _9[6];					//0xAC
	DWORD dwFlags;					//0xC4
	DWORD dwFlags2;					//0xC8
	DWORD _10[5];					//0xCC
	LPUNITANY pChangedNext;			//0xE0
	LPUNITANY pRoomNext;			//0xE4
	LPUNITANY pListNext;			//0xE8 -> 0xD8
	CHAR szNameCopy[0x10];			//+0x66
};

struct BnetData
{
	DWORD dwId;					//0x00
	DWORD dwId2;				//0x04
	DWORD _1[3];				//0x08
	DWORD dwId3;				//0x14
	WORD Unk3;					//0x18
	char szGameName[22];		//0x1A
	char szGameIP[16];			//0x30
	DWORD _2[16];				//0x40
	DWORD dwId4;				//0x80
	DWORD _3;					//0x84
	char szAccountName[48];		//0x88
	char szPlayerName[24];		//0xB8
	char szRealmName[8];		//0xD0
	BYTE _4[273];				//0xD8
	BYTE nCharClass;			//0x1E9
	BYTE nCharFlags;			//0x1EA
	BYTE nMaxDiff;				//0x1EB
	BYTE _5[31];				//0x1EC
	BYTE nDifficulty;			//0x20B
	void *_6;					//0x20C
	DWORD _7[5];				//0x210
	WORD _8;					//0x224
	BYTE _9;					//0x226
	char szRealmName2[24];		//0x227
	char szGamePass[24];		//0x23F
	char szGameDesc[256];		//0x257
	WORD _10;					//0x348
	BYTE _11;					//0x34B
};


struct WardenClientRegion_t
{
	DWORD cbAllocSize; //+00
	DWORD offsetFunc1; //+04
	DWORD offsetRelocAddressTable; //+08
	DWORD nRelocCount; //+0c
	DWORD offsetWardenSetup; //+10
	DWORD _2[2];
	DWORD offsetImportAddressTable; //+1c
	DWORD nImportDllCount; //+20
	DWORD nSectionCount; //+24
};

struct SMemBlock_t
{
	DWORD _1[6];
	DWORD cbSize; //+18
	DWORD _2[31];
	BYTE data[1]; //+98
};

struct WardenClient_t
{
	WardenClientRegion_t* pWardenRegion; //+00
	DWORD cbSize; //+04
	DWORD nModuleCount; //+08
	DWORD param; //+0c
	DWORD fnSetupWarden; //+10
};

struct WardenIATInfo_t
{
	DWORD offsetModuleName;
	DWORD offsetImportTable;
};

struct AttackStruct
{
	DWORD dwAttackType;			//0x00
	UnitAny* lpPlayerUnit;		//0x04
	UnitAny* lpTargetUnit;		//0x08
	DWORD dwTargetX;			//0x0C
	DWORD dwTargetY;			//0x10
	DWORD _1;					//0x14
	DWORD _2;					//0x18
};

struct Skill_t
{
	CHAR			name[64];
	WORD			skillID;
};


/*
Akara NPCEntry

0   94 00 00 00 03 00 00 00  �.......
8   35 0D 44 0D 00 00 00 00  5.D.....
10  00 00 B0 AA B0 6F 90 BD  ..���o��
18  B0 6F 00 00 00 00 00 00  �o......
20  00 00 00 00 00 00 01     .......
*/
#pragma pack(push)
#pragma pack(1)

typedef struct
{
	DWORD dwNPCClassId;
	DWORD dwEntryAmount;
	WORD wEntryId1;
	WORD wEntryId2;
	WORD wEntryId3;
	WORD wEntryId4;	
	WORD _1;
	DWORD dwEntryFunc1;
	DWORD dwEntryFunc2; 
	DWORD dwEntryFunc3; 
	DWORD dwEntryFunc4;
	BYTE _2[5];
} NPCMenu;

struct OverheadMsg
{
	DWORD _1;
	DWORD dwTrigger;
	DWORD _2[2];
	CHAR Msg[232];
};

struct RareItemTable
{
	DWORD _1[3];		 //0x00
	WORD wLocaleStringId;//0x0C
	DWORD _2[6];		 //0x0E
	CHAR szFixName[20];	 //0x26
};

#pragma pack(pop)

struct D2MSG
{
	HWND myHWND;
	char lpBuf[256];
};


struct InventoryLayout
{
	BYTE SlotWidth;
	BYTE SlotHeight;
	BYTE unk1;
	BYTE unk2;
	DWORD Left;
	DWORD Right;
	DWORD Top;
	DWORD Bottom;
	BYTE SlotPixelWidth;
	BYTE SlotPixelHeight;
};

struct MpqTable
{
};
struct sgptDataTable
{
	MpqTable*	pPlayerClass;
	DWORD		dwPlayerClassRecords;
	MpqTable*	pBodyLocs;
	DWORD		dwBodyLocsRecords;
	MpqTable*	pStorePage;
	DWORD		dwStorePageRecords;
	MpqTable*	pElemTypes;
	DWORD		dwElemTypesRecords;
	MpqTable*	pHitClass;
	DWORD		dwHitClassRecords;
	MpqTable*	pMonMode;
	DWORD		dwMonModeRecords;
	MpqTable*	pPlrMode;
	DWORD		dwPlrModeRecords;
	MpqTable*	pSkillCalc;
	DWORD		dwSkillCalcRecords;
	MpqTable*	pSkillsCode;
	DWORD		dwSkillsCodeRecords;
	MpqTable*	pSkillsDescCode;
	DWORD		dwSkillsDescCodeRecords;
	MpqTable*	pMissCalc;
	DWORD		dwMissCalcRecords;
	MpqTable*	pMissCode;
	DWORD		dwMissCodeRecords;
	MpqTable*	pEvents;
	DWORD		dwEventsRecord;

};

/*
Layout of sgptDataTables

+0     :  ptr to playerclass.bin
+4     :  num records in playerclass.bin
+8     :  ptr to bodylocs.bin
+c     :  num records in bodylocs.bin
+10   :  ptr to storepage.bin
+14   :  num records in storepage.bin
+18   :  ptr to elemtypes.bin
+1c   :  num records in elemtypes.bin
+20   :  ptr to hitclass.bin
+24   :  num records in hitclass.bin
+28   :  ptr to monmode.bin
+2c   :  num records in monmode.bin
+30   :  ptr to plrmode.bin
+34   :  num records in plrmode.bin
+38   :  ptr to skillcalc.bin
+3c   :  num records in skillcalc.bin
+44   :  ptr to skillscode.bin
+48   :  num records in skillscode.bin
+50   :  ptr to skilldesccode.bin
+54   :  num records in skilldesccode.bin
+58   :  ptr to misscalc.bin
+5c   :  num records in misccalc.bin
+64   :  ptr to misscode.bin
+68   :  numrecords in misccode.bin
+70   :  ptr to events.bin
+74   :  numrecords in events.bin
+88   :  ptr to monai.bin
+8c    : num records in monai.bin
+A4   :  ptr to properties.bin
+Ac   :  num records in properties.bin
+b4   :  ptr to hiredesc.bin
+b8   :  num records in hiredesc.bin
+194 :  ptr to sounds.bin
+19c :  num records in sounds.bin
+1a0 :  ptr to heirling.bin
+1a4 :  num records in heirling.bin
+9a8 :  ptr to npc.txt
+9ac :  num records in npc.txt
+9b0 :  ptr to colors.bin
+9b4 :  num records in colors.bin
+a78 :  ptr to monstats.bin
+a80 :  num records in monstats.bin
+a84 :  ptr to monsounds.bin
+a8c :  num records in monsunds.bin
+a90 :  ptr to monstats2.bin
+a98 :  num records in monstats2.bin
+a9c :  ptr to monplace.bin
+aa0 :  numrecirds in monplace.bin
+aa8 :  ptr to monprest.bin
+aac :  ptr to monprest.bin (again) // check this
+ad4 :  ptr to superuniques.bin
+adc :  num records in superuniques.bin
+b64 :  missiles.bin
+b6c :  num records in missiles.bin
+b70 :  ptr to monlvl.bin
+b74 :  num records in monlvl.bin
+b78 :  ptr to monseq.bin
+b7c :  num records in monseq.bin
+b94 :  num records in skillsdesc.bin
+b98 :  ptr to skills.bin
+ba0 :  num records in skills table
+bbc :  ptr to overlay.bin
+bc0 :  num records in overlay.bin
+bc4 :  charstats table
+bcc :  ptr to itemstatcost table
+bd4 :  num records in itemstatcost
+be0 :  ptr to monequip.bin
+be4 :  num records in monequip.bin
+bf8  :  ptr to itemtypes.bin
+bfc  :  num records in itemtypes.bin
+c0c :  ptr to sets.bin
+c10 :  num records in sets.bin
+c18 :  ptr to setitems.bin
+c1c :  num records in setitems.bin
+c24 :  ptr to uniqueitems.bin
+c28 :  num records in uniqueitems.bin
+c30 :  ptr to monprop.bin
+c34 :  num records in monprop.bin
+c3c :  ptr to montype.bin
+c40 :  num records in montype.bin
+c50 :  ptr to monumod.bin
+c54 :  num records in monummod.bin
+c58 :  ptr to levels.bin
+c5c :  num records in levels.bin
+c60 :  ptr to lvldefs.bin
+c64 :  ptr to lvlPrest.bin
+c68 :  num records in lvlPrest.bin
+cb8 :  ptr to chartemplate.bin
+cbc :  num records in chartemplate.bin
+cc0 :  ptr to arena.bin
+cc4 :  ptr to lvlTypes.bin
+cd0 :  num records in lvlTypes.bin
+cd4 :  ptr to lvlwarp.bin
+cd8 :  num records in lvlwarp.bin
+cdc :  ptr to lvlmaze.bin
+ce0 :  num records in lvlmaze.bin
+ce4 :  ptr to levelsub.bin
+ce8 :  num records in levelsub.bin
+d04 :  ptr to cubemain.bin
+d08 :  num records in cubemain.bin
*/



#endif