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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
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

	// ���� ����� �ֺ� ���� Ÿ�ϵ��� ����.
	for (auto pTile : CTileMgr::GetInstance()->Get_Graph()[iStartIndex])
	{
		// ������ Ÿ���� ������ Ÿ���̶�� �˻� ����.
		if (iGoalIndex == pTile->iMyIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true;
		}

		// ������ Ÿ���� �ƴ϶�� OpenList�� �����Ѵ�.
		if (!���¸���Ʈ�������մϱ�(pTile->iMyIndex) && !Ŭ�����Ʈ�������մϱ�(pTile->iMyIndex))
		{
			pTile->iParentIndex = iStartIndex;
			m_OpenList.push_back(pTile->iMyIndex);
		}		
	}

	// ���̻� ã�� ���� ����.
	if (m_OpenList.empty())
		return false;

	// �޸���ƽ ���� �� ����� ���� ������ OpenList ����.
	// �޸���ƽ = 
	// ���� ��� ���κ��� ��� N������ ���� �Ÿ� + ��� N���κ��� ��ǥ �������� ���� �Ÿ�
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

		return fHeuristicA < fHeuristicB; // �������� ����.
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

bool CAstarMgr::���¸���Ʈ�������մϱ�(int iIndex)
{
	auto iter_find = find(m_OpenList.begin(), m_OpenList.end(), iIndex);

	if(m_OpenList.end() == iter_find)
		return false;

	return true;
}

bool CAstarMgr::Ŭ�����Ʈ�������մϱ�(int iIndex)
{
	auto iter_find = find(m_CloseList.begin(), m_CloseList.end(), iIndex);

	if (m_CloseList.end() == iter_find)
		return false;

	return true;
}
