#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#define _WIN32_WINNT 0x600

#undef UNICODE

#include <windows.h>
#include <ctime>
#include <iosfwd>
#include <list>
#include <math.h>
#include <process.h>
#include <psapi.h>
#include <shlwapi.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <tlhelp32.h>
#include <valarray>
#include <vector>
#include <winbase.h>
#include <windef.h>
#include <winnt.h>
#include <winuser.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shlwapi.lib")

#define EXPORT __declspec(dllexport)
#define FASTCALL __fastcall
#define FUNCCALL __cdecl
#define IMPORT __declspec(dllimport)
#define NAKED __declspec(naked)
#define STDCALL __stdcall

#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include "Aim.h"
#include "Array Ex.h"
#include "Common Math.h"
#include "Configuration.h"
#include "Constants.h"
#include "Copy and Paste.h"
#include "D2 Pointers.h"
#include "D2 Stdio.h"
#include "D2 Structs.h"
#include "D2 Stubs.h"
#include "Design.h"
#include "Enchant.h"
#include "Escape.h"
#include "Game Filter.h"
#include "Handlers.h"
#include "Helpers.h"
#include "Item.h"
#include "Matrix.h"
#include "Message.h"
#include "Miscellaneous.h"
#include "Offset.h"
#include "Packet Received.h"
#include "Packet Sent.h"
#include "Patch Stubs.h"
#include "Potion.h"
#include "Reveal.h"
#include "Skill.h"
#include "Speed.h"
#include "Teleport.h"
#include "Threads.h"
#include "Unit.h"
#include "Vars.h"
#include "Winamp.h"
#include "Rogue.h"

BOOL Attach(HINSTANCE DLL);
BOOL Detach();
