#include "Hack.h"

LRESULT CurrentTime()
{
	if (Winamp)
		return SendMessageA(Winamp, WM_USER, 0, 105);

	return FALSE;
}

LRESULT MaxTime()
{
	if (Winamp)
		return SendMessageA(Winamp, WM_USER, 1, 105);

	return FALSE;
}

LPSTR CurrentSong()
{
	static CHAR Title[1000];

	if (!Winamp)
		return "";
	
	if (Winamp)
	{
		GetWindowTextA(Winamp, Title, sizeof(Title));

		if (strlen(Title) > 12)
		{
			* strrchr(Title, '-') = NULL;
			* strrchr(Title, ' ') = NULL;
		}

		else
			return "";
	}
	
	return Title;
}

VOID Rewind()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40061, 0);
}

VOID FastForward()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40060, 0);
}

VOID LowerVolume()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40059, 0);
}

VOID HigherVolume()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40058, 0);
}

VOID NextSong()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40048, 0);
}

VOID Stop()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40047, 0);
}

VOID Pause()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40046, 0);
}

VOID Play()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40045, 0);
}

VOID PreviousSong()
{
	if (Winamp)
		SendMessageA(Winamp, WM_COMMAND, 40044, 0);
}