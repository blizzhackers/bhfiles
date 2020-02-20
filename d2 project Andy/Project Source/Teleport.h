#define RANGE_INVALID	10000
#define TELEPORT_RANGE	33

enum PATHRETURN
{
	PATH_REACHED = 0,
	PATH_CONTINUE,
	PATH_FAILED,
};

class CPathFinder
{
public:
	
	CPathFinder();
	virtual ~CPathFinder();

	VOID SetPathingMap(LPWORD *pMap, INT nSizeX, INT nSIzeY);
	BOOL CreateBestPath(POINT ptStart, POINT ptTarget, LPPOINT ptPath, DWORD dwPath);

protected:
	PATHRETURN GetNextMove(POINT& ptTarget, INT nAdjust = 2);
	BOOL CreateDistanceTable();

	BOOL IsValidIndex(INT nX, INT nY);
	VOID BlockLocation(POINT ptPos, INT nRange);
	BOOL CheckRedundancy(LPPOINT lpPath, INT nCount, POINT ptPos);
	
	LPWORD *m_pMap;
	INT m_nSizeX;
	INT m_nSizeY;
	POINT m_ptStart, m_ptEnd;
};

typedef struct VectorTarget_t
{
	POINT Position;
	INT Type;
	DWORD Target;
	DWORD TargetType;
} VECTOR, *LPVECTOR;

VOID DestroyVectors();
BOOL EnumerateVectors();
POINT PickFarcastLocation(POINT Target);
BOOL Teleport(WORD X, WORD Y);
BOOL TeleportToVector(INT VectorType);