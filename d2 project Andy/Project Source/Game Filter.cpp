#include "Hack.h"

VOID BuildGameList(string sFilter)
{
	if (!V_GameList.empty() && (*p_D2MULTI_GameListControl))
	{
		LPCONTROLTEXT pText = (*p_D2MULTI_GameListControl)->pFirstText;

		(*p_D2MULTI_GameListControl)->pFirstText = NULL;
		(*p_D2MULTI_GameListControl)->pLastText = NULL;
		(*p_D2MULTI_GameListControl)->pSelectedText = NULL;
		(*p_D2MULTI_GameListControl)->dwSelectEnd = 0;
		(*p_D2MULTI_GameListControl)->dwSelectStart = 0;
		(*p_D2MULTI_GameListControl)->dwMaxLength = 0;

		(*p_D2MULTI_GameListControl)->pChildControl->dwMaxLength = 0;
		(*p_D2MULTI_GameListControl)->pChildControl->dwScrollPosition = 0;
		(*p_D2MULTI_GameListControl)->pChildControl->dwScrollEntries = 0;

		while (pText)
		{
			LPCONTROLTEXT pNext = pText->pNext;
			
			delete[] pText->wText;
			delete[] pText->wText2;
			delete pText;

			pText = pNext;
		}

		for (list<LPGAMELISTENTRY>::iterator ListEntry = V_GameList.begin(); ListEntry != V_GameList.end(); ListEntry++)
		{
			string sGameName((*ListEntry)->sGameName);

			for (UINT i = 0; i < sGameName.length(); i++)
				sGameName[i] = toupper(sGameName[i]);

			if (sGameName.find(sFilter) != string::npos)
			{
				LPCONTROLTEXT pText = new ControlText;
				memset(pText, NULL, sizeof(ControlText));

				pText->dwColor = 4;
				pText->wText = new WCHAR[64];
				pText->wText2 = new WCHAR[12];

				memset(pText->wText, NULL, 64);
				memset(pText->wText2, NULL, 12);

				wstring wGameName((*ListEntry)->sGameName.begin(), (*ListEntry)->sGameName.end());

				wcscpy_s(pText->wText, 64, wGameName.c_str());
				pText->wText2[0] = (*ListEntry)->bPlayers + 0x30;

				if ((*p_D2MULTI_GameListControl)->pFirstText)
				{
					(*p_D2MULTI_GameListControl)->pLastText->pNext = pText;
					(*p_D2MULTI_GameListControl)->pLastText = pText;
				}
				
				else
				{
					(*p_D2MULTI_GameListControl)->pFirstText = pText;	
					(*p_D2MULTI_GameListControl)->pLastText = pText;
					(*p_D2MULTI_GameListControl)->pSelectedText = pText;
				}

				(*p_D2MULTI_GameListControl)->dwSelectEnd += 1;
				(*p_D2MULTI_GameListControl)->pChildControl->dwScrollEntries = (*p_D2MULTI_GameListControl)->dwSelectEnd > 10 ? (*p_D2MULTI_GameListControl)->dwSelectEnd - 10 : 0;
			}
		}
	}
}

VOID CreateGameList()
{
	if (!V_GameList.empty())
	{
		for (list<LPGAMELISTENTRY>::iterator ListEntry = V_GameList.begin(); ListEntry != V_GameList.end(); ListEntry++)
			delete (*ListEntry);

		V_GameList.clear();
	}

	if (V_GameFilter && !V_FilterBox)
	{
		V_FilterBox = D2WIN_CreateEditBox(599, 185, 145, 41, 7, 0, 0, (BOOL (STDCALL *)(LPCONTROL))FilterBoxReturn, NULL, NULL, (LPCONTROLPREFERENCES)p_D2MULTI_EditboxPreferences);
		V_FilterBox->pCellFile = (LPCELLFILE)LoadUIImage("DATA\\GLOBAL\\UI\\FrontEnd\\TextBox2");
		D2WIN_SetEditBoxCallback(V_FilterBox, FilterBoxInput);
		V_FilterBox->dwMaxLength = 0x12;
		V_FilterBox->Initialize(V_FilterBox);
	}
}

VOID DestroyFilterBox()
{
	if (V_FilterBox)
	{
		if (!V_GameList.empty())
		{
			for (list<LPGAMELISTENTRY>::iterator ListEntry = V_GameList.begin(); ListEntry != V_GameList.end(); ListEntry++)
				delete (*ListEntry);

			V_GameList.clear();
		}

		D2WIN_DestroyEditBox(V_FilterBox);
		V_FilterBox = NULL;
	}
}

VOID STDCALL DestroyGameList(LPCONTROL pControl)
{
	if (pControl)
	{
		if(V_FilterBox)
		{
			if (!V_GameList.empty())
			{
				for (list<LPGAMELISTENTRY>::iterator ListEntry = V_GameList.begin(); ListEntry != V_GameList.end(); ListEntry++)
					delete (*ListEntry);

				V_GameList.clear();
			}

			D2WIN_DestroyEditBox(V_FilterBox);
			V_FilterBox = NULL;
		}

		D2WIN_DestroyControl(pControl);
	}
}

BOOL STDCALL FilterBoxInput(LPCONTROL pControl, DWORD dwLength, LPSTR pChar)
{
	wstring wInput(pControl->wText);

	string sFilter(wInput.begin(), wInput.end());

	if (dwLength > 0)
		sFilter += *pChar;

	for (UINT i = 0; i < sFilter.length(); i++)
		sFilter[i] = toupper(sFilter[i]);

	BuildGameList(sFilter);
	return TRUE;
}

BOOL STDCALL FilterBoxReturn(WCHAR * wText)
{
	return TRUE;
}