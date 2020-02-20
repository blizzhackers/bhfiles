#include "Hack.h"
#include "Vectors.h"

VOID DestroyVectors()
{
	if (!V_CurrentVectors.IsEmpty())
	{
		for (INT i = 0; i < V_CurrentVectors.GetSize(); i++)
		{
			if (V_CurrentVectors[i])
				delete V_CurrentVectors[i];
		}

		V_CurrentVectors.RemoveAll();
	}
}

BOOL EnumerateVectors()
{
	if (GetUnitLevel(Me)->dwLevelNo != V_LastLevel)
	{
		LPLEVELEXIT Levels[0x40] = {NULL};

		V_CurrentVectors.Lock();

		for (INT i = 0; i < V_CurrentVectors.GetSize(); i++)
			delete V_CurrentVectors[i];

		V_CurrentVectors.RemoveAll();
		V_Reveal->CreateCollisionMap();
		V_Reveal->GetLevelExits(Levels, 0x40);

		for (INT i = 0; i < ArraySize(Waypoints); i++)
		{
			LPROOM2 Room;
			LPPRESETUNIT Unit = FindPresetUnit(Waypoints[i], UNIT_TYPE_OBJECT, &Room);

			if (!Unit)
				continue;

			LPVECTOR Vector = new VECTOR;
			Vector->Position.x = ((Room->dwPosX * 5) + Unit->dwPosX) * 32;
			Vector->Position.y = ((Room->dwPosY * 5) + Unit->dwPosY) * 32;
			Vector->Type = VECTOR_WAYPOINT;
			Vector->Target = Waypoints[i];
			Vector->TargetType = UNIT_TYPE_OBJECT;

			V_CurrentVectors.Add(Vector);
			break;
		}

		for (INT i = 0; i < ArraySize(Vectors); i++)
		{
			if (Vectors[i].CurrentLevel != GetUnitLevel(Me)->dwLevelNo)
				continue;

			if (Vectors[i].TargetType == TARGET_LEVEL)
			{
				for (INT l = 0; l < 0x40; l++)
				{
					if (!Levels[l])
						break;

					if (Vectors[i].Target != Levels[l]->dwTargetLevel)
						continue;

					if (Vectors[i].CurrentLevel == MAP_A2_CANYON_OF_THE_MAGI && Vectors[i].Target != Me->pAct->pMisc->dwStaffTombLevel)
						continue;

					for (INT k = 0; k < V_CurrentVectors.GetSize(); k++)
					{
						if (V_CurrentVectors[k]->Type == Vectors[i].VectorType)
							goto Continue;
					}

					LPVECTOR Vector = new VECTOR;
					Vector->Position.x = Levels[l]->ptPos.x * 32;
					Vector->Position.y = Levels[l]->ptPos.y * 32;
					Vector->Type = Vectors[i].VectorType;
					Vector->Target = Vectors[i].Target;
					Vector->TargetType = Vectors[i].TargetType;

					V_CurrentVectors.Add(Vector);
				}
			}

			else
			{
				LPROOM2 Room;
				LPPRESETUNIT Unit = FindPresetUnit(Vectors[i].Target, Vectors[i].TargetType, &Room);

				if (!Unit)
					continue;

				for (INT k = 0; k < V_CurrentVectors.GetSize(); k++)
				{
					if (V_CurrentVectors[k]->Type == Vectors[i].VectorType)
						goto Continue;
				}

				LPVECTOR Vector = new VECTOR;
				Vector->Position.x = ((Room->dwPosX * 5) + Unit->dwPosX) * 32;
				Vector->Position.y = ((Room->dwPosY * 5) + Unit->dwPosY) * 32;
				Vector->Type = Vectors[i].VectorType;
				Vector->Target = Vectors[i].Target;
				Vector->TargetType = Vectors[i].TargetType;

				V_CurrentVectors.Add(Vector);
			}

Continue:
			continue;
		}

		V_LastLevel = GetUnitLevel(Me)->dwLevelNo;
		V_CurrentVectors.Unlock();
	}

	return TRUE;
}

BOOL TeleportToVector(INT VectorType)
{
	if (!ClientReady(TRUE) || InTown(Me))
		return FALSE;

	if (V_TeleportQueue.IsLocked || !V_TeleportQueue.IsEmpty())
		return FALSE;

	POINT Start, End, PaTh[200] = {0};
	LPVECTOR Target = NULL;
	INT Count = 0;
	DWORD X, Y;
	CMatrix<WORD, WORD> Map;
	CPathFinder TeleportPaTh;

	for (INT i = 0; i < V_CurrentVectors.GetSize(); i++)
	{
		if (V_CurrentVectors[i]->Type == VectorType)
		{
			Target = V_CurrentVectors[i];
			break;
		}
	}

	if (!Target)
		return FALSE;

	V_Reveal->CreateCollisionMap();
	V_Reveal->ExportCollisionMap(Map);

	X = GetUnitLevel(Me)->dwPosX * 5;
	Y = GetUnitLevel(Me)->dwPosY * 5;

	Start.x = Me->pPath->xPos - X;
	Start.y = Me->pPath->yPos - Y;

	if (!Map.IsValidIndex(Start.x, Start.y))
	{
		Print(0, 0, "ÿc4Teleportÿc0: Failed to generate path, starting point is not a valid index");
		return FALSE;
	}

	End.x = (Target->Position.x / 32) - X;
	End.y = (Target->Position.y / 32) - Y;

	if (!Map.IsValidIndex(End.x, End.y))
	{
		Print(0, 4, "Teleportÿc0: ÿc1Failed to generate path, ending point is not a valid index");
		return FALSE;
	}

	TeleportPaTh.SetPathingMap(Map.GetData(), Map.GetCX(), Map.GetCY());
	Count = TeleportPaTh.CreateBestPath(Start, End, PaTh, 200);

	if (!Count)
	{
		Print(0, 4, "Teleportÿc0: ÿc1Failed to generate path, could not generate any nodes");
		return FALSE;
	}

	for (INT i = 1; i < Count; i++)
	{
		PaTh[i].x += X;
		PaTh[i].y += Y;

		V_TeleportQueue.Add(PaTh[i]);
	}

	V_CurrentVector = Target;
	V_Threads.Add(MakeThread((LPVOID)Threads, (LPVOID)TELEPORT));
	return TRUE;
}