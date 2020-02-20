VOID Game(BOOL ExitGame);
VOID GameEnd();
VOID GameStart();
VOID GameLoop();

VOID SetAutomap();
VOID SetStates();
VOID SetVars();
VOID SetDeathMessage();
VOID SetInTownVars();

VOID HostileImage();
VOID KillLog();
VOID RemainingFrames();
VOID ScreenShot();

VOID FASTCALL AssignPlayer(LPUNITANY Unit);
VOID STDCALL GameShake(LPDWORD lpX, LPDWORD lpY);
DWORD FASTCALL PlayerBlobs(LPUNITANY Unit);
BOOL FASTCALL InfraVision(LPUNITANY Unit);
VOID FASTCALL MonsterDescComma(WCHAR * String, LPMONSTERTXT Text);
WCHAR * FASTCALL MonsterLifeBarName(WCHAR * Old, LPDWORD TextSize, LPDWORD Size, DWORD Flag);
VOID FASTCALL OnGameKeyDown(BYTE Key, BYTE Repeat);
INT FASTCALL OutTownSelect(LPUNITANY Unit);

LONG STDCALL WindowProc(HWND HWnd, UINT MSG, WPARAM WParam, LPARAM LParam);