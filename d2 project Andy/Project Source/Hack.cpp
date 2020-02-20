#define PTRS
#define VARS

#include "Hack.h"

BOOL WINAPI DllMain(HINSTANCE DLL, DWORD Reason, LPVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
		return Attach(DLL);

	else if (Reason == DLL_PROCESS_DETACH)
		return Detach();

	return TRUE;
}

BOOL Attach(HINSTANCE DLL)
{
	V_Name = "Project";
	V_DLL = DLL;
	V_Version = 1.3f;

	DefineOffsets();
	Load();

	if (D2GFX_GetHwnd())
	{
		V_OldWndProc = (WNDPROC)GetWindowLongA(D2GFX_GetHwnd(), GWL_WNDPROC);
		SetWindowLongA(D2GFX_GetHwnd(), GWL_WNDPROC, (LONG)WindowProc);
	}

	if (ClientReady(FALSE))
	{
		V_Design = new Design;
		V_Reveal = new Reveal;

		InstallPatches(INGAME);
		V_PatchInGame = TRUE;
	}

	else
	{
		if (RemovePEHeader(DLL))
		{
			InstallPatches(OUTGAME);
			V_PatchOutGame = TRUE;
			V_PatchInGame = -1;
		}

		else
			TerminateProcess(GetCurrentProcess(), NULL);
	}

	V_PrimaryThread.Handle = MakeThread((LPVOID)Threads, (LPVOID)PRIMARY);

	return TRUE;
}

BOOL Detach()
{
	DestroyThread(V_PrimaryThread.Handle, PRIMARY, INFINITE);
	DestroyInGameThreads();

	if (ClientReady(FALSE))
		RemovePatches(INGAME);
	else if (!ClientReady(FALSE))
		RemovePatches(OUTGAME);

	if (ClientReady(FALSE) && (!V_HostileImage || V_Design->Hide))
		PatchBytes((DWORD)LoadLibraryA("D2Client.dll") + 0x2864C, 0x46, 1);

	if (D2GFX_GetHwnd())
		SetWindowLongA(D2GFX_GetHwnd(), GWL_WNDPROC, (LONG)V_OldWndProc);

	if (ClientReady(FALSE))
	{
		V_Reveal->DestroyLevelExits();
		V_Reveal->RemoveMyAutomapCells();
		V_Reveal->DestroyMyAutomapCells();
		V_Reveal->DeleteShrines();
	}

	DestroyEditBox();
	DestroyFilterBox();
	DestroyTargets();
	DestroyVectors();

	return TRUE;
}
