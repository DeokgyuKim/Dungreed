#pragma once
class CMouse
{
	DECLARE_SINGLETON(CMouse)
private:
	CMouse();
	~CMouse();
public:
	void	Update();
	void	Render();
public:
	static D3DXVECTOR3 GetMousePos();
private:
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vSize;
};

