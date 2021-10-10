#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	static D3DXVECTOR3 Get_Scroll();

public:
	static void Set_Scroll(const D3DXVECTOR3& vMove);
	static void Initialize(const D3DXVECTOR3& vMove);

private:
	static D3DXVECTOR3 m_vScrollMove;
};

