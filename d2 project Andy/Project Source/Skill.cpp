#include "Hack.h"

WORD GetCurrentSkill(BOOL Left)
{
	if (!ClientReady(TRUE))
		return FALSE;

	if (Left)
		return Me->pInfo->pLeftSkill->pSkillInfo->wSkillId;
	else
		return Me->pInfo->pRightSkill->pSkillInfo->wSkillId;
}

INT GetSkill(WORD SkillId)
{
	if (!ClientReady(TRUE))
		return FALSE;

	for (LPSKILL Skill = Me->pInfo->pFirstSkill; Skill; Skill = Skill->pNextSkill)
		if (Skill->pSkillInfo->wSkillId == SkillId)
			return Skill->dwSkillLevel ? Skill->dwSkillLevel : TRUE;

	return FALSE;
}

BOOL FASTCALL OnUseSkill(LPATTACKSTRUCT Attack, BOOL AttackingUnit)
{
	BOOL LeftAttack = (Attack->dwAttackType == ATTACKTYPE_LEFT || Attack->dwAttackType == ATTACKTYPE_SHIFTLEFT) ? TRUE : FALSE;
	WORD SelectedSkill = GetCurrentSkill(LeftAttack);
	POINT Target = {(Attack->dwTargetX), (Attack->dwTargetY)};

	if (KeyDown(VK_MBUTTON) && !LeftAttack)
	{
		if (V_FarcastTeleport && SelectedSkill == D2S_TELEPORT)
		{
			POINT Farcast = PickFarcastLocation(Target);
			Attack->dwTargetX = Farcast.x;
			Attack->dwTargetY = Farcast.y;
			return TRUE;
		}
	}

	return AttackingUnit;
}

BOOL UsingSkill()
{
	if (Me->dwMode == PLAYER_MODE_CAST || Me->dwMode == PLAYER_MODE_ATTACK1 || Me->dwMode == PLAYER_MODE_ATTACK2 ||
		Me->dwMode == PLAYER_MODE_KICK || Me->dwMode == PLAYER_MODE_THROW)
		return TRUE;

	return FALSE;
}

BOOL SetSkill(WORD SkillId, BOOL Left)
{
	if (!Me)
		return FALSE;

	BYTE Packet[9];
	Packet[0] = 0x3C;
	*(LPWORD)&Packet[1] = SkillId;
	Packet[3] = NULL;
	Packet[4] = Left ? 0x80 : 0x00;
	*(LPDWORD)&Packet[5] = 0xFFFFFFFF;
	D2NET_SendPacket(9, 1, Packet);
	return TRUE;
}

BOOL GetSkillInfo(WORD wSpell, D2SpellInfo *lpBuffer)
{
	if (lpBuffer == NULL)
		return FALSE;

	::memset(lpBuffer, 0, sizeof(D2SpellInfo));
	lpBuffer->wSpellID = wSpell;
	lpBuffer->nClass = CLASS_NA;

	switch (wSpell)
	{
		// Common Skills 		
	case D2S_ATTACK:			

		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Attack", SPELL_MAX_NAME);
		break;

	case D2S_THROW:			

		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Throw", SPELL_MAX_NAME);
		break;

	case D2S_UNSUMMON:			

		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	

		strncpy(lpBuffer->szSpellName, "Unsummon", SPELL_MAX_NAME);
		break;

	case D2S_TOMEOFIDENTIFY:			

		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	

		strncpy(lpBuffer->szSpellName, "Tome of Identify", SPELL_MAX_NAME);
		break;

	case D2S_SCROLLOFIDENTIFY:


		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	

		strncpy(lpBuffer->szSpellName, "Scroll of Identify", SPELL_MAX_NAME);
		break;

	case D2S_TOMEOFTOWNPORTAL:

		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			

		strncpy(lpBuffer->szSpellName, "Tome of Town Portal", SPELL_MAX_NAME);
		break;

	case D2S_SCROLLOFTOWNPORTAL:			

		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			

		strncpy(lpBuffer->szSpellName, "Scroll of Town Portal", SPELL_MAX_NAME);
		break;

		///////////////////////////////////////////////////////////////
		//    Amazon Skills
		///////////////////////////////////////////////////////////////

		// Javelin and Spear Skills
	case D2S_JAB:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Jab", SPELL_MAX_NAME);
		break;

	case D2S_IMPALE:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Impale", SPELL_MAX_NAME);
		break;

	case D2S_FEND:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;			

		strncpy(lpBuffer->szSpellName, "Fend", SPELL_MAX_NAME);
		break;

	case D2S_POWERSTRIKE:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Power Strike", SPELL_MAX_NAME);
		break;

	case D2S_CHARGEDSTRIKE:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Charged Strike", SPELL_MAX_NAME);
		break;

	case D2S_LIGHTNINGSTRIKE:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;	

		strncpy(lpBuffer->szSpellName, "Lightning Strike", SPELL_MAX_NAME);
		break;

	case D2S_POISONJAVELIN:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_POISON;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Poison Javelin", SPELL_MAX_NAME);
		break;

	case D2S_LIGHTNINGBOLT:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Lightning Bolt", SPELL_MAX_NAME);
		break;

	case D2S_PLAGUEJAVELIN:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_POISON;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Plague Javelin", SPELL_MAX_NAME);
		break;

	case D2S_LIGHTNINGFURY:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Lightning Fury", SPELL_MAX_NAME);
		break;


		// Passive and Magic Skills
	case D2S_INNERSIGHT:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			

		strncpy(lpBuffer->szSpellName, "Inner Sight", SPELL_MAX_NAME);
		break;

	case D2S_SLOWMISSILES:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			

		strncpy(lpBuffer->szSpellName, "Slow Missiles", SPELL_MAX_NAME);
		break;

	case D2S_DECOY:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Decoy", SPELL_MAX_NAME);
		break;

	case D2S_VALKYRIE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Valkyrie", SPELL_MAX_NAME);
		break;

	case D2S_DODGE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Dodge", SPELL_MAX_NAME);
		break;

	case D2S_AVOID:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Avoid", SPELL_MAX_NAME);
		break;

	case D2S_EVADE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Evade", SPELL_MAX_NAME);
		break;

	case D2S_CRITICALSTRIKE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Critical Strike", SPELL_MAX_NAME);
		break;

	case D2S_PENETRATE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Penetrate", SPELL_MAX_NAME);
		break;

	case D2S_PIERCE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Pierce", SPELL_MAX_NAME);
		break;

		// Bow and Crossbow Skills
	case D2S_COLDARROW:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			

		strncpy(lpBuffer->szSpellName, "Cold Arrow", SPELL_MAX_NAME);
		break;

	case D2S_ICEARROW:			
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				

		strncpy(lpBuffer->szSpellName, "Ice Arrow", SPELL_MAX_NAME);
		break;

	case D2S_FREEZINGARROW:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Freezing Arrow", SPELL_MAX_NAME);
		break;

	case D2S_MAGICARROW:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	

		strncpy(lpBuffer->szSpellName, "Magic Arrow", SPELL_MAX_NAME);
		break;

	case D2S_MULTIPLESHOT:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Multiple Shot", SPELL_MAX_NAME);
		break;

	case D2S_GUIDEDARROW:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_GUIDED;

		strncpy(lpBuffer->szSpellName, "Guided Arrow", SPELL_MAX_NAME);
		break;

	case D2S_STRAFE:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Strafe", SPELL_MAX_NAME);
		break;

	case D2S_FIREARROW:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Fire Arrow", SPELL_MAX_NAME);
		break;

	case D2S_EXPLODINGARROW:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Exploding Arrow", SPELL_MAX_NAME);
		break;

	case D2S_IMMOLATIONARROW:
		lpBuffer->nClass = CLASS_AMA;
		lpBuffer->dwCoolDown = 1000;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Immolation Arrow", SPELL_MAX_NAME);
		break;


		///////////////////////////////////////////////////////////////
		//    Assassin Skills
		///////////////////////////////////////////////////////////////

		// Martial Arts
	case D2S_FISTSOFFIRE:
		lpBuffer->nClass = CLASS_ASN;
		//lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Fists of Fire", SPELL_MAX_NAME);
		break;

	case D2S_CLAWSOFTHUNDER:
		lpBuffer->nClass = CLASS_ASN;
		//lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Claws of Thunder", SPELL_MAX_NAME);
		break;

	case D2S_BLADESOFICE:
		lpBuffer->nClass = CLASS_ASN;
		//lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		//lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Blades of Ice", SPELL_MAX_NAME);
		break;

	case D2S_TIGERSTRIKE:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Tiger Strike", SPELL_MAX_NAME);
		break;

	case D2S_COBRASTRIKE:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Cobra Strike", SPELL_MAX_NAME);
		break;

	case D2S_PHOENIXSTRIKE:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Phoenix Strike", SPELL_MAX_NAME);
		break;

	case D2S_DRAGONTALON:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Dragon Talon", SPELL_MAX_NAME);
		break;

	case D2S_DRAGONCLAW:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Dragon Claw", SPELL_MAX_NAME);
		break;

	case D2S_DRAGONTAIL:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Dragon Tail", SPELL_MAX_NAME);
		break;

	case D2S_DRAGONFLIGHT:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwCoolDown = 2000;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		//lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Dragon Flight", SPELL_MAX_NAME);
		break;


		// Shadow Disciplines
	case D2S_BURSTOFSPEED:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Burst of Speed", SPELL_MAX_NAME);
		break;

	case D2S_FADE:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Fade", SPELL_MAX_NAME);
		break;

	case D2S_VENOM:
		lpBuffer->nClass = CLASS_ASN;	
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Venom", SPELL_MAX_NAME);
		break;

	case D2S_CLAWMASTERY:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Claw Mastery", SPELL_MAX_NAME);
		break;

	case D2S_WEAPONBLOCK:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Weapon Block", SPELL_MAX_NAME);
		break;

	case D2S_SHADOWWARRIOR:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Shadow Warrior", SPELL_MAX_NAME);
		break;

	case D2S_SHADOWMASTER:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Shadow Master", SPELL_MAX_NAME);
		break;

	case D2S_PSYCHICHAMMER:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;

		strncpy(lpBuffer->szSpellName, "Psychic Hammer", SPELL_MAX_NAME);
		break;

	case D2S_CLOAKOFSHADOWS:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Cloak of Shadows", SPELL_MAX_NAME);
		break;

	case D2S_MINDBLAST:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Mind Blast", SPELL_MAX_NAME);
		break;

		// Traps
	case D2S_SHOCKWEB:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;			

		strncpy(lpBuffer->szSpellName, "Shock Web", SPELL_MAX_NAME);
		break;

	case D2S_CHARGEDBOLTSENTRY:

		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Charged Bolt Sentry", SPELL_MAX_NAME);
		break;

	case D2S_LIGHTNINGSENTRY:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Lightning Sentry", SPELL_MAX_NAME);
		break;

	case D2S_DEATHSENTRY:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Death Sentry", SPELL_MAX_NAME);
		break;

	case D2S_FIREBLAST:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Fire Blast", SPELL_MAX_NAME);
		break;

	case D2S_WAKEOFFIRE:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Wake of Fire", SPELL_MAX_NAME);
		break;

	case D2S_WAKEOFINFERNO:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Wake of Inferno", SPELL_MAX_NAME);
		break;

	case D2S_BLADESENTINEL:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			

		strncpy(lpBuffer->szSpellName, "Blade Sentinel", SPELL_MAX_NAME);
		break;

	case D2S_BLADEFURY:
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	

		strncpy(lpBuffer->szSpellName, "Blade Fury", SPELL_MAX_NAME);
		break;

	case D2S_BLADESHIELD:			
		lpBuffer->nClass = CLASS_ASN;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Blade Shield", SPELL_MAX_NAME);
		break;


		///////////////////////////////////////////////////////////////
		//    Barbarian Skills
		///////////////////////////////////////////////////////////////

		// Warcries
	case D2S_HOWL:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;			

		strncpy(lpBuffer->szSpellName, "Howl", SPELL_MAX_NAME);
		break;

	case D2S_TAUNT:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			

		strncpy(lpBuffer->szSpellName, "Taunt", SPELL_MAX_NAME);
		break;

	case D2S_BATTLECRY:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;

		strncpy(lpBuffer->szSpellName, "Battle Cry", SPELL_MAX_NAME);
		break;

	case D2S_WARCRY:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;

		strncpy(lpBuffer->szSpellName, "War Cry", SPELL_MAX_NAME);
		break;

	case D2S_SHOUT:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;

		strncpy(lpBuffer->szSpellName, "Shout", SPELL_MAX_NAME);
		break;

	case D2S_BATTLEORDERS:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;

		strncpy(lpBuffer->szSpellName, "Battle Orders", SPELL_MAX_NAME);
		break;

	case D2S_BATTLECOMMAND:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;

		strncpy(lpBuffer->szSpellName, "Battle Command", SPELL_MAX_NAME);
		break;

	case D2S_FINDPOTION:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;

		strncpy(lpBuffer->szSpellName, "Find Potion", SPELL_MAX_NAME);
		break;

	case D2S_FINDITEM:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;

		strncpy(lpBuffer->szSpellName, "Find Item", SPELL_MAX_NAME);
		break;

	case D2S_GRIMWARD:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;

		strncpy(lpBuffer->szSpellName, "Grim Ward", SPELL_MAX_NAME);
		break;


		// Combat Masteries
	case D2S_SWORDMASTERY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Sword Mastery", SPELL_MAX_NAME);
		break;

	case D2S_POLEARMMASTERY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Pole Arm Mastery", SPELL_MAX_NAME);
		break;

	case D2S_INCREASEDSTAMINA:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Increased Stamin", SPELL_MAX_NAME);
		break;

	case D2S_INCREASEDSPEED:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Increased Speed", SPELL_MAX_NAME);
		break;

	case D2S_AXEMASTERY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Axe Mastery", SPELL_MAX_NAME);
		break;

	case D2S_THROWINGMASTERY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Throwing Mastery", SPELL_MAX_NAME);
		break;

	case D2S_MACEMASTERY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Mace Mastery", SPELL_MAX_NAME);
		break;

	case D2S_SPEARMASTERY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Spear Mastery", SPELL_MAX_NAME);
		break;

	case D2S_IRONSKIN:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Iron Skin", SPELL_MAX_NAME);
		break;

	case D2S_NATURALRESISTANCE:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Natural Resistance", SPELL_MAX_NAME);
		break;

		// Combat Skills
	case D2S_LEAP:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;	

		strncpy(lpBuffer->szSpellName, "Leap", SPELL_MAX_NAME);
		break;

	case D2S_LEAPATTACK:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;

		strncpy(lpBuffer->szSpellName, "Leap Attack", SPELL_MAX_NAME);
		break;

	case D2S_WHIRLWIND:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Whirlwind", SPELL_MAX_NAME);
		break;

	case D2S_BASH:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Bash", SPELL_MAX_NAME);
		break;

	case D2S_STUN:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Stun", SPELL_MAX_NAME);
		break;

	case D2S_CONCENTRATE:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Concentrate", SPELL_MAX_NAME);
		break;

	case D2S_BERSERK:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Berserk", SPELL_MAX_NAME);
		break;

	case D2S_DOUBLESWING:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Double Swing", SPELL_MAX_NAME);
		break;

	case D2S_DOUBLETHROW:			
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Double Throw", SPELL_MAX_NAME);
		break;

	case D2S_FRENZY:
		lpBuffer->nClass = CLASS_BAR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Frenzy", SPELL_MAX_NAME);
		break;


		///////////////////////////////////////////////////////////////
		//    Druid Skills
		///////////////////////////////////////////////////////////////

		// Elemental
	case D2S_FIRESTORM:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;			

		strncpy(lpBuffer->szSpellName, "Firestorm", SPELL_MAX_NAME);
		break;

	case D2S_MOLTENBOULDER:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwCoolDown = 2000;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Molten Boulder", SPELL_MAX_NAME);
		break;

	case D2S_FISSURE:			
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwCoolDown = 2000;

		strncpy(lpBuffer->szSpellName, "Fissure", SPELL_MAX_NAME);
		break;

	case D2S_VOLCANO:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwCoolDown = 4000;

		strncpy(lpBuffer->szSpellName, "Volcano", SPELL_MAX_NAME);
		break;

	case D2S_ARMAGEDDON:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwCoolDown = 6000;

		strncpy(lpBuffer->szSpellName, "Armageddon", SPELL_MAX_NAME);
		break;

	case D2S_TWISTER:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Twister", SPELL_MAX_NAME);
		break;

	case D2S_TORNADO:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Tornado", SPELL_MAX_NAME);
		break;

	case D2S_HURRICANE:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwCoolDown = 6000;

		strncpy(lpBuffer->szSpellName, "Hurricane", SPELL_MAX_NAME);
		break;

	case D2S_ARCTICBLAST:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Arctic Blast", SPELL_MAX_NAME);
		break;

	case D2S_CYCLONEARMOR:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Cyclone Armor", SPELL_MAX_NAME);
		break;


		// Shape Shifting
	case D2S_WEREWOLF:			
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
		lpBuffer->dwCoolDown = 1000;

		strncpy(lpBuffer->szSpellName, "Werewolf", SPELL_MAX_NAME);
		break;

	case D2S_FERALRAGE:			
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Feral Rage", SPELL_MAX_NAME);
		break;

	case D2S_RABIES:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_POISON;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Rabies", SPELL_MAX_NAME);
		break;

	case D2S_FURY:			
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Fury", SPELL_MAX_NAME);
		break;

	case D2S_LYCANTHROPY:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Lycanthropy", SPELL_MAX_NAME);
		break;

	case D2S_FIRECLAWS:			
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Fire Claws", SPELL_MAX_NAME);
		break;

	case D2S_HUNGER:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;

		strncpy(lpBuffer->szSpellName, "Hunger", SPELL_MAX_NAME);
		break;

	case D2S_WEREBEAR:			
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;	
		lpBuffer->dwCoolDown = 1000;		

		strncpy(lpBuffer->szSpellName, "Werebear", SPELL_MAX_NAME);
		break;

	case D2S_MAUL:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;


		strncpy(lpBuffer->szSpellName, "Maul", SPELL_MAX_NAME);
		break;

	case D2S_SHOCKWAVE:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;

		strncpy(lpBuffer->szSpellName, "Shock Wave", SPELL_MAX_NAME);
		break;


		// Summoning
	case D2S_OAKSAGE:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Oak Sage", SPELL_MAX_NAME);
		break;

	case D2S_HEARTOFWOLVERINE:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Heart of Wolverine", SPELL_MAX_NAME);
		break;

	case D2S_SPIRITOFBARBS:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Spirit of Barbs", SPELL_MAX_NAME);
		break;

	case D2S_RAVEN:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Raven", SPELL_MAX_NAME);
		break;

	case D2S_SUMMONSPIRITWOLF:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Summon Spirit Wolf", SPELL_MAX_NAME);
		break;

	case D2S_SUMMONDIREWOLF:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Summon Dire Wolf", SPELL_MAX_NAME);
		break;

	case D2S_SUMMONGRIZZLY:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Summon Grizzly", SPELL_MAX_NAME);
		break;

	case D2S_POISONCREEPER:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Poison Creeper", SPELL_MAX_NAME);
		break;

	case D2S_CARRIONVINE:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Carrion Vine", SPELL_MAX_NAME);
		break;

	case D2S_SOLARCREEPER:
		lpBuffer->nClass = CLASS_DRU;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Solar Creeper", SPELL_MAX_NAME);
		break;


		///////////////////////////////////////////////////////////////
		//    Necromaner Skills
		///////////////////////////////////////////////////////////////

		// Summoning Spells
	case D2S_SKELETONMASTERY:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Skeleton Mastery", SPELL_MAX_NAME);
		break;

	case D2S_GOLEMMASTERY:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Golem Mastery", SPELL_MAX_NAME);
		break;

	case D2S_SUMMONRESIST:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Summon Resist", SPELL_MAX_NAME);
		break;

	case D2S_CLAYGOLEM:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Clay Golem", SPELL_MAX_NAME);
		break;

	case D2S_BLOODGOLEM:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Blood Golem", SPELL_MAX_NAME);
		break;

	case D2S_IRONGOLEM:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Iron Golem", SPELL_MAX_NAME);
		break;

	case D2S_FIREGOLEM:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Fire Golem", SPELL_MAX_NAME);
		break;

	case D2S_RAISESKELETON:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Raise Skeleton", SPELL_MAX_NAME);
		break;

	case D2S_RAISESKELETALMAGE:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Raise Skeletal Mage", SPELL_MAX_NAME);
		break;

	case D2S_REVIVE:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;

		strncpy(lpBuffer->szSpellName, "Revive", SPELL_MAX_NAME);
		break;


		// Poison and Bone Spells
	case D2S_POISONDAGGER:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_POISON;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;

		strncpy(lpBuffer->szSpellName, "Poison Dagger", SPELL_MAX_NAME);
		break;

	case D2S_POISONEXPLOSION:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
		lpBuffer->dwSpellInfoFlag |= DSI_POISON;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Poison Explosion", SPELL_MAX_NAME);
		break;

	case D2S_POISONNOVA:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_POISON;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Poison Nova", SPELL_MAX_NAME);
		break;

	case D2S_TEETH:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Teeth", SPELL_MAX_NAME);
		break;

	case D2S_CORPOSEEXPLOSION:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_NEEDCORPSE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Corpose Explosion", SPELL_MAX_NAME);
		break;

	case D2S_BONESPEAR:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Bone Spear", SPELL_MAX_NAME);
		break;

	case D2S_BONESPIRIT:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_GUIDED;

		strncpy(lpBuffer->szSpellName, "Bone Spirit", SPELL_MAX_NAME);
		break;

	case D2S_BONEARMOR:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Bone Armor", SPELL_MAX_NAME);
		break;

	case D2S_BONEWALL:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Bone Wall", SPELL_MAX_NAME);
		break;

	case D2S_BONEPRISON:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Bone Prison", SPELL_MAX_NAME);
		break;


		// Curses
	case D2S_DIMVISION:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Dim Vision", SPELL_MAX_NAME);
		break;

	case D2S_CONFUSE:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Confuse", SPELL_MAX_NAME);
		break;

	case D2S_ATTRACT:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Attract", SPELL_MAX_NAME);
		break;

	case D2S_AMPLIFYDAMAGE:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Amplify Damage", SPELL_MAX_NAME);
		break;

	case D2S_IRONMAIDEN:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Iron Maiden", SPELL_MAX_NAME);
		break;

	case D2S_LIFETAP:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Life Tap", SPELL_MAX_NAME);
		break;

	case D2S_LOWERRESIST:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Lower Resist", SPELL_MAX_NAME);
		break;

	case D2S_WEAKEN:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Weaken", SPELL_MAX_NAME);
		break;

	case D2S_TERROR:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Terror", SPELL_MAX_NAME);
		break;

	case D2S_DECREPIFY:
		lpBuffer->nClass = CLASS_NEC;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Decrepify", SPELL_MAX_NAME);
		break;



		///////////////////////////////////////////////////////////////
		//    Paladin Skills
		///////////////////////////////////////////////////////////////

		// Defensive Auras
	case D2S_PRAYER:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Prayer", SPELL_MAX_NAME);
		break;

	case D2S_CLEANSING:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Cleansing", SPELL_MAX_NAME);
		break;

	case D2S_MEDITATION:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Meditation", SPELL_MAX_NAME);
		break;

	case D2S_DEFIANCE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Defiance", SPELL_MAX_NAME);
		break;

	case D2S_VIGOR:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Vigor", SPELL_MAX_NAME);
		break;

	case D2S_REDEMPTION:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Redemption", SPELL_MAX_NAME);
		break;

	case D2S_RESISTFIRE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Resist Fire", SPELL_MAX_NAME);
		break;

	case D2S_RESISTCOLD:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Resist Cold", SPELL_MAX_NAME);
		break;

	case D2S_RESISTLIGHTNING:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Resist Lightning", SPELL_MAX_NAME);
		break;

	case D2S_SALVATION:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Salvation", SPELL_MAX_NAME);
		break;


		// Offensive Auras
	case D2S_MIGHT:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Might", SPELL_MAX_NAME);
		break;

	case D2S_BLESSEDAIM:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Blessed Aim", SPELL_MAX_NAME);
		break;

	case D2S_CONCENTRATION:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Concentration", SPELL_MAX_NAME);
		break;

	case D2S_FANATICISM:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Fanaticism", SPELL_MAX_NAME);
		break;

	case D2S_HOLYFIRE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;

		strncpy(lpBuffer->szSpellName, "Holy Fire", SPELL_MAX_NAME);
		break;

	case D2S_HOLYFREEZE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Holy Freeze", SPELL_MAX_NAME);
		break;

	case D2S_HOLYSHOCK:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;

		strncpy(lpBuffer->szSpellName, "Holy Shock", SPELL_MAX_NAME);
		break;

	case D2S_THORNS:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Thorns", SPELL_MAX_NAME);
		break;

	case D2S_SANCTUARY:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Sanctuary", SPELL_MAX_NAME);
		break;

	case D2S_CONVICTION:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Conviction", SPELL_MAX_NAME);
		break;


		// Combat Skills
	case D2S_SACRIFICE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;			

		strncpy(lpBuffer->szSpellName, "Sacrifice", SPELL_MAX_NAME);
		break;

	case D2S_ZEAL:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Zeal", SPELL_MAX_NAME);
		break;

	case D2S_VENGEANCE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;	

		strncpy(lpBuffer->szSpellName, "Vengeance", SPELL_MAX_NAME);
		break;

	case D2S_CONVERSION:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;				

		strncpy(lpBuffer->szSpellName, "Conversion", SPELL_MAX_NAME);
		break;

	case D2S_HOLYBOLT:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Holy Bolt", SPELL_MAX_NAME);
		break;

	case D2S_BLESSEDHAMMER:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MAGICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Blessed Hammer", SPELL_MAX_NAME);
		break;

	case D2S_FISTOFTHEHEAVENS:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_GUIDED;
		lpBuffer->dwCoolDown = 1000;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Fist of The Heavens", SPELL_MAX_NAME);
		break;

	case D2S_SMITE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;

		strncpy(lpBuffer->szSpellName, "Smite", SPELL_MAX_NAME);
		break;

	case D2S_CHARGE:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
		lpBuffer->dwSpellInfoFlag |= DSI_MELEE;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PHYSICAL;	

		strncpy(lpBuffer->szSpellName, "Charge", SPELL_MAX_NAME);
		break;

	case D2S_HOLYSHIELD:
		lpBuffer->nClass = CLASS_PAL;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Holy Shield", SPELL_MAX_NAME);
		break;


		///////////////////////////////////////////////////////////////
		//    Sorceress Skills
		///////////////////////////////////////////////////////////////

		// Cold Spells
	case D2S_FROSTNOVA:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Frost Nova", SPELL_MAX_NAME);
		break;

	case D2S_BLIZZARD:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwCoolDown = 1800;

		strncpy(lpBuffer->szSpellName, "Blizzard", SPELL_MAX_NAME);
		break;

	case D2S_FROZENORB:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwCoolDown = 1000;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Frozen Orb", SPELL_MAX_NAME);
		break;

	case D2S_ICEBOLT:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Ice Bolt", SPELL_MAX_NAME);
		break;

	case D2S_ICEBLAST:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Ice Blast", SPELL_MAX_NAME);
		break;

	case D2S_GLACIALSPIKE:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_COLD;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Glacial Spike", SPELL_MAX_NAME);
		break;

	case D2S_COLDMASTERY:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Cold Mastery", SPELL_MAX_NAME);
		break;

	case D2S_FROZENARMOR:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Frozen Armor", SPELL_MAX_NAME);
		break;

	case D2S_SHIVERARMOR:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Shiver Armor", SPELL_MAX_NAME);
		break;

	case D2S_CHILLINGARMOR:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Chilling Armor", SPELL_MAX_NAME);
		break;

		// Lightning Spells
	case D2S_STATICFIELD:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Static Field", SPELL_MAX_NAME);
		break;

	case D2S_NOVA:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Nova", SPELL_MAX_NAME);
		break;

	case D2S_THUNDERSTORM:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Thunder Storm", SPELL_MAX_NAME);
		break;

	case D2S_CHARGEDBOLT:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Charged Bolt", SPELL_MAX_NAME);
		break;

	case D2S_LIGHTNING:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Lightning", SPELL_MAX_NAME);
		break;

	case D2S_CHAINLIGHTNING:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Chain Lightning", SPELL_MAX_NAME);
		break;

	case D2S_LIGHTNINGMASTERY:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Lightning Mastery", SPELL_MAX_NAME);
		break;

	case D2S_TELEKINESIS:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_LIGHTNING;
		lpBuffer->dwSpellInfoFlag |= DSI_STUN;
		lpBuffer->dwSpellInfoFlag |= DSI_KNOCKBACK;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;			

		strncpy(lpBuffer->szSpellName, "Telekinesis", SPELL_MAX_NAME);
		break;

	case D2S_TELEPORT:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;

		strncpy(lpBuffer->szSpellName, "Teleport", SPELL_MAX_NAME);
		break;

	case D2S_ENERGYSHIELD:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN;

		strncpy(lpBuffer->szSpellName, "Energy Shield", SPELL_MAX_NAME);
		break;


		// Fire Spells
	case D2S_INFERNO:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Inferno", SPELL_MAX_NAME);
		break;

	case D2S_BLAZE:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Blaze", SPELL_MAX_NAME);
		break;

	case D2S_FIREWALL:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwCoolDown = 1400;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Fire Wall", SPELL_MAX_NAME);
		break;

	case D2S_FIREBOLT:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;

		strncpy(lpBuffer->szSpellName, "Fire Bolt", SPELL_MAX_NAME);
		break;

	case D2S_FIREBALL:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;

		strncpy(lpBuffer->szSpellName, "Fire Ball", SPELL_MAX_NAME);
		break;

	case D2S_METEOR:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_AFFECTGROUP;
		lpBuffer->dwCoolDown = 1200;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSTHROUGH;
		strncpy(lpBuffer->szSpellName, "Meteor", SPELL_MAX_NAME);
		break;

	case D2S_FIREMASTERY:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Fire Mastery", SPELL_MAX_NAME);
		break;

	case D2S_WARMTH:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_PASSIVE;

		strncpy(lpBuffer->szSpellName, "Warmth", SPELL_MAX_NAME);
		break;

	case D2S_ENCHANT:
		lpBuffer->nClass = CLASS_SOR;
		//lpBuffer->dwSpellInfoFlag |= DSI_ENABLEINTOWN; // no longer in 1.10
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_PARTYCAST;

		strncpy(lpBuffer->szSpellName, "Enchant", SPELL_MAX_NAME);
		break;

	case D2S_HYDRA:
		lpBuffer->nClass = CLASS_SOR;
		lpBuffer->dwSpellInfoFlag |= DSI_RIGHTONLY;
		lpBuffer->dwSpellInfoFlag |= DSI_TARGETABLE;
		lpBuffer->dwSpellInfoFlag |= DSI_FIRE;
		lpBuffer->dwSpellInfoFlag |= DSI_SUMMON;
		lpBuffer->dwCoolDown = 2000;

		strncpy(lpBuffer->szSpellName, "Hydra", SPELL_MAX_NAME);
		break;


	default:
		lpBuffer->nClass = CLASS_NA;
		lpBuffer->wSpellID = D2S_INVALID;
		return FALSE;
		break;
	}

	return TRUE;
}