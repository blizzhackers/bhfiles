#define ATTACKTYPE_LEFT				0xC5
#define ATTACKTYPE_SHIFTLEFT		0xE5
#define ATTACKTYPE_RIGHT			0x46
#define ATTACKTYPE_SHIFTRIGHT		0x66

typedef struct TargetInfo_t
{
	CHAR PlayerName[0x10];
	DWORD UnitId;
	DWORD ClassId;
	BYTE Level;
	BYTE Life;
	BOOL Flashing;
} TARGETINFO, *PTARGETINFO;

typedef struct Blind_t
{
	DOUBLE X;
	DOUBLE Y;
} BLIND, *PBLIND;

typedef struct PacketedAttack_t
{
	WORD SkillID;
	BOOL Shift;
} PACKETEDATTACK, *PPACKETEDATTACK;

BOOL AttackNearestTarget();
BOOL AttackTarget(DWORD UnitID, DWORD UnitType);
BOOL EnumeratePlayers();
BOOL MiddleMouseButton();
BOOL Knockback();
BOOL NextTarget();
BOOL PreviousTarget();
BOOL Reblind();
VOID DestroyTargets();