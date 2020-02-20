#include "Hack.h"

CPathFinder::CPathFinder()
{
	m_pMap = NULL;
	m_nSizeX = 0;
	m_nSizeY = 0;
	memset(&m_ptStart, 0, sizeof(POINT));
	memset(&m_ptEnd, 0, sizeof(POINT));
}

CPathFinder::~CPathFinder()
{
}

BOOL CPathFinder::CreateBestPath(POINT ptStart, POINT ptEnd, LPPOINT lpBuffer, DWORD dwMaxCount)
{
	POINT ptPos;
	INT nFlag, nIterations = 0;
	DWORD dwFound;

	if(lpBuffer == NULL || dwMaxCount == 0 || m_nSizeX <= 0 || m_nSizeY <= 0 || m_pMap == NULL)
		return 0;

	if(ptStart.x > m_nSizeX || ptStart.y > m_nSizeY || ptEnd.x > m_nSizeX || ptEnd.y > m_nSizeY)
		return 0;

	memset(lpBuffer, 0, sizeof(POINT) * dwMaxCount);
	m_ptStart = ptStart;
	m_ptEnd = ptEnd;

	CreateDistanceTable();

	lpBuffer[0] = ptStart;
	dwFound = 1;
	ptPos = ptStart;

	nFlag = GetNextMove(ptPos);

	while(nFlag != PATH_FAILED && dwFound < dwMaxCount)
	{
		if(nIterations > 5000)
			return 0;

		if(nFlag == PATH_REACHED)
		{
			lpBuffer[dwFound] = ptEnd;
			dwFound++;
			return dwFound;
		}

		int nRedundancy = CheckRedundancy(lpBuffer, dwFound, ptPos);
		if(nRedundancy == -1)
		{
			lpBuffer[dwFound] = ptPos;
			dwFound++;
		}
		else
		{
			dwFound = nRedundancy + 1;
			lpBuffer[dwFound] = ptPos;
		}	

		nFlag = GetNextMove(ptPos);
		nIterations++;
	}

	return FALSE;
}

VOID CPathFinder::SetPathingMap(LPWORD *pMap, INT nSizeX, INT nSizeY)
{
	m_pMap = pMap;
	m_nSizeX = nSizeX;
	m_nSizeY = nSizeY;
}

PATHRETURN CPathFinder::GetNextMove(POINT& pos, int nAdjust)
{	
	if(CalculateDistance(m_ptEnd, pos) <= TELEPORT_RANGE)
	{
		pos = m_ptEnd;
		return PATH_REACHED;
	}

	if (!IsValidIndex(pos.x, pos.y))
		return PATH_FAILED;

	BlockLocation(pos, nAdjust);

	POINT p, best;
	int value = RANGE_INVALID;

	for (p.x = pos.x - TELEPORT_RANGE; p.x <= pos.x + TELEPORT_RANGE; p.x++)
	{
		for (p.y = pos.y - TELEPORT_RANGE; p.y <= pos.y + TELEPORT_RANGE; p.y++)
		{
			if (!IsValidIndex(p.x, p.y))
				continue;

			if (m_pMap[p.x][p.y] < value && CalculateDistance(p, pos) <= TELEPORT_RANGE)
			{				
				value = m_pMap[p.x][p.y];
				best = p;
			}
		}
	}

	if (value >= RANGE_INVALID)
		return PATH_FAILED;

	pos = best;
	BlockLocation(pos, nAdjust);	
	return PATH_CONTINUE;
}

BOOL CPathFinder::CreateDistanceTable()
{
	if(m_pMap == NULL)
		return FALSE;

	for (int x = 0; x < m_nSizeX; x++)	
	{
		for (int y = 0; y < m_nSizeY; y++)
		{
			if ((m_pMap[x][y] % 2) == 0)
				m_pMap[x][y] = (SHORT)CalculateDistance(x, y, m_ptEnd.x, m_ptEnd.y);
			else
				m_pMap[x][y] = RANGE_INVALID;
		}
	}

	m_pMap[m_ptEnd.x][m_ptEnd.y] = 1;	
	return TRUE;
}

BOOL CPathFinder::CheckRedundancy(LPPOINT lpPath, INT nCount, POINT ptPos)
{
	if(lpPath == NULL || nCount == 0)
		return -1;

	for(INT i = 0; i < nCount; i++)
	{
		if(CalculateDistance(ptPos, lpPath[i]) <= (TELEPORT_RANGE / 3))
			return i;
	}

	return -1;
}

BOOL CPathFinder::IsValidIndex(INT nX, INT nY)
{
	return nX >= 0 && nX < m_nSizeX && nY >= 0 && nY < m_nSizeY;
}

VOID CPathFinder::BlockLocation(POINT pos, INT nRange)
{
	nRange = max(nRange, 1);

	for(INT i = pos.x - nRange; i < pos.x + nRange; i++)
	{
		for(INT j = pos.y - nRange; j < pos.y + nRange; j++)
		{
			if (IsValidIndex(i, j))
				m_pMap[i][j] = RANGE_INVALID;
		}
	}
}

POINT PickFarcastLocation(POINT Target)
{
	POINT Origin = {(Me->pPath->xPos), (Me->pPath->yPos)};
	V_Design->ScreenToAutoMap(&Target);

	INT Angle = CalculateAngle(Origin, Target);
	INT Radius = 100;
	POINT Cast = {NULL};

	while (TRUE)
	{
		Cast = CalculatePointOnTrack(Origin, Radius--, Angle);

		if (!D2COMMON_CheckCollision(Me->pPath->pRoom1, Cast.x, Cast.y, 2))
			if (::abs(Me->pPath->xPos - Cast.x) <= 50 && ::abs(Me->pPath->yPos - Cast.y) <= 50)
				break;
	}

	return Cast;
}

BOOL Teleport(WORD X, WORD Y)
{
	if (!ClientReady(TRUE))
		return FALSE;

	LPBYTE Packet = new BYTE[5];
	Packet[0] = 0x5F;
	*(LPWORD)&Packet[1] = X;
	*(LPWORD)&Packet[3] = Y;
	D2NET_SendPacket(5, 1, Packet);
	delete [] Packet;

	LPBYTE Update = new BYTE[9];
	Update[0] = 0x4B;
	*(LPDWORD)&Update[1] = Me->dwType;
	*(LPDWORD)&Update[5] = Me->dwUnitId;
	D2NET_SendPacket(9, 1, Update);
	delete [] Update;
	return TRUE;
}