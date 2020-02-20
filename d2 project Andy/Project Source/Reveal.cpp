#include "Hack.h"

Reveal::Reveal()
{
	m_ActLevels[0] = 1;
	m_ActLevels[1] = 40;
	m_ActLevels[2] = 75;
	m_ActLevels[3] = 103;
	m_ActLevels[4] = 109;
	m_ActLevels[5] = 137;

	memset(m_RevealedActs, 0, sizeof(UCHAR) * 5);
	m_LastLevel = -1;
}

Reveal::~Reveal()
{
}

VOID Reveal::RevealAutomap()
{
	LPUNITANY pPlayer = *p_D2CLIENT_PlayerUnit;
	LPACT pAct = pPlayer->pAct;
	DWORD dwAct = pPlayer->pAct->dwAct;

	if (V_AutoRevealAutomap && m_RevealedActs[dwAct])
		return;

	for(INT nLevel = m_ActLevels[dwAct]; nLevel < m_ActLevels[dwAct + 1]; nLevel++)
	{
		LPLEVEL pLevel = GetLevel(pAct->pMisc, nLevel);

		if (!pLevel)
			break;

		if (!pLevel->pRoom2First)
			D2COMMON_InitLevel(pLevel);

		InitAutomapLayer(nLevel);
		RevealLevel(pLevel);
	}

	InitAutomapLayer(pPlayer->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);
	m_RevealedActs[dwAct] = TRUE;
}

VOID Reveal::RevealLevel(LPLEVEL pLevel)
{
	for(LPROOM2 pRoom2 = pLevel->pRoom2First; pRoom2; pRoom2 = pRoom2->pRoom2Next)
		RevealRoom(pRoom2);
}

VOID Reveal::RevealRoom(LPROOM2 pRoom2)
{
	BOOL bAdded = FALSE;

	if(pRoom2->dwPresetType == 2)
	{
		DWORD dwPresetNo = *pRoom2->pType2Info;

		if(dwPresetNo == 38 || dwPresetNo == 39) return;
		if(dwPresetNo >= 401 && dwPresetNo <= 405) return;
		if(dwPresetNo == 836 || dwPresetNo == 863) return;
	}

	if(!pRoom2->pRoom1)
	{
		AddAutomapRoom(pRoom2);
		bAdded = TRUE;
	}

	RevealRoom1(pRoom2);

	if(bAdded)
		RemoveAutomapRoom(pRoom2);
}


VOID Reveal::RevealRoom1(LPROOM2 pRoom)
{
	D2CLIENT_RevealAutomapRoom(pRoom->pRoom1, 1, *p_D2CLIENT_AutomapLayer);

	for(LPPRESETUNIT pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
	{
		INT nCell = 0;

		if(pUnit->dwType == UNIT_TYPE_NPC)
		{
			if(pUnit->dwTxtFileNo == 256) nCell = 300;
			if(pUnit->dwTxtFileNo == 745) nCell = 745;
		}

		if(pUnit->dwType == UNIT_TYPE_OBJECT)
		{
			nCell = GetUnitCellNumber(pUnit->dwTxtFileNo, pRoom->pLevel->dwLevelNo);
		}

		if(pUnit->dwType == UNIT_TYPE_TILE)
		{
			DWORD dwTargetLevel = GetTileLevelNo(pRoom, pUnit->dwTxtFileNo);
			if(dwTargetLevel)
			{
				for(INT i = 0; i < m_LevelExits.GetSize(); i++)
				{
					if((m_LevelExits[i]->ptPos.x == (pRoom->dwPosX * 5) + pUnit->dwPosX) && 
						(m_LevelExits[i]->ptPos.y == (pRoom->dwPosY * 5) + pUnit->dwPosY))
						return;
				}

				LPCAVEDESC LevelDesc = new CAVEDESC;
				CHAR szLevel[0x40] = "";

				::memset(LevelDesc, 0, sizeof(CAVEDESC));
				GetMapName((INT)dwTargetLevel, szLevel, 0x40);

				sprintf_s(LevelDesc->szName, szLevel);
				LevelDesc->ptPos.x = (pRoom->dwPosX * 5) + pUnit->dwPosX - (8 << 1);
				LevelDesc->ptPos.y = (pRoom->dwPosY * 5) + pUnit->dwPosY - 10;
				LevelDesc->dwAct = Me->pAct->dwAct;
				LevelDesc->dwLevelNo = pRoom->pLevel->dwLevelNo;
				LevelDesc->dwTargetLevelNo = dwTargetLevel;

				if(dwTargetLevel == pRoom->pLevel->pMisc->dwStaffTombLevel)
					strcat_s(LevelDesc->szName, V_StaffTombIndicator);

				m_LevelExits.Add(LevelDesc);
			}
		}

		if(nCell)
			AddRoomCell(pUnit->dwPosX, pUnit->dwPosY, nCell, pRoom);
	}
}

VOID Reveal::AddMyAutomapCell(LPAUTOMAPCELL cell)
{
	LPMYAUTOMAPLAYERMAN pMyLayerMan = &V_MyAutoMapLayer[AutoMapLayer->nLayerNo];
	LPMYAUTOMAPLAYER * ppMyLayer = &pMyLayerMan->pMyLayer;

	while (*ppMyLayer && (*ppMyLayer)->pNextChunk)
		ppMyLayer = &(*ppMyLayer)->pNextChunk;

	if (*ppMyLayer && pMyLayerMan->wCellNodeNum < MYCELL_CHUNK_NUM)
	{
		(*ppMyLayer)->aMyCell[pMyLayerMan->wCellNodeNum].nCellNo = cell->nCellNo;
		(*ppMyLayer)->aMyCell[pMyLayerMan->wCellNodeNum].xPixel = cell->xPixel;
		(*ppMyLayer)->aMyCell[pMyLayerMan->wCellNodeNum].yPixel = cell->yPixel;
		pMyLayerMan->wCellNodeNum++;
	}

	else
	{
		if (*ppMyLayer)
			ppMyLayer = &(*ppMyLayer)->pNextChunk;

		*ppMyLayer = new MyAutomapLayer;
		memset(*ppMyLayer, 0, sizeof(MyAutomapLayer));
		pMyLayerMan->wCellChunkNum++;
		(*ppMyLayer)->aMyCell[0].nCellNo = cell->nCellNo;
		(*ppMyLayer)->aMyCell[0].xPixel = cell->xPixel;
		(*ppMyLayer)->aMyCell[0].yPixel = cell->yPixel;
		pMyLayerMan->wCellNodeNum = 1;
	}
}

VOID Reveal::DestroyMyAutomapCells()
{
	for (INT i = 0; i < ArraySize(V_MyAutoMapLayer); i++)
	{
		LPMYAUTOMAPLAYER pMyLayer = V_MyAutoMapLayer[i].pMyLayer;

		while(pMyLayer)
		{
			MyAutomapLayer *pTemp = pMyLayer->pNextChunk;
			delete pMyLayer;
			pMyLayer = pTemp;
		}
	}

	::memset(V_MyAutoMapLayer, 0, sizeof(V_MyAutoMapLayer));
}

VOID Reveal::DoRemoveMyAutomapCell(AutomapCell* cell)
{
	if (cell->nCellNo >= CELLNO_MYSHRINES && cell->nCellNo < CELLNO_MYSHRINES+23)
		cell->nCellNo = CELLNO_SHRINE;

	else if ((SHORT)cell->nCellNo < 0)
		cell->nCellNo = 0;
}

VOID Reveal::NextRemoveMyAutomapCell(AutomapCell* cell)
{
	if (cell)
	{
		NextRemoveMyAutomapCell(cell->pLess);
		DoRemoveMyAutomapCell(cell);
		NextRemoveMyAutomapCell(cell->pMore);
	}
}

VOID Reveal::RemoveMyAutomapCells()
{
	for (AutomapLayer *pAutomapLayer = *p_D2CLIENT_AutomapLayerList; pAutomapLayer != NULL; pAutomapLayer = pAutomapLayer->pNextLayer)
		NextRemoveMyAutomapCell(pAutomapLayer->pObjects);
}

VOID Reveal::DeleteShrines()
{
	for (INT i = 0; i < 2; i++)
	{
		for (INT j = 0; j < TOTALSHRINES; j++)
		{
			if (V_MyShrineCells[i][j])
				DeleteCellFile(V_MyShrineCells[i][j]);
		}
	}
	
	if (V_WaypointCell)
		DeleteCellFile(V_WaypointCell);
	
	if (V_MinimapObjectCell)
		DeleteCellFile(V_MinimapObjectCell);
}

VOID STDCALL Reveal::FilterOutMyAutomapCells(LPAUTOMAPCELL Cell)
{
	if (Cell)
	{
		FilterOutMyAutomapCells(Cell->pLess);

		if (Cell->nCellNo >= CELLNO_MYSHRINES && Cell->nCellNo < CELLNO_MYSHRINES + 23)
			Cell->fSaved = 1;

		else if ((SHORT)Cell->nCellNo < 0)
			Cell->fSaved = 1;

		FilterOutMyAutomapCells(Cell->pMore);
	}
}

LPMYAUTOMAPCELL Reveal::GetMyAutomapCell(INT Index)
{
	LPMYAUTOMAPLAYERMAN MyLayerMan = &V_MyAutoMapLayer[AutoMapLayer->nLayerNo];

	INT Chunk = Index / MYCELL_CHUNK_NUM;
	INT ID = Index % MYCELL_CHUNK_NUM;

	LPMYAUTOMAPLAYER MyLayer = MyLayerMan->pMyLayer;
	
	for (INT j = 0; MyLayer && j < Chunk; j++)
		MyLayer = MyLayer->pNextChunk;
	
	return MyLayer ? &MyLayer->aMyCell[ID] : NULL;
}

VOID Reveal::InitShrines()
{
	CHAR FileName[MAX_PATH];
	
	for (INT i = 0; i < 2; i++)
	{
		for (INT j = 0; j < TOTALSHRINES; j++)
		{
			wsprintfA(GetShrinesFolder(FileName, '\\'), "MINISHRINE%.2d.BMP" + (i ? 0 : 4), j);
			V_MyShrineCells[i][j] = InitCellFile(LoadBMPCellFile(FileName));
		}
	}
	
	wsprintfA(GetShrinesFolder(FileName, '\\'), "WAYPOINT.BMP");
	V_WaypointCell = InitCellFile(LoadBMPCellFile(FileName));

	wsprintfA(GetShrinesFolder(FileName, '\\'), "BLOBCELL.BMP");
	V_MinimapObjectCell = InitCellFile(LoadBMPCellFile(FileName));

	if (!V_MinimapObjectCell)
	{
		wsprintfA(GetShrinesFolder(FileName, '\\'), "BLOBDOT.BMP");
		V_MinimapObjectCell = InitCellFile(LoadBMPCellFile(FileName));
	}
}

VOID Reveal::InitTCList()
{
	LPMONSTERTXT mon;
	LPSUPERUNIQUETXT sup;

	for (INT i = 0; mon = D2CLIENT_GetMonsterTxt(i); i++)
	{
		TCTALLYS tallys;
		memset(&tallys, 0, sizeof(tallys));

		for (INT j = 0; j < 4; j++)
		{
			INT tcno = mon->tcs[D2CLIENT_GetDifficulty()][j];
			GetHighestTC(tcno, &tallys);
		}

		V_MonsterTCs[i] = tallys.groupnos[1];
	}

	for (INT j = 0; sup = D2COMMON_GetSuperUniqueTxt(j); j++)
	{
		TCTallys tallys;
		memset(&tallys, 0, sizeof(tallys));

		INT tcno = sup->tcs[D2CLIENT_GetDifficulty()];
		GetHighestTC(tcno, &tallys);
		V_SuperUniqueTCs[j] = tallys.groupnos[1];
	}
}

VOID Reveal::LoadMyAutomapLayer()
{
	LPMYAUTOMAPCELL MyCell;
	
	for (INT i = 0; (MyCell = GetMyAutomapCell(i)) != NULL; i++)
	{
		LPAUTOMAPCELL Cell = D2CLIENT_NewAutomapCell();

		Cell->nCellNo = MyCell->nCellNo;
		Cell->xPixel = MyCell->xPixel;
		Cell->yPixel = MyCell->yPixel;

		D2CLIENT_AddAutomapCell(Cell, &AutoMapLayer->pObjects);
	}
}

VOID Reveal::AddRoomCell(INT xPos, INT yPos, INT nCell, LPROOM2 pRoom)
{
	if(nCell == 376 && pRoom->pLevel->dwLevelNo == MAP_A4_RIVER_OF_FLAME)
		return;

	LPAUTOMAPCELL pCell = D2CLIENT_NewAutomapCell();

	xPos += pRoom->dwPosX * 5;
	yPos += pRoom->dwPosY * 5;

	pCell->nCellNo = nCell;
	pCell->xPixel = (((xPos - yPos) * 16) / 10) + 1;
	pCell->yPixel = (((xPos + yPos) * 8) / 10) - 3;
	D2CLIENT_AddAutomapCell(pCell, &((*p_D2CLIENT_AutomapLayer)->pObjects));
	AddMyAutomapCell(pCell);
}

VOID Reveal::AddAutomapRoom(LPROOM2 pRoom2)
{
	LPUNITANY pUnit = *p_D2CLIENT_PlayerUnit;
	D2COMMON_AddRoomData(pRoom2->pLevel->pMisc->pAct,
		pRoom2->pLevel->dwLevelNo,
		pRoom2->dwPosX,
		pRoom2->dwPosY,
		pUnit->pPath->pRoom1);
}

VOID Reveal::RemoveAutomapRoom(LPROOM2 pRoom2)
{
	LPUNITANY pUnit = *p_D2CLIENT_PlayerUnit;
	D2COMMON_RemoveRoomData(pRoom2->pLevel->pMisc->pAct,
		pRoom2->pLevel->dwLevelNo,
		pRoom2->dwPosX,
		pRoom2->dwPosY,
		pUnit->pPath->pRoom1);
}

BOOL Reveal::GetCaveExits(LPCAVEDESC *lpLevel, INT nMaxExits)
{
	INT nTotal = 0;

	for(INT i = 0; i < m_LevelExits.GetSize(); i++)
	{
		if(nTotal > nMaxExits)
			return FALSE;

		lpLevel[nTotal] = m_LevelExits[i];
		nTotal++;
	}

	return nTotal;
}

INT Reveal::GetUnitCellNumber(DWORD dwClassId, DWORD dwLevelNo)
{
	if(dwClassId == 397) return 318;
	if(dwClassId == 371) return 301;
	if(dwClassId == 152) return 300;
	if(dwClassId == 460) return 1468;

	//if(dwClassId == 473 && dwLevelNo == MAP_A5_FRIGID_HIGHLANDS) return 0;
	if(dwClassId == 580 && dwLevelNo == MAP_A3_LOWER_KURAST) return 318;
	if(dwClassId == 402 && dwLevelNo == MAP_A2_CANYON_OF_THE_MAGI) return 0;
	if(dwClassId == 376 && dwLevelNo == MAP_A4_RIVER_OF_FLAME) return 376;
	if(dwClassId == 267 && dwLevelNo != MAP_A3_KURAST_DOCKS && dwLevelNo != MAP_A4_THE_PANDEMONIUM_FORTRESS) return 0;

	if (dwClassId >= 574)
	{
		static WORD RandomObjectCells[9] =
		{
			CELLNO_MYSHRINES + 0,
			CELLNO_MYSHRINES + 7,
			CELLNO_MYSHRINES + 0,
			CELLNO_MYSHRINES + 12,
			CELLNO_MYSHRINES + 0,
			CELLNO_MYSHRINES + 14,
			NULL,
			NULL,
			NULL
		};

		return RandomObjectCells[dwClassId - 574];
	}

	LPOBJECTTXT pTxt = D2COMMON_GetObjectTxt(dwClassId);

	if (pTxt->nAutoMap == CELLNO_SHRINE)
	{
		static WORD Cells[4] =
		{
			CELLNO_MYSHRINES + 0,
			CELLNO_MYSHRINES + 2,
			CELLNO_MYSHRINES + 3,
			CELLNO_MYSHRINES + 0,
		};
		
		return (pTxt->nSubClass & 1) ? Cells[pTxt->nParm0] : 0;
	}

	return pTxt->nAutoMap;
}

DWORD Reveal::GetTileLevelNo(LPROOM2 lpRoom2, DWORD dwTileNo)
{
	for(LPROOMTILE pRoomTile = lpRoom2->pRoomTiles; pRoomTile; pRoomTile = pRoomTile->pNext)
	{
		if(*(pRoomTile->nNum) == dwTileNo)
			return pRoomTile->pRoom2->pLevel->dwLevelNo;
	}

	return NULL;
}

LPLEVEL Reveal::GetLevel(LPACTMISC pMisc, DWORD dwLevelNo)
{
	for(LPLEVEL pLevel = pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel)
		if (pLevel->dwLevelNo == dwLevelNo)
			return pLevel;

	return D2COMMON_GetLevel(pMisc, dwLevelNo);
}

LPAUTOMAPLAYER2 Reveal::InitAutomapLayer(DWORD levelno)
{
	LPAUTOMAPLAYER2 pLayer = D2COMMON_GetLayer(levelno);
	return (LPAUTOMAPLAYER2)InitAutomapLayerSTUB(pLayer->nLayerNo);
}

BOOL Reveal::CreateCollisionMap()
{
	if(GetUnitLevel(Me)->dwLevelNo == m_LastLevel)
		return TRUE;

	if(!Me)
		return FALSE;

	if(!Me->pPath->pRoom1)
		return FALSE;

	LPLEVEL pLevel = Me->pPath->pRoom1->pRoom2->pLevel;
	if(!pLevel)
		return FALSE;

	if(m_Map.IsCreated())
		m_Map.Destroy();

	m_LastLevel = pLevel->dwLevelNo;
	m_LevelOrigin.x = pLevel->dwPosX * 5;
	m_LevelOrigin.y = pLevel->dwPosY * 5;
	m_SizeX = pLevel->dwSizeX * 5;
	m_SizeY = pLevel->dwSizeY * 5;

	m_Map.Lock();
	if(!m_Map.Create(pLevel->dwSizeX * 5, pLevel->dwSizeY * 5, MAP_DATA_NULL))
	{
		m_Map.Unlock();
		return FALSE;
	}

	CArrayEx<DWORD, DWORD> aSkip;
	AddCollisionData(Me->pPath->pRoom1->pRoom2, aSkip);

	FillGaps();
	FillGaps();

	m_Map.Unlock();
	return TRUE;
}

BOOL Reveal::IsGap(INT nX, INT nY)
{
	if(m_Map[nX][nY] % 2)
		return FALSE;

	INT nSpaces = 0;
	INT i = 0;

	for(i = nX - 2; i <= nX + 2 && nSpaces < 3; i++)
	{
		if(i < 0 || i >= m_Map.GetCX() || (m_Map[i][nY] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	if(nSpaces < 3)
		return TRUE;

	nSpaces = 0;
	for(i = nY - 2; i <= nY + 2 && nSpaces < 3; i++)
	{
		if(i < 0 || i >= m_Map.GetCY() || (m_Map[nX][i] % 2))
			nSpaces = 0;
		else
			nSpaces++;
	}

	return nSpaces < 3;
}

BOOL Reveal::FillGaps()
{
	if(!m_Map.IsCreated())
		return FALSE;

	INT nCX = m_Map.GetCX();
	INT nCY = m_Map.GetCY();

	for(INT x = 0; x < nCX; x++)
	{
		for(INT y = 0; y < nCY; y++)
		{
			if(IsGap(x, y))
				m_Map[x][y] = MAP_DATA_FILL;
		}
	}

	return TRUE;
}

BOOL Reveal::AddCollisionData(LPCOLLMAP pColl)
{
	if(!pColl)
		return FALSE;

	INT nX = pColl->dwPosGameX - m_LevelOrigin.x;
	INT nY = pColl->dwPosGameY - m_LevelOrigin.y;
	INT nCX = pColl->dwSizeGameX;
	INT nCY = pColl->dwSizeGameY;

	if(!m_Map.IsValidIndex(nX, nY))
		return FALSE;

	INT nLimitX = nX + nCX;
	INT nLimitY = nY + nCY;

	LPWORD p = pColl->pMapStart;
	if(!p)
		return FALSE;

	for(INT y = nY; y < nLimitY; y++)
	{
		for(INT x = nX; x < nLimitX; x++)
		{
			m_Map[x][y] = *p;

			if(m_Map[x][y] == 1024)
				m_Map[x][y] = MAP_DATA_AVOID;

			p++;
		}
	}

	return TRUE;
}

BOOL Reveal::AddCollisionData(LPROOM2 pRoom2, CArrayEx<DWORD, DWORD>& aSkip)
{
	if(!pRoom2)
		return FALSE;

	if(pRoom2->pLevel->dwLevelNo != m_LastLevel)
		return FALSE;

	if(aSkip.Find((DWORD)pRoom2) != -1)
		return TRUE;

	aSkip.Add((DWORD)pRoom2);

	BOOL bAdd = FALSE;
	if(!pRoom2->pRoom1)
	{
		AddAutomapRoom(pRoom2);
		bAdd = TRUE;
	}

	if(pRoom2->pRoom1)
		AddCollisionData(pRoom2->pRoom1->Coll);

	LPROOM2 *pRoomNear = pRoom2->pRoom2Near;
	for(DWORD i = 0; i < pRoom2->dwRoomsNear; i++)
		AddCollisionData(pRoomNear[i], aSkip);

	if(bAdd)
		RemoveAutomapRoom(pRoom2);

	return TRUE;
}

BOOL Reveal::GetLevelExits(LPLEVELEXIT *lpLevel, INT nMaxExits)
{
	POINT ptExitPoints[0x40][2];
	INT nTotalPoints = 0, nCurrentExit = 0;

	for(INT i = 0; i < m_Map.GetCX(); i++)
	{
		if(!(m_Map[i][0] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = i;
			ptExitPoints[nTotalPoints][0].y = 0;

			for(i++; i < m_Map.GetCX(); i++)
			{
				if(m_Map[i][0] % 2)
				{
					ptExitPoints[nTotalPoints][1].x = i - 1;
					ptExitPoints[nTotalPoints][1].y = 0;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	for(INT i = 0; i < m_Map.GetCX(); i++)
	{
		if(!(m_Map[i][m_Map.GetCY() - 1] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = i;
			ptExitPoints[nTotalPoints][0].y = m_Map.GetCY() - 1;

			for(i++; i < m_Map.GetCX(); i++)
			{
				if((m_Map[i][m_Map.GetCY() - 1] % 2))
				{
					ptExitPoints[nTotalPoints][1].x = i - 1;
					ptExitPoints[nTotalPoints][1].y = m_Map.GetCY() - 1;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	for(INT i = 0; i < m_Map.GetCY(); i++)
	{
		if(!(m_Map[0][i] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = 0;
			ptExitPoints[nTotalPoints][0].y = i;

			for(i++; i < m_Map.GetCY(); i++)
			{
				if((m_Map[0][i] % 2))
				{
					ptExitPoints[nTotalPoints][1].x = 0;
					ptExitPoints[nTotalPoints][1].y = i - 1;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	for(INT i = 0; i < m_Map.GetCY(); i++)
	{
		if(!(m_Map[m_Map.GetCX() - 1][i] % 2))
		{
			ptExitPoints[nTotalPoints][0].x = m_Map.GetCX() - 1;
			ptExitPoints[nTotalPoints][0].y = i;

			for(i++; i < m_Map.GetCY(); i++)
			{
				if((m_Map[m_Map.GetCX() - 1][i] % 2))
				{
					ptExitPoints[nTotalPoints][1].x = m_Map.GetCX() - 1;
					ptExitPoints[nTotalPoints][1].y = i - 1;
					break;
				}
			}

			nTotalPoints++;
			break;
		}
	}

	LPPOINT ptCenters = new POINT[nTotalPoints];

	for(INT i = 0; i < nTotalPoints; i++)
	{
		INT nXDiff = ptExitPoints[i][1].x - ptExitPoints[i][0].x;
		INT nYDiff = ptExitPoints[i][1].y - ptExitPoints[i][0].y;
		INT nXCenter = 0, nYCenter = 0;

		if(nXDiff > 0)
		{
			if(nXDiff % 2)
				nXCenter = ptExitPoints[i][0].x + ((nXDiff - (nXDiff % 2)) / 2);
			else
				nXCenter = ptExitPoints[i][0].x + (nXDiff / 2);
		}

		if(nYDiff > 0)
		{
			if(nYDiff % 2)
				nYCenter = ptExitPoints[i][0].y + ((nYDiff - (nYDiff % 2)) / 2);
			else
				nYCenter = ptExitPoints[i][0].y + (nYDiff / 2);
		}

		ptCenters[i].x = nXCenter ? nXCenter : ptExitPoints[i][0].x;
		ptCenters[i].y = nYCenter ? nYCenter : ptExitPoints[i][0].y;
	}

	for(LPROOM2 pRoom = GetUnitLevel(Me)->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next)
	{
		LPROOM2 * pNear = pRoom->pRoom2Near;

		for(DWORD i = 0; i < pRoom->dwRoomsNear; i++)
		{
			if(pNear[i]->pLevel->dwLevelNo != GetUnitLevel(Me)->dwLevelNo)
			{
				INT nRoomX = pRoom->dwPosX * 5;
				INT nRoomY = pRoom->dwPosY * 5;

				for(INT j = 0; j < nTotalPoints; j++)
				{
					if((ptCenters[j].x + m_LevelOrigin.x) >= (WORD)nRoomX && (ptCenters[j].x + m_LevelOrigin.x) <= (WORD)(nRoomX + (pRoom->dwSizeX * 5)))
					{
						if((ptCenters[j].y + m_LevelOrigin.y) >= (WORD)nRoomY && (ptCenters[j].y + m_LevelOrigin.y) <= (WORD)(nRoomY + (pRoom->dwSizeY * 5)))
						{
							if(nCurrentExit >= nMaxExits)
								return FALSE;

							lpLevel[nCurrentExit] = new LEVELEXIT;
							lpLevel[nCurrentExit]->dwTargetLevel = pNear[i]->pLevel->dwLevelNo;
							lpLevel[nCurrentExit]->ptPos.x = ptCenters[j].x + m_LevelOrigin.x;
							lpLevel[nCurrentExit]->ptPos.y = ptCenters[j].y + m_LevelOrigin.y;
							nCurrentExit++;
						}
					}
				}

				break;
			}
		}

		BOOL bAdded = FALSE;

		if(!pRoom->pRoom1)
		{
			D2COMMON_AddRoomData(Me->pAct, pRoom->pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, Me->pPath->pRoom1);
			bAdded = TRUE;
		}
			
		for(PresetUnit* pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			if(nCurrentExit >= nMaxExits)
			{
				if(bAdded)
					D2COMMON_RemoveRoomData(Me->pAct, pRoom->pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, Me->pPath->pRoom1);

				return FALSE;
			}

			if(pUnit->dwType == UNIT_TYPE_TILE)
			{
				DWORD dwTargetLevel = GetTileLevelNo(pRoom, pUnit->dwTxtFileNo);
				
				if(dwTargetLevel)
				{
					BOOL Exists = FALSE;

					for(INT i = 0; i < nCurrentExit; i++)
					{
						if((lpLevel[i]->ptPos.x == (pRoom->dwPosX * 5) + pUnit->dwPosX) &&
							(lpLevel[i]->ptPos.y == (pRoom->dwPosY * 5) + pUnit->dwPosY))
							Exists = TRUE;
					}

					if (!Exists)
					{
						lpLevel[nCurrentExit] = new LEVELEXIT;
						lpLevel[nCurrentExit]->dwTargetLevel = dwTargetLevel;
						lpLevel[nCurrentExit]->ptPos.x = (pRoom->dwPosX * 5) + pUnit->dwPosX;
						lpLevel[nCurrentExit]->ptPos.y = (pRoom->dwPosY * 5) + pUnit->dwPosY;
						nCurrentExit++;
					}
				}
			}
		}

		if(bAdded)
			D2COMMON_RemoveRoomData(Me->pAct, pRoom->pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, Me->pPath->pRoom1);
	}

	return TRUE;
}

VOID Reveal::GetHighestTC(INT tcno, LPTCTALLYS tallys)
{
	LPTREASURECLASS tc = D2COMMON_GetTreasureClasses(tcno, 0);

	if (!tc) return;

	for (int i = 0; i < (int)tc->numitems; i++)
	{
		if (tc->items[i].tcflag)
		{
			if (tc->items[i].thingno <= 160)
			{
				int groupno = (tc->items[i].thingno-1)/32;
				int levno = (((tc->items[i].thingno-1)%32)+1)*3;
				if (levno > tallys->groupnos[groupno]) tallys->groupnos[groupno] = levno;
			}
			
			else
				GetHighestTC(tc->items[i].thingno, tallys);
		}
		
		else
		{
			LPITEMTXT itemtxt = D2COMMON_GetItemText(tc->items[i].thingno);

			if (itemtxt && itemtxt->nType == 74)
			{
				int runeno = (itemtxt->szCode[1]-'0')*10+(itemtxt->szCode[2]-'0');
				if (runeno > tallys->runeno) tallys->runeno = runeno;
			}
		}
	}
}

VOID Reveal::DestroyLevelExits()
{
	if (!m_LevelExits.IsEmpty())
	{
		for (INT i = 0; i < m_LevelExits.GetSize(); i++)
		{
			if (m_LevelExits[i])
				delete m_LevelExits[i];
		}

		m_LevelExits.RemoveAll();
	}
}

WORD Reveal::GetCollisionInfo(INT nX, INT nY)
{
	nX -= m_LevelOrigin.x;
	nY -= m_LevelOrigin.y;

	if(!m_Map.IsValidIndex(nX, nY))
		return NULL;

	return m_Map[nX][nY];
}

BOOL Reveal::ExportCollisionMap(CMatrix<WORD, WORD>& cMatrix)
{
	m_Map.Lock();
	m_Map.ExportData(cMatrix);
	m_Map.Unlock();

	return TRUE;
}

VOID STDCALL AutomapCells(LPCELLCONTEXT Context, DWORD X, DWORD Y, RECT * Clip, DWORD Bright)
{
	INT Cell = Context->nCellNo;

	if (V_ShrineImages && Cell == CELLNO_WAYPOINT && V_WaypointCell)
	{
		Context->nCellNo = 0;
		Context->pCellFile = V_WaypointCell;
		X += (D2CLIENT_GetAutomapSize() ? 4 : 8) - (V_WaypointCell->cells[0]->width / 2);
		Y += (D2CLIENT_GetAutomapSize() ? 4 : 0);
	}
	
	if (((Cell -= CELLNO_MYSHRINES) >= 0) && (Cell < TOTALSHRINES))
	{
		LPCELLFILE MyShrine = V_MyShrineCells[D2CLIENT_GetAutomapSize() & V_MiniShrine][Cell];
		
		if (V_ShrineImages && MyShrine && !V_Design->Hide)
		{
			Context->pCellFile = MyShrine;
			Context->nCellNo = 0;
			X += (D2CLIENT_GetAutomapSize() ? 4 : 8) - (MyShrine->cells[0]->width / 2);
		}
		
		else
			Context->nCellNo = CELLNO_SHRINE;
	}
	
	D2GFX_DrawAutomapCell(Context, X, Y, Clip, Bright);
}

VOID GetMonsterLevelTC(WCHAR * String, LPUNITANY Unit, DWORD Number)
{
	if (V_MonsterLifeBarLevel)
	{
		INT Lvl = D2COMMON_GetUnitStat(Unit, STAT_LEVEL, 0);

		if (Unit->pMonsterData->fBoss & 1)
		{
			Lvl += 3;

			PBYTE P = Unit->pMonsterData->anEnchants;

			for (INT i = 0; *P && i < ARRAYSIZE(Unit->pMonsterData->anEnchants); ++i, ++P)
			{
				if (*P == 16 || (*P >= 36 && *P <= 38))
					--Lvl;
			}
		}
		
		if (Lvl > 99)
			Lvl = 99;

		wsprintfW2(String + wcslen(String), " [L%d]", Lvl);
	}

	if (V_MonsterLifeBarTC)
	{
		BYTE TC;

		if (Unit->pMonsterData->fNormal & 1)
			TC = V_SuperUniqueTCs[Unit->pMonsterData->wUniqueNo];
		else
			TC = V_MonsterTCs[Unit->dwTxtFileNo];

		if (TC)
			wsprintfW2(String + wcslen(String), " [TC%d]", TC);
	}
}

VOID GetMonsterResists(WCHAR * String, LPUNITANY Unit, DWORD Number)
{
	if (V_MonsterLifeBarResists)
	{
		int dr = D2COMMON_GetUnitStat(Unit, STAT_DAMAGEREDUCTION, 0);
		int mr = D2COMMON_GetUnitStat(Unit, STAT_MAGICRESIST, 0);
		int fr = D2COMMON_GetUnitStat(Unit, STAT_FIRERESIST, 0);
		int lr = D2COMMON_GetUnitStat(Unit, STAT_LIGHTNINGRESIST, 0);
		int cr = D2COMMON_GetUnitStat(Unit, STAT_COLDRESIST, 0);
		int pr = D2COMMON_GetUnitStat(Unit, STAT_POISONRESIST, 0);

		if (dr | mr | fr | lr | cr | pr)
		{
			wsprintfW2(String + wcslen(String), " [Res:");

			if (dr)	wsprintfW2Colour(String + wcslen(String), 4, " D%d", dr); // Golden
			if (mr)	wsprintfW2Colour(String + wcslen(String), 8, " M%d", mr); // Deep yellow
			if (fr)	wsprintfW2Colour(String + wcslen(String), 1, " F%d", fr); // Red
			if (lr)	wsprintfW2Colour(String + wcslen(String), 9, " L%d", lr); // Light yellow
			if (cr)	wsprintfW2Colour(String + wcslen(String), 3, " C%d", cr); // Blue
			if (pr)	wsprintfW2Colour(String + wcslen(String), 2, " P%d", pr); // Green
			
			wsprintfW2(String + wcslen(String), "]");
		}
	}
}

LPUNITANY GetViewingUnit()
{
	if (!V_ViewingUnit)
		return Me;

	return V_ViewingUnit;
}

VOID FASTCALL ItemName(WCHAR * Name, LPUNITANY Item)
{
	if (!V_Design->Hide)
	{
		CHAR NaMe[1000];
		WideCharToMultiByte(CP_ACP, 0, Name, -1, NaMe, (INT)sizeof(NaMe), 0, 0);

		if (Item->pItemData->dwFlags & 0x400000 && V_Ethereal)
		{
			CHAR Ethereal[5] = "[E]";
			strcat_s(NaMe, sizeof(NaMe), Ethereal);
		}

		if (V_Sockets)
		{
			INT Sockets = GetUnitStat(Item, STAT_SOCKETS);

			if (Sockets > 0)
			{
				CHAR Socket[5];
				sprintf_s(Socket, sizeof(Socket), "[%d]", Sockets);
				strcat_s(NaMe, sizeof(NaMe), Socket);
			}
		}

		if (V_ItemLevel)
		{
			CHAR LvL[10];
			sprintf_s(LvL, sizeof(LvL), "[L%i]", Item->pItemData->dwItemLevel);
			strcat_s(NaMe, sizeof(NaMe), LvL);
		}

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, NaMe, (INT)strlen(NaMe), Name, (INT)strlen(NaMe) + 1);
	}
}

VOID FASTCALL OverrideShrine(LPAUTOMAPCELL New, LPAUTOMAPCELL Current)
{
	if ((New->nCellNo < CELLNO_MYSHRINES) || (New->nCellNo >= (CELLNO_MYSHRINES + TOTALSHRINES)))
		return;
	
	if (Current->nCellNo >= New->nCellNo)
		return;

	Current->fSaved = New->fSaved;
	Current->nCellNo = New->nCellNo;
}

VOID NAKED FASTCALL SocketViewtableCallRoutine(LPUNITANY Unit, DWORD Argument, VOID (STDCALL * Function)(LPUNITANY, DWORD))
{
	__asm
	{
		MOV EAX, 0x6650;
		PUSH EDX;
		PUSH ECX;
		CALL [ESP + 0x0C];
		RET 4;
	}
}

VOID STDCALL SocketViewtable(VOID (STDCALL * Function)(LPUNITANY, DWORD), LPUNITANY Unit, DWORD Argument)
{
	V_ViewSocketableMonsterLifeBarFlag = FALSE;
	SocketViewtableCallRoutine(Unit, Argument, Function);
	
	if (V_ViewSocketableMonsterLifeBarFlag && V_ViewSocketables)
	{
		V_ViewSocketable = TRUE;
		LPUNITANY pUnit = *p_D2CLIENT_InSocketablePatch;
		
		if (pUnit && pUnit->pInventory)
		{
			DWORD N = D2COMMON_GetItemFromInventory(pUnit->pInventory);
			
			while (N)
			{
				*p_D2CLIENT_InSocketablePatch = D2COMMON_GetUnitFromItem(N);
				SocketViewtableCallRoutine(pUnit, 1, Function);
				N = D2COMMON_GetNextItemFromInventory(N);
			}
		}
		
		V_ViewSocketable = FALSE;
		*p_D2CLIENT_InSocketablePatch = pUnit;
	}
}

LPUNITANY ViewInventory()
{
	LPUNITANY Unit = GetViewingUnit();

	if (Unit != Me)
		V_Design->TextHook(565, 300, 0, 0, 1, 1, Unit->pPlayerData->szName);

	return Unit;
}

BOOL ViewInventorySocketProtection()
{
	if (GetViewingUnit())
	{
		SocketProtectionSTUB();
		return TRUE;
	}

	return FALSE;
}