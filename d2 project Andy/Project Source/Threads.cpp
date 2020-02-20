#include "Hack.h"

BOOL STDCALL Threads(LPVOID Parameter)
{
	INT Thread = (INT)Parameter;

	switch (Thread)
	{
	case PRIMARY:

		while (TRUE)
		{
			SleepEx(50, TRUE);

			if (Me && !V_Game)
				Game(FALSE);
			else if (!Me && V_Game)
				Game(TRUE);

			if (!ClientReady(FALSE))
				continue;

			if (V_DelayedChicken)
			{
				SleepEx(500, TRUE);
				V_DelayedChicken = FALSE;
			}

			if (V_DeathMessageIteration)
			{
				V_DeathMessageIteration = FALSE;
				DeathMessage();
			}
		}

		break;

	case DESIGN:

		while (TRUE)
		{
			SleepEx(1, TRUE);

			if (!ClientReady(FALSE))
				continue;

			if (V_Screenshot)
				ScreenShot();

			if (V_Design->Hide && !V_HostileImage)
			{
				PatchBytes((DWORD)LoadLibraryA("D2Client.dll") + 0x2864C, 0x46, 1);
				V_HostileImage = TRUE;
			}

			else if (!V_Design->Hide && V_HostileImage)
			{
				if (D2CLIENT_GetDifficulty() > 0)
					PatchBytes((DWORD)LoadLibraryA("D2Client.dll") + 0x2864C, ((strlen(D2CLIENT_GetGameInfo()->szGamePassword) != NULL) ?
					0x46 + 0x5F : 0x46 + 0x55), 1);
				else
					PatchBytes((DWORD)LoadLibraryA("D2Client.dll") + 0x2864C, ((strlen(D2CLIENT_GetGameInfo()->szGamePassword) != NULL) ?
					0x46 + 0x4B : 0x46 + 0x41), 1);

				V_HostileImage = FALSE;
			}

			if (!ClientReady(TRUE))
				continue;

			if (V_AutoKB && V_KB)
				if (Knockback()) V_KB = FALSE;

			if (V_AutoBlind && V_Reblind)
				if (Reblind()) V_Reblind = FALSE;
		}

		break;

	case ENCHANT:

		while (TRUE)
		{
			SleepEx(500, TRUE);

			if (!ClientReady(TRUE) || !V_Enchant)
				continue;

			if (V_EnchantNoteIteration && !V_Players.IsEmpty())
			{
				V_EnchantNoteIteration = FALSE;

				if (Say("%s's Enchant Bot is enabled", V_Name))
					SleepEx(500, TRUE);

				if (Say("%s - Enchants you", V_EnchantPlayer))
					SleepEx(500, TRUE);

				if (Say("%s - Enchants your mercenary", V_EnchantMercenary))
					SleepEx(500, TRUE);

				if (Say("%s - Enchants everything", V_EnchantAll))
					SleepEx(500, TRUE);
			}

			if (V_EnchantQueue.IsEmpty() || V_EnchantQueue.IsLocked)
				continue;

			ExecuteEnchantQueue(V_EnchantQueue[0]);
			V_EnchantQueue.RemoveAt(0);
			V_EnchantQueue.FreeExtra();
		}

		break;

	case PACKETS:

		while (TRUE)
		{
			SleepEx(50, TRUE);

			if (!ClientReady(TRUE))
				continue;

			if (GetTickCount() - V_FlashLast >= V_FlashDelay)
			{
				for (INT i = 0; i < V_Players.GetSize(); i++)
				{
					if (V_Players[i]->Flashing)
					{
						if (ResynchPlayer(V_Players[i]->UnitId, UNIT_TYPE_PLAYER))
							SleepEx(50, TRUE);
					}
				}

				V_FlashLast = GetTickCount();
			}

			if (V_GoldDrop && V_GoldAmount > 0)
			{
				if (GetUnitStat(Me, STAT_GOLD) >= 1)
				{
					if (DropGold(Me->dwUnitId, V_GoldAmount))
						SleepEx(50, TRUE);
				}
			}
		}

		break;

	case PARTY:

		while(TRUE)
		{
			SleepEx(500, TRUE);

			if (!ClientReady(TRUE) || !V_AutoParty)
				continue;

			LPROSTERUNIT pMe = FindPartyById(Me->dwUnitId);

			if (!pMe)
				continue;

			for(LPROSTERUNIT pUnit = *p_D2CLIENT_PlayerUnitList; pUnit; pUnit = pUnit->pNext)
			{
				if(pUnit->dwUnitId == pMe->dwUnitId)
					continue;

				DWORD dwFlags = GetPvPFlags(pUnit->dwUnitId);

				if(dwFlags & PVP_ALLIED || dwFlags & PVP_ALLIED_WITH_YOU)
					continue;

				if(dwFlags & PVP_INVITED_BY_YOU)
					continue;

				if(dwFlags & PVP_HOSTILED_YOU || dwFlags & PVP_HOSTILED_BY_YOU)
					continue;

				if(dwFlags & PVP_INVITED_YOU)
				{
					BYTE aPacket[6];
					*(LPWORD)&aPacket[0] = 0x085E;
					*(LPDWORD)&aPacket[2] = pUnit->dwUnitId;
					D2NET_SendPacket(6, 1, aPacket);

					for(INT i = 0; i < 500 + (INT)*p_D2CLIENT_Ping; i++)
					{
						if(pMe->wPartyId != 0xFFFF)
							break;
						else
							SleepEx(1, TRUE);
					}

					continue;
				}

				if(!(dwFlags & PVP_ALLIED || dwFlags & PVP_ALLIED_WITH_YOU ||
					dwFlags & PVP_INVITED_YOU || dwFlags & PVP_INVITED_BY_YOU))
				{
					BYTE aPacket[6];
					*(LPWORD)&aPacket[0] = 0x065E;
					*(LPDWORD)&aPacket[2] = pUnit->dwUnitId;
					D2NET_SendPacket(6, 1, aPacket);

					for(INT i = 0; i < 500 + (INT)*p_D2CLIENT_Ping; i++)
					{
						DWORD dwFlags = GetPvPFlags(pUnit->dwUnitId);
						if(dwFlags & PVP_INVITED_BY_YOU)
							break;
						else
							SleepEx(1, TRUE);
					}

					continue;
				}
			}
		}

		break;

	case TELEPORT:

		WORD OldSkill;
		INT OldLevel;

		if (GetSkill(D2S_TELEPORT) == NULL)
		{
			V_TeleportQueue.RemoveAll();
			Print(0, 4, "Teleportÿc0: ÿc1Teleport skill not found");
			DestroyThread(GetCurrentThread(), NULL, INFINITE);
			while (TRUE) SleepEx(1, TRUE);
		}

		if (V_TeleportQueue.IsEmpty())
		{
			Print(0, 4, "Teleportÿc0: ÿc1There are no nodes to teleport to");
			DestroyThread(GetCurrentThread(), NULL, INFINITE);
			while (TRUE) SleepEx(1, TRUE);
		}

		V_TeleportQueue.Lock();

		OldSkill = GetCurrentSkill(V_LeftAttack);
		OldLevel = GetUnitLevel(Me)->dwLevelNo;

		while (!V_TeleportQueue.IsEmpty())
		{
			BOOL Arrived = FALSE;
			INT Iterations = NULL;

			if (GetCurrentSkill(V_LeftAttack) != D2S_TELEPORT)
				SetSkill(D2S_TELEPORT, V_LeftAttack);

			while (GetCurrentSkill(V_LeftAttack) != D2S_TELEPORT)
				SleepEx(1, TRUE);

			LPBYTE Packet = new BYTE[5];
			Packet[0] = V_LeftAttack ? 0x05 : 0x0C;
			*(LPWORD)&Packet[1] = (WORD)V_TeleportQueue[0].x;
			*(LPWORD)&Packet[3] = (WORD)V_TeleportQueue[0].y;
			D2NET_SendPacket(5, 1, Packet);
			delete [] Packet;

			while (Iterations < 1000)
			{
				if (!ClientReady(TRUE) || InTown(Me) || V_TeleportQueue.IsEmpty())
					break;

				if (CalculateDistance(Me->pPath->xPos, Me->pPath->yPos, V_TeleportQueue[0].x, V_TeleportQueue[0].y) <= 5)
				{
					Arrived = TRUE;
					break;
				}

				Iterations++;
				SleepEx(1, TRUE);
			}

			if (!Arrived)
			{
				V_TeleportQueue.RemoveAll();
				V_TeleportQueue.Unlock();

				Print(0, 4, "Teleportÿc0: ÿc1Failed to reach target, terminating teleport sequence");

				if (GetCurrentSkill(V_LeftAttack) != OldSkill)
					SetSkill(OldSkill, V_LeftAttack);

				while (GetCurrentSkill(V_LeftAttack) != OldSkill)
					SleepEx(1, TRUE);

				DestroyThread(GetCurrentThread(), NULL, INFINITE);
				return TRUE;
			}

			V_TeleportQueue.RemoveAt(0);
			V_TeleportQueue.FreeExtra();
		}

		if (V_CurrentVector)
		{
			if (GetCurrentSkill(V_LeftAttack) != OldSkill)
				SetSkill(OldSkill, V_LeftAttack);

			while (GetCurrentSkill(V_LeftAttack) != OldSkill)
				SleepEx(1, TRUE);

			for (LPROOM1 Room = Me->pAct->pRoom1; Room; Room = Room->pRoomNext)
			{
				for (LPUNITANY Unit = Room->pUnitFirst; Unit; Unit = Unit->pListNext)
				{
					if (Unit->dwType == UNIT_TYPE_TILE)
					{
						if (V_CurrentVector->TargetType == -1)
						{
							if (V_Reveal->GetTileLevelNo(Room->pRoom2, Unit->dwTxtFileNo) == V_CurrentVector->Target)
							{
								SleepEx(50 + (INT)Ping, TRUE);
								Interact(Unit->dwUnitId, Unit->dwType, TRUE);
								goto End;
							}
						}

						if (V_CurrentVector->Type == 3)
						{
							if (Unit->dwTxtFileNo == V_CurrentVector->Target)
							{
								SleepEx(50 + (INT)Ping, TRUE);
								Interact(Unit->dwUnitId, Unit->dwType, TRUE);
								goto End;
							}
						}
					}

					if (Unit->dwType == UNIT_TYPE_OBJECT)
					{
						if (V_CurrentVector->Type == 4)
						{
							if (Unit->dwTxtFileNo == V_CurrentVector->Target)
							{
								SleepEx(50 + (INT)Ping, TRUE);

								for (INT i = 0; i < 2; i++)
								{
									if (Unit->dwMode < 2)
									{
										Interact(Unit->dwUnitId, Unit->dwType, TRUE);
										SleepEx(500 + (INT)Ping, TRUE);
									}

									else
									{
										Interact(Unit->dwUnitId, Unit->dwType, TRUE);
										break;
									}
								}

								goto End;
							}
						}
					}
				}
			}
End:;
		}

		V_TeleportQueue.Unlock();
		DestroyThread(GetCurrentThread(), NULL, INFINITE);
		break;
	}
	
	return TRUE;
}

HANDLE MakeThread(LPVOID Process, LPVOID Parameter)
{
	HANDLE Thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Process, Parameter, NULL, NULL);
	if (!Thread) return NULL;
	return Thread;
}

VOID DestroyThread(HANDLE Thread, DWORD Parameter, DWORD MaxWaitTime)
{
	TerminateThread(Thread, Parameter);
	WaitForSingleObject(Thread, MaxWaitTime);
	CloseHandle(Thread);
}

VOID CreateInGameThreads()
{
	V_Threads.Add(MakeThread((LPVOID)Threads, (LPVOID)DESIGN));
	V_Threads.Add(MakeThread((LPVOID)Threads, (LPVOID)ENCHANT));
	V_Threads.Add(MakeThread((LPVOID)Threads, (LPVOID)PACKETS));
	V_Threads.Add(MakeThread((LPVOID)Threads, (LPVOID)PARTY));
}

VOID DestroyInGameThreads()
{
	if (!V_Threads.IsEmpty())
	{
		for (INT i = 0; i < V_Threads.GetSize(); i++)
		{
			if (V_Threads[i])
				DestroyThread(V_Threads[i], i + 1, INFINITE);
		}

		V_Threads.RemoveAll();
	}
}