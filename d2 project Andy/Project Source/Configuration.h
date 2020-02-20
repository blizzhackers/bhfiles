#define BOOLEAN		1
#define INTEGER		2
#define STRING		3

struct CONFIGURATION
{
	LPCSTR Section;
	LPCSTR Name;
	LPCSTR Default;
	INT Type;
	LPVOID Variable;
};

BOOL Bool(LPSTR Value);
INT Integer(LPSTR Value);
BOOL Load();
BOOL Save();
VOID GetModuleDirectory();
VOID WritePrivateProfileIntA(LPCSTR Section, LPCSTR Entry, INT Value, LPCSTR Filename);