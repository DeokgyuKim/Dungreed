#pragma once
class CToolView;
class CTileMgr
{
	DECLARE_SINGLETON(CTileMgr)
private:
	CTileMgr();
	~CTileMgr();
public:
	HRESULT Initialize(TCHAR* pFilePath = nullptr);
	HRESULT CreateGraph();
	void	Render();
	void	MiniRender();
	void	Release();
public:
	inline void	Set_View(CToolView* pView) { m_pView = pView; }
	inline void Set_Point(POINT vPt) { m_vPt = vPt; }
public:
	HRESULT SaveData(const TCHAR* pFilePath);
	HRESULT LoadData(const TCHAR* pFilePath);
public:
	void	TileChange(VEC3 vPos, BYTE byDrawID, BYTE byOption);
public:
	int									Get_TileIndex(VEC3 vPos);
	inline vector<TILE_INFO*>&			Get_Tile() { return m_vecTile; }
	inline vector<list<TILE_INFO*>>&	Get_Graph() { return m_vecGraph; }
private:
	vector<TILE_INFO*>	m_vecTile;
	vector<list<TILE_INFO*>>	m_vecGraph;
	CToolView*	m_pView;
	POINT		m_vPt;
};

