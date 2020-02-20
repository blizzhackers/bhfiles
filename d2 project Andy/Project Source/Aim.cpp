#include "Hack.h"

BOOL AttackNearestTarget()
{
	D2SpellInfo Spell;
	GetSkillInfo(GetCurrentSkill(V_LeftAttack), &Spell);

	if (!ClientReady(TRUE) || !V_Aim)
		return FALSE;

	if (InTown(Me) || UsingSkill())
		return FALSE;

	POINT Mouse = {MouseX, MouseY};
	LPUNITANY Target = FindNearestUnit(Mouse, Spell.dwSpellInfoFlag, FALSE, FALSE);

	if (Target && Target->dwMode != PLAYER_MODE_DEAD && Target->dwMode != PLAYER_MODE_DEATH)
	{
		if (Me->dwMode != PLAYER_MODE_CAST)
		{
			AttackTarget(Target->dwUnitId, Target->dwType);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL AttackTarget(DWORD UnitID, DWORD UnitType)
{
	LPUNITANY Target = (LPUNITANY)GetUnit(UnitID, UnitType);
	BLIND Blinds[4] = {{2, 2}, {3, 2}, {-1, 3}, {2.0, -1.2}};
	WORD SkillID = GetCurrentSkill(V_LeftAttack);

	PACKETEDATTACK Attacks[] =
	{
		{D2S_ATTACK,			TRUE},
		{D2S_BERSERK,			TRUE},
		{D2S_BLIZZARD,			FALSE},
		{D2S_BONEPRISON,		FALSE},
		{D2S_CHARGE,			FALSE},
		{D2S_CHARGEDSTRIKE,		TRUE},
		{D2S_DECOY,				FALSE},
		{D2S_ENCHANT,			FALSE},
		{D2S_FIREBALL,			FALSE},
		{D2S_KICK,				TRUE},
		{D2S_LEAPATTACK,		TRUE},
		{D2S_LIGHTNINGSTRIKE,	TRUE},
		{D2S_SLOWMISSILES,		FALSE},
		{D2S_SMITE,				TRUE}
	};

	if (!ClientReady(TRUE) || InTown(Me) || !V_Aim)
		return FALSE;

	if (V_BlockFakes && UsingSkill())
		return FALSE;

	if (!Target)
		return FALSE;

	if (Target->dwType == UNIT_TYPE_PLAYER && (Target->dwMode == PLAYER_MODE_DEAD || Target->dwMode == PLAYER_MODE_DEATH))
		return FALSE;

	if (Target->dwType == UNIT_TYPE_NPC && (Target->dwMode == NPC_MODE_DEAD || Target->dwMode == NPC_MODE_DEATH))
		return FALSE;

	for (INT i = 0; i < ArraySize(Attacks); i++)
	{
		if (SkillID == Attacks[i].SkillID)
		{
			UseSkillOnUnit(Target, Attacks[i].SkillID, V_LeftAttack, Attacks[i].Shift, TRUE);
			return TRUE;
		}
	}

	if (SkillID == D2S_TELEPORT && V_Blind == 0 && Target->dwMode != PLAYER_MODE_RUN && !GetUnitState(Target, AFFECT_WHIRLWIND))
	{
		UseSkillOnUnit(Target, SkillID, V_LeftAttack, FALSE, TRUE);
		return TRUE;
	}

	else
	{
		AttackStruct Attack;
		D2SpellInfo Spell;

		Attack.dwAttackType = V_LeftAttack ? ATTACKTYPE_SHIFTLEFT : ATTACKTYPE_SHIFTRIGHT;
		GetSkillInfo(SkillID, &Spell);

		if (SkillID == D2S_TELEPORT)
		{
			if (Target->dwMode == PLAYER_MODE_RUN || GetUnitState(Target, AFFECT_WHIRLWIND))
			{
				Attack.dwTargetX = Target->pPath->xTarget;
				Attack.dwTargetY = Target->pPath->yTarget;
			}

			else
			{
				if (V_Blind >= 1 && V_Blind <= 4)
				{
					Attack.dwTargetX = Target->pPath->xPos + (WORD)Blinds[V_Blind - 1].X;
					Attack.dwTargetY = Target->pPath->yPos + (WORD)Blinds[V_Blind - 1].Y;
				}

				if (V_Blind == 5)
				{
					Attack.dwTargetX = Target->pPath->xTarget;
					Attack.dwTargetY = Target->pPath->yTarget;
				}
			}
		}

		else
		{
			Attack.dwTargetX = Target->pPath->xPos;
			Attack.dwTargetY = Target->pPath->yPos;
		}

		Attack.lpPlayerUnit = Me;

		if (Spell.dwSpellInfoFlag & DSI_GUIDED)
		{
			Attack.dwAttackType = V_LeftAttack ? ATTACKTYPE_LEFT : ATTACKTYPE_RIGHT;
			Attack.lpTargetUnit = Target;
		}

		else
			Attack.lpTargetUnit = NULL;

		Attack._1 = 3;
		Attack._2 = 4;

		D2CLIENT_Attack(&Attack, (Spell.dwSpellInfoFlag & DSI_GUIDED) ? TRUE : FALSE);
		return TRUE;
	}

	return FALSE;
}

BOOL EnumeratePlayers()
{
	CArrayEx<PTARGETINFO, PTARGETINFO> TempInfo;
	INT NewTarget = NULL;

	if (V_Players.IsLocked)
		return FALSE;

	V_Players.Lock();

	for (INT i = 0; i < V_Players.GetSize(); i++)
		TempInfo.Add(V_Players[i]);
	
	V_Players.RemoveAll();

	for (LPROSTERUNIT Unit = *p_D2CLIENT_PlayerUnitList; Unit; Unit = Unit->pNext)
	{
		if (!Unit)
			continue;

		if (Unit->dwUnitId == Me->dwUnitId)
			continue;

		PTARGETINFO PlayerInfo = new TARGETINFO;

		strcpy_s(PlayerInfo->PlayerName, Unit->szName);
		PlayerInfo->UnitId = Unit->dwUnitId;
		PlayerInfo->Level = (BYTE)Unit->wLevel;
		PlayerInfo->ClassId = Unit->dwClassId;
		PlayerInfo->Flashing = FALSE;
		PlayerInfo->Life = (BYTE)Unit->dwPartyLife;

		for (INT i = 0; i < TempInfo.GetSize(); i++)
		{
			if (TempInfo[i]->UnitId == Unit->dwUnitId)
			{
				PlayerInfo->Flashing = TempInfo[i]->Flashing;
				PlayerInfo->Life = TempInfo[i]->Life;
			}
		}

		V_Players.Add(PlayerInfo);

		if (!TempInfo.IsEmpty() && PlayerInfo->UnitId == TempInfo[V_Target]->UnitId)
			NewTarget = V_Players.GetSize() - 1;
	}

	for (INT i = 0; i < TempInfo.GetSize(); i++)
		delete TempInfo[i];

	V_Target = NewTarget;
	V_Players.Unlock();

	return TRUE;
}

BOOL Knockback()
{
	if (!ClientReady(TRUE))
		return FALSE;

	if (InTown(Me) || V_Players.IsEmpty())
		return FALSE;

	for (LPROOM1 Room1 = Me->pAct->pRoom1; Room1; Room1 = Room1->pRoomNext)
	{
		for (LPUNITANY Unit = Room1->pUnitFirst; Unit; Unit = Unit->pListNext)
		{
			if (Unit && Unit->dwType == UNIT_TYPE_PLAYER)
			{
				if (Unit->dwUnitId == Me->dwUnitId)
					continue;

				if (GetUnitDist(Me, Unit) > V_AutoKBDistance)
					continue;

				if (InTown(Unit))
					continue;

				if (Unit->dwMode == PLAYER_MODE_DEAD || Unit->dwMode == PLAYER_MODE_DEATH)
					continue;

				if (GetPvPFlags(Unit->dwUnitId) & PVP_HOSTILED_YOU || GetPvPFlags(Unit->dwUnitId) & PVP_HOSTILED_BY_YOU)
				{
					WORD Old = GetCurrentSkill(TRUE);
					WORD KB = GetSkill(D2S_SMITE) ? D2S_SMITE : D2S_KICK;

					if (GetCurrentSkill(TRUE) != KB)
						SetSkill(KB, TRUE);

					while (GetCurrentSkill(TRUE) != KB)
						SleepEx(1, TRUE);

					if (GetCurrentSkill(TRUE) == KB)
						UseSkillOnUnit(Unit, KB, TRUE, TRUE, TRUE);

					if (GetCurrentSkill(TRUE) != Old)
						SetSkill(Old, TRUE);

					while (GetCurrentSkill(TRUE) != Old)
						SleepEx(1, TRUE);

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL MiddleMouseButton()
{
	if (!ClientReady(TRUE))
		return FALSE;

	if (InTown(Me))
		return FALSE;

	if (Me->dwMode == PLAYER_MODE_CAST || Me->dwMode == PLAYER_MODE_ATTACK1 || Me->dwMode == PLAYER_MODE_ATTACK2 ||
		Me->dwMode == PLAYER_MODE_KICK || Me->dwMode == PLAYER_MODE_THROW)
		return FALSE;

	if (KeyDown(VK_MBUTTON))
	{
		if (V_FarcastTeleport && GetCurrentSkill(FALSE) == D2S_TELEPORT)
		{
			POINT Origin = {(Me->pPath->xPos), (Me->pPath->yPos)};
			POINT Mouse = {(MouseX), (MouseY)};

			V_Design->ScreenToAutoMap(&Mouse);

			INT Angle = CalculateAngle(Origin, Mouse);
			INT Radius = 100;
			POINT Cast;

			while (TRUE)
			{
				if (Radius <= 0)
					break;

				Cast = CalculatePointOnTrack(Origin, Radius--, Angle);

				if (GetRoom(Cast.x, Cast.y))
				{
					if (!D2COMMON_CheckCollision(Me->pPath->pRoom1, Cast.x, Cast.y, 2))
						if (::abs(Me->pPath->xPos - Cast.x) <= 50)
							if (::abs(Me->pPath->yPos - Cast.y) <= 50)
								break;
				}
			}

			AttackStruct Attack;
			Attack.dwAttackType = ATTACKTYPE_SHIFTRIGHT;

			Attack.dwTargetX = Cast.x;
			Attack.dwTargetY = Cast.y;

			Attack.lpPlayerUnit = Me;
			Attack.lpTargetUnit = NULL;

			D2CLIENT_Attack(&Attack, FALSE);
			SleepEx(50, TRUE);
			return TRUE;
		}

		else if (V_Aim && GetCurrentSkill(FALSE) != D2S_TELEPORT)
		{
			AttackNearestTarget();
			SleepEx(50, TRUE);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL NextTarget()
{
	if (V_Players.IsEmpty())
		return FALSE;

	if (V_Target <= V_Players.GetSize() - 1 && V_Target > 0)
		V_Target--;

	else if (V_Target == NULL && V_Players.GetSize() - 1 > 0)
		V_Target = V_Players.GetSize() - 1;

	else if (V_Players.GetSize() <= 0)
		return FALSE;

	return TRUE;
}

BOOL PreviousTarget()
{
	if (V_Players.IsEmpty())
		return FALSE;

	if (V_Target >= 0 && V_Target < V_Players.GetSize() - 1)
		V_Target++;

	else if (V_Target == V_Players.GetSize() - 1 && V_Players.GetSize() - 1 > 0)
		V_Target = NULL;

	else if (V_Players.GetSize() <= 0)
		return FALSE;

	return TRUE;
}

BOOL Reblind()
{
	if (!ClientReady(TRUE))
		return FALSE;

	if (InTown(Me) || V_Players.IsEmpty())
		return FALSE;

	if (!GetSkill(D2S_TELEPORT))
		return FALSE;

	for (LPROOM1 Room1 = Me->pAct->pRoom1; Room1; Room1 = Room1->pRoomNext)
	{
		for (LPUNITANY Unit = Room1->pUnitFirst; Unit; Unit = Unit->pListNext)
		{
			if (Unit && Unit->dwType == UNIT_TYPE_PLAYER)
			{
				if (Unit->dwUnitId == Me->dwUnitId)
					continue;

				if (GetUnitDist(Me, Unit) > V_AutoBlindDistance)
					continue;

				if (InTown(Unit))
					continue;

				if (Unit->dwMode == PLAYER_MODE_DEAD || Unit->dwMode == PLAYER_MODE_DEATH)
					continue;

				if (GetPvPFlags(Unit->dwUnitId) & PVP_HOSTILED_YOU || GetPvPFlags(Unit->dwUnitId) & PVP_HOSTILED_BY_YOU)
				{
					WORD Old = GetCurrentSkill(V_LeftAttack);

					if (V_Blind >= 1 && V_Blind <= 4)
					{
						if (GetCurrentSkill(V_LeftAttack) != D2S_TELEPORT)
							SetSkill(D2S_TELEPORT, V_LeftAttack);

						while (GetCurrentSkill(V_LeftAttack) != D2S_TELEPORT)
							SleepEx(1, TRUE);

						if (Me->dwMode == PLAYER_MODE_CAST)
						{
							Me->dwMode = PLAYER_MODE_STAND_OUTTOWN;
							Me->dwFrameRemain = 1;
						}

						if (GetCurrentSkill(V_LeftAttack) == D2S_TELEPORT)
							AttackTarget(Unit->dwUnitId, UNIT_TYPE_PLAYER);

						if (GetCurrentSkill(V_LeftAttack) != Old)
							SetSkill(Old, V_LeftAttack);

						while (GetCurrentSkill(V_LeftAttack) != Old)
							SleepEx(1, TRUE);

						return TRUE;
					}

					else if (V_Blind == 0 || V_Blind == 5)
					{
						INT Blind = V_Blind;

						if (GetCurrentSkill(V_LeftAttack) != D2S_TELEPORT)
							SetSkill(D2S_TELEPORT, V_LeftAttack);

						while (GetCurrentSkill(V_LeftAttack) != D2S_TELEPORT)
							SleepEx(1, TRUE);

						if (Me->dwMode == PLAYER_MODE_CAST)
						{
							Me->dwMode = PLAYER_MODE_STAND_OUTTOWN;
							Me->dwFrameRemain = 1;
						}

						V_Blind = 1;

						if (GetCurrentSkill(V_LeftAttack) == D2S_TELEPORT)
							AttackTarget(Unit->dwUnitId, UNIT_TYPE_PLAYER);

						V_Blind = Blind;

						if (GetCurrentSkill(V_LeftAttack) != Old)
							SetSkill(Old, V_LeftAttack);

						while (GetCurrentSkill(V_LeftAttack) != Old)
							SleepEx(1, TRUE);

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

VOID DestroyTargets()
{
	if (!V_Players.IsEmpty())
	{
		for (INT i = 0; i < V_Players.GetSize(); i++)
		{
			if (V_Players[i])
				delete V_Players[i];
		}

		V_Players.RemoveAll();
	}
}