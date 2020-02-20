#define D2FONT_SMALL		6
#define D2FONT_THIN			4
#define D2FONT_NORMAL		0
#define D2FONT_SMOOTH		13
#define D2FONT_BIGSMOOTH	5
#define D2FONT_CAPS			1
#define D2FONT_ROUGH		7
#define D2FONT_BIGROUGH		2
#define D2FONT_BIGBIGROUGH	3
#define D2FONT_SMOKE		11

struct AUTOMAP
{
	LPSTR Message;
	LPSTR Data;
	INT FontColor;
	INT FontType;
};

struct STATE
{
	LPSTR Text;
	DWORD State;
	DWORD Timer;
};

class Design
{
public:
	Design();
	virtual ~Design();

	BOOL GameFilter();
	BOOL Mouse();
	BOOL Options();
	BOOL Supplies();

	VOID GameDraw();
	VOID GameDraw2();
	VOID GameDraw3();
	VOID GameDrawAutomap();
	VOID WinDraw();

	INT GetScreenSizeX();
	INT GetScreenSizeX2();
	INT GetScreenSizeY();
	INT GetScreenSizeY2();

	VOID DrawOut(INT X, INT Y, INT Color, INT Size, LPSTR Format, ...);
	BOOL DrawOutCenter(INT xPos, INT yPos, DWORD dwColor, DWORD dwSize, DWORD dwDiv, LPSTR lpFormat, ...);
	VOID TextHook(INT X, INT Y, DWORD Color, BOOL Automap, INT Font, INT Center, LPSTR Text, ...);

	BOOL ClearScreen(INT Screen);
	VOID ScreenToAutoMap(LPPOINT Point);
	VOID ScreenToAutomap(LPPOINT Position, INT X, INT Y);

	BOOL CharacterStats;
	BOOL Hide;

protected:

	BOOL AutomapInfo();
	BOOL EscapeInfo();
	BOOL ExperienceInfo();
	BOOL PotionInfo();

	BOOL Missiles();
	BOOL Monsters();

	BOOL MusicPlayer();

	BOOL Orbs();
	BOOL PlayerLifePercent();
	BOOL States();
	BOOL Stats();

	BOOL Targets();
	BOOL Vectors();

	VOID BoxHook(INT X, INT Y, INT XSize, INT YSize, DWORD Color, DWORD Transparency, BOOL Automap);
	VOID DrawBox(INT X1, INT Y1, INT X2, INT Y2, INT LineColor, INT BackGroundColor, INT Transparency);

	VOID DrawCross(INT X, INT Y, DWORD Color, BOOL Automap);

	VOID GetPlayerPosition(LPPOINT Player);
	VOID MapToScreenCoords(LPPOINT Position);
	VOID Rectangle(INT X, INT Y, INT X2, INT Y2, INT Color, INT Transparency);

	VOID ScreenToAutomapRelative(LPPOINT ptPos, INT x, INT y);
};
