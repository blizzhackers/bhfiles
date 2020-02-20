#ifndef _SKILL_H
#define _SKILL_H

#define SPELL_MAX_NAME 25

typedef struct D2SpellInfo_t
{
	WORD wSpellID;
	INT nClass;
	DWORD dwSpellInfoFlag;
	CHAR szSpellName[SPELL_MAX_NAME + 1];
	DWORD dwCoolDown;
} D2SpellInfo;

WORD GetCurrentSkill(BOOL Left);
INT GetSkill(WORD SkillId);
BOOL FASTCALL OnUseSkill(LPATTACKSTRUCT Attack, BOOL AttackingUnit);
BOOL UsingSkill();
BOOL SetSkill(WORD SkillId, BOOL Left);
BOOL GetSkillInfo(WORD wSpell, D2SpellInfo *lpBuffer);

#endif