#pragma once
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
	void	Release();
public:
	HRESULT LoadData(const TCHAR* pFilePath);
public:
	bool								IsCollision(VEC3 vPos, RECT& rc);
	int									Get_TileIndex(VEC3 vPos);
	inline vector<TILE_INFO*>&			Get_Tile() { return m_vecTile; }
	inline vector<list<TILE_INFO*>>&	Get_Graph() { return m_vecGraph; }
private:
	vector<TILE_INFO*>	m_vecTile;
	vector<list<TILE_INFO*>>	m_vecGraph;
};

