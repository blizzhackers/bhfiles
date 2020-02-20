#define ArraySize(X)	(sizeof(X) / sizeof(X[0]))
#define KeyDown(Code)	((GetAsyncKeyState(Code) & 0x8000) ? TRUE : FALSE)

LPSTR AddCommas(DOUBLE f, INT places);
LPBYTE AllocReadFile(LPSTR FileName);
VOID DeleteCellFile(LPCELLFILE File);
BOOL DropGold(DWORD ID, INT Amount);
BOOL EnableDebugPrivileges();
DWORD GetExp(DWORD Level);
BOOL GetMapName(BYTE iMapID, LPSTR lpszBuffer, DWORD dwMaxChars);
INT GetPlayerClassName(INT nClass, LPSTR szBuffer, DWORD dwMaxLen);
INT GetPlayerClassNameShort(INT nClass, LPSTR szBuffer, DWORD dwMaxLen);
LPSTR GetShrinesFolder(LPSTR dest, CHAR ch);
DWORD GetTextFileNo(WCHAR * str);
DWORD GetTextWidth(WCHAR * str);
BOOL HostilePlayer(DWORD dwUnitId);
LPCELLFILE InitCellFile(LPCELLFILE File);
BOOL Interact(DWORD ID, DWORD Type, BOOL Check);
LPCELLFILE LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height);
LPCELLFILE LoadBMPCellFile(LPSTR FileName);
VOID * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count);
HANDLE OpenFileRead(LPSTR filename);
HANDLE OpenFileWrite(LPSTR filename);
DWORD ReadFile(HANDLE hFile, LPVOID buf, DWORD len);
BOOL RemovePEHeader(HMODULE DLL);
LPSTR ReplaceString(CHAR * Source, CHAR * Old, CHAR * New);
BOOL ReassignPlayer(LPUNITANY Unit, WORD X, WORD Y);
BOOL ResynchPlayer(DWORD ID, INT Type);
VOID SendClick(INT nX, INT nY);
BOOL SpoofTownPortal(BYTE State, BYTE AreaID, DWORD UnitID);
INT StringTokenize(CHAR * Input, CHAR Separator, LPSTR * TokenBuffer, INT MaxTokens);
BOOL ValidHostileMonsters(LPUNITANY Unit);
BOOL ValidMonsters(LPUNITANY Unit);
VOID wcscpy(WCHAR * dest, CHAR * src);
WCHAR * wcsrcat(WCHAR * dest, WCHAR * src);
WCHAR * FUNCCALL wsprintfW2(WCHAR * dest, LPSTR fmt, ...);
WCHAR * FUNCCALL wsprintfW2Colour(WCHAR * dest, INT col, LPSTR fmt, ...);
DWORD WriteFile(HANDLE hFile, LPVOID buf, DWORD len);