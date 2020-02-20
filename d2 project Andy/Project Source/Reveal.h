#include "Matrix.h"

#define MAP_DATA_NULL		-1
#define MAP_DATA_AVOID		11115
#define MAP_DATA_FILL		11111

#define MYCELL_CHUNK_NUM 15

typedef struct TCTallys
{
	INT groupnos[5];
	INT runeno;
} TCTALLYS, *LPTCTALLYS;

typedef struct MyAutomapCell
{
	WORD nCellNo;
	WORD xPixel;
	WORD yPixel;
} MYAUTOMAPCELL, *LPMYAUTOMAPCELL;

typedef struct MyAutomapLayer
{
	MyAutomapCell aMyCell[MYCELL_CHUNK_NUM];
	MyAutomapLayer *pNextChunk;
} MYAUTOMAPLAYER, *LPMYAUTOMAPLAYER;

typedef struct MyAutomapLayerMan
{
	MyAutomapLayer *pMyLayer;
	WORD wCellChunkNum;
	WORD wCellNodeNum;
} MYAUTOMAPLAYERMAN, *LPMYAUTOMAPLAYERMAN;

typedef struct CaveDescriptor_t
{
	CHAR szName[0x40];
	POINT ptPos;
	DWORD dwLevelNo;
	DWORD dwTargetLevelNo;
	DWORD dwAct;
} CAVEDESC, *LPCAVEDESC;

typedef struct LevelExit_t
{
	POINT ptPos;
	DWORD dwTargetLevel;
} LEVELEXIT, *LPLEVELEXIT;

class Reveal
{
public:
	Reveal();
	virtual ~Reveal();

	VOID RevealAutomap();
	BOOL CreateCollisionMap();
	LPLEVEL GetLevel(LPACTMISC misc, DWORD levelno);
	BOOL GetCaveExits(LPCAVEDESC *lpLevel, INT nMaxExits);
	BOOL GetLevelExits(LPLEVELEXIT *lpLevel, INT nMaxExits);
	VOID GetHighestTC(INT tcno, LPTCTALLYS tallys);
	VOID DestroyLevelExits();

	VOID AddMyAutomapCell(LPAUTOMAPCELL cell);
	VOID DestroyMyAutomapCells();
	VOID DoRemoveMyAutomapCell(AutomapCell* cell);
	VOID NextRemoveMyAutomapCell(AutomapCell* cell);
	VOID RemoveMyAutomapCells();

	VOID DeleteShrines();
	VOID InitShrines();
	VOID InitTCList();

	VOID STDCALL FilterOutMyAutomapCells(LPAUTOMAPCELL Cell);
	LPMYAUTOMAPCELL GetMyAutomapCell(INT Index);

	VOID LoadMyAutomapLayer();

	WORD GetCollisionInfo(INT nX, INT nY);
	BOOL ExportCollisionMap(CMatrix<WORD, WORD>& cMatrix);

	DWORD GetTileLevelNo(LPROOM2 lpRoom2, DWORD dwTileNo);

	CArrayEx<LPCAVEDESC, LPCAVEDESC> m_LevelExits;

	UCHAR m_ActLevels[6];
	UCHAR m_RevealedActs[5];

protected:
	VOID AddAutomapRoom(LPROOM2 pRoom2);
	VOID RemoveAutomapRoom(LPROOM2 pRoom2);

	VOID RevealLevel(LPLEVEL pLevel);
	VOID RevealRoom(LPROOM2 pRoom);
	VOID RevealRoom1(LPROOM2 pRoom);
	VOID AddRoomCell(INT xPos, INT yPos, INT nCell, LPROOM2 pRoom);

	INT GetUnitCellNumber(DWORD dwClassId, DWORD dwLevelNo);
	LPAUTOMAPLAYER2 InitAutomapLayer(DWORD dwLayer);

	BOOL AddCollisionData(LPROOM2 pRoom2, CArrayEx<DWORD, DWORD>& aSkip);
	BOOL AddCollisionData(LPCOLLMAP pColl);
	BOOL FillGaps();
	BOOL IsGap(INT nX, INT nY);

	INT m_LastLevel;
	INT m_SizeX;
	INT m_SizeY;
	POINT m_LevelOrigin;
	CMatrix<WORD, WORD> m_Map;
};

VOID STDCALL AutomapCells(LPCELLCONTEXT Context, DWORD X, DWORD Y, RECT * Clip, DWORD Bright);
VOID GetMonsterLevelTC(WCHAR * String, LPUNITANY Unit, DWORD Number);
VOID GetMonsterResists(WCHAR * String, LPUNITANY Unit, DWORD Number);
LPUNITANY GetViewingUnit();
VOID FASTCALL OverrideShrine(LPAUTOMAPCELL New, LPAUTOMAPCELL Current);
VOID FASTCALL ItemName(WCHAR * Name, LPUNITANY Item);
VOID FASTCALL SocketViewtableCallRoutine(LPUNITANY Unit, DWORD Argument, VOID (STDCALL * Function)(LPUNITANY, DWORD));
VOID STDCALL SocketViewtable(VOID (STDCALL * Function)(LPUNITANY, DWORD), LPUNITANY Unit, DWORD Argument);
LPUNITANY ViewInventory();
BOOL ViewInventorySocketProtection();