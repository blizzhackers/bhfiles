#include "Hack.h"

VOID ChangeStat(BYTE StatID, BYTE Value)
{
	LPBYTE Packet = new BYTE[3];
	Packet[0] = 0x1D;
	Packet[1] = StatID;
	Packet[2] = Value;
	D2NET_ReceivePacket(Packet, 3);
	delete [] Packet;
}

VOID STDCALL GameAfterAttack(LPUNITANY Unit, DWORD SkillID, DWORD SkillLevel, BOOL Unknown)
{
	if (!ClientReady(TRUE))
		return;

	D2CLIENT_CreateAttack(Unit, SkillID, SkillLevel, Unknown);

	if (!Unit)
		return;

	if (Me && Unit->dwType == UNIT_TYPE_PLAYER && Unit->dwUnitId == Me->dwUnitId)
	{
		if (V_SpeedAttack)
		{
			if (Me->dwMode == PLAYER_MODE_ATTACK1 || Me->dwMode == PLAYER_MODE_ATTACK2 ||
				Me->dwMode == PLAYER_MODE_KICK || Me->dwMode == PLAYER_MODE_THROW)
				Me->dwFrameRemain = V_AttackFrames * (Me->dwFrameRemain / 7);
		}

		if (V_SpeedCast)
		{
			if (SkillID == D2S_CHAINLIGHTNING || SkillID == D2S_INFERNO || SkillID == D2S_LIGHTNING)
				Me->dwFrameRemain = 0;

			if (SkillID == D2S_TELEPORT)
				if (GetUnitStat(Me, STAT_FASTERCAST) < 39)
					Me->dwFrameRemain = 0;

			if (SkillID != D2S_TELEPORT && SkillID != D2S_CHAINLIGHTNING && SkillID != D2S_INFERNO && SkillID != D2S_LIGHTNING)
			{
				if (Me->dwMode == PLAYER_MODE_CAST || Me->dwMode == PLAYER_MODE_USESKILL1 ||
					Me->dwMode == PLAYER_MODE_USESKILL2 || Me->dwMode == PLAYER_MODE_USESKILL3 ||
					Me->dwMode == PLAYER_MODE_USESKILL4)
					Me->dwFrameRemain = V_CastFrames * (Me->dwFrameRemain / 7);
			}
		}
	}
}

VOID IncreaseSpeedStats()
{
	if (ClientReady(TRUE))
	{
		if (V_AttackSpeed > 0)
			ChangeStat(STAT_IAS, V_AttackSpeed > 255 ? 255 : V_AttackSpeed);

		if (V_BlockRate > 0)
			ChangeStat(STAT_FASTERBLOCK, V_BlockRate > 255 ? 255 : V_BlockRate);

		if (V_CastRate > 0)
			ChangeStat(STAT_FASTERCAST, V_CastRate > 255 ? 255 : V_CastRate);

		if (V_HitRecoveryRate > 0)
			ChangeStat(STAT_FASTERHITRECOVERY, V_HitRecoveryRate > 255 ? 255 : V_HitRecoveryRate);

		if (V_RunWalkSpeed > 0)
			ChangeStat(STAT_FASTERRUNWALK, V_RunWalkSpeed > 255 ? 255 : V_RunWalkSpeed);

		if (V_VelocitySpeed > 0)
			ChangeStat(STAT_VELOCITYPERCENT, V_VelocitySpeed > 255 ? 255 : V_VelocitySpeed);
	}
}