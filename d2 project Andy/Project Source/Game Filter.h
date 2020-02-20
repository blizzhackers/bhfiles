VOID BuildGameList(string sFilter);
VOID CreateGameList();
VOID DestroyFilterBox();
VOID STDCALL DestroyGameList(LPCONTROL pControl);
BOOL STDCALL FilterBoxInput(LPCONTROL pControl, DWORD dwLength, LPSTR pChar);
BOOL STDCALL FilterBoxReturn(WCHAR * wText);