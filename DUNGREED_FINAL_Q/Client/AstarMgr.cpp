#include "stdafx.h"
#include "AstarMgr.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
	: m_iStartIndex(0)
	, m_pObjectMgr(CObjectMgr::GetInstance())
{
}


CAstarMgr::~CAstarMgr()
{
}

list<TILE_INFO*>& CAstarMgr::GetBestPath()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_BestPath;
}

void CAstarMgr::StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos)
{
#ifdef _DEBUG
	for (auto pTile : m_BestPath)
		pTile->byDrawID = 2;
#endif

	m_OpenList.clear();
	m_CloseList.clear();
	m_BestPath.clear();

	m_iStartIndex = CTileMgr::GetInstance()->Get_TileIndex(vStartPos);
	int iGoalIndex = CTileMgr::GetInstance()->Get_TileIndex(vGoalPos);

	if (m_iStartIndex == iGoalIndex)
		return;

	if (1 == CTileMgr::GetInstance()->Get_Tile()[iGoalIndex]->byOption)
		return;

	if (true == PathFinding(m_iStartIndex, iGoalIndex))
		CreatePath(m_iStartIndex, iGoalIndex);
}

bool CAstarMgr::PathFinding(int iStartIndex, int iGoalIndex)
{
	if(!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIndex);

	// 현재 노드의 주변 인접 타일들을 조사.
	for (auto pTile : CTileMgr::GetInstance()->Get_Graph()[iStartIndex])
	{
		// 인접한 타일이 목적지 타일이라면 검색 종료.
		if (iGoalIndex == pTile->iMyIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		// 목적지 타일이 아니라면 OpenList에 보관한다.
		if (!오픈리스트에존재합니까(pTile->iMyIndex) && !클로즈리스트에존재합니까(pTile->iMyIndex))
		{
			pTile->iParentIndex = iStartIndex;
			m_OpenList.push_back(pTile->iMyIndex);
		}		
	}

	// 더이상 찾을 길이 없음.
	if (m_OpenList.empty())
		return false;

	// 휴리스틱 조사 후 비용이 적은 순으로 OpenList 정렬.
	// 휴리스틱 = 
	// 최초 출발 노드로부터 노드 N까지의 직선 거리 + 노드 N으로부터 목표 노드까지의 직선 거리
	m_OpenList.sort(
		[&](int iPrevIndex, int iNextIndex)
	{
		D3DXVECTOR3 v1 = CTileMgr::GetInstance()->Get_Tile()[m_iStartIndex]->vPos 
			- CTileMgr::GetInstance()->Get_Tile()[iPrevIndex]->vPos;
		D3DXVECTOR3 v2 = CTileMgr::GetInstance()->Get_Tile()[iGoalIndex]->vPos
			- CTileMgr::GetInstance()->Get_Tile()[iPrevIndex]->vPos;
		float fHeuristicA = D3DXVec3Length(&v1) + D3DXVec3Length(&v2);

		v1 = CTileMgr::GetInstance()->Get_Tile()[m_iStartIndex]->vPos
			- CTileMgr::GetInstance()->Get_Tile()[iNextIndex]->vPos;
		v2 = CTileMgr::GetInstance()->Get_Tile()[iGoalIndex]->vPos
			- CTileMgr::GetInstance()->Get_Tile()[iNextIndex]->vPos;
		float fHeuristicB = D3DXVec3Length(&v1) + D3DXVec3Length(&v2);

		return fHeuristicA < fHeuristicB; // 오름차순 정렬.
	});

//#ifdef _DEBUG
//	for (int iIndex : m_OpenList)
//		m_pTerrain->GetVecTile()[iIndex]->byDrawID = 9;
//#endif

	return PathFinding(m_OpenList.front(), iGoalIndex);
}

void CAstarMgr::CreatePath(int iStartIndex, int iGoalIndex)
{
	m_BestPath.push_front(CTileMgr::GetInstance()->Get_Tile()[iGoalIndex]);
	int iParentIndex = CTileMgr::GetInstance()->Get_Tile()[iGoalIndex]->iParentIndex;

	while (true)
	{
		if (iParentIndex == iStartIndex)
			break;

		m_BestPath.push_front(CTileMgr::GetInstance()->Get_Tile()[iParentIndex]);
		iParentIndex = CTileMgr::GetInstance()->Get_Tile()[iParentIndex]->iParentIndex;
	}

#ifdef _DEBUG
	for (auto pTile : m_BestPath)
		pTile->byDrawID = 36;
#endif
}

bool CAstarMgr::오픈리스트에존재합니까(int iIndex)
{
	auto iter_find = find(m_OpenList.begin(), m_OpenList.end(), iIndex);

	if(m_OpenList.end() == iter_find)
		return false;

	return true;
}

bool CAstarMgr::클로즈리스트에존재합니까(int iIndex)
{
	auto iter_find = find(m_CloseList.begin(), m_CloseList.end(), iIndex);

	if (m_CloseList.end() == iter_find)
		return false;

	return true;
}
