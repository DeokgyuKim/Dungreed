#pragma once

class CTerrain;
class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE_INFO*>& GetBestPath();

public:
	void StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos);

private:
	bool PathFinding(int iStartIndex, int iGoalIndex);
	void CreatePath(int iStartIndex, int iGoalIndex);
	bool 오픈리스트에존재합니까(int iIndex);
	bool 클로즈리스트에존재합니까(int iIndex);

private:
	list<int>			m_OpenList;		// 미방문 노드들의 인덱스 보관.
	list<int>			m_CloseList;	// 방문했던 노드들의 인덱스 보관.
	list<TILE_INFO*>	m_BestPath;		// 에이스타를 통해서 만들어낸 경로.

	int					m_iStartIndex;
	CObjectMgr*			m_pObjectMgr;
};

