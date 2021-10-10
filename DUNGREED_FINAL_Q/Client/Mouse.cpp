#include "stdafx.h"
#include "Mouse.h"

IMPLEMENT_SINGLETON(CMouse)
CMouse::CMouse()
	: m_vSize({21.f, 21.f, 0.f})
{
	ShowCursor(FALSE);
}


CMouse::~CMouse()
{
}

void CMouse::Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_vPos = { (FLOAT)pt.x, (FLOAT)pt.y , 0.f };
}

void CMouse::Render()
{
	MATRIX	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 4.f, 4.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y + m_vSize.y * 0.5f, 0.f);
	matWorld = matScale * matTrans;
	D3DXMatrixScaling(&matScale, 0.25f, 0.25f, 0.25f);
	matWorld = matWorld * matScale;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"MouseCursor", &matWorld, L"Normal", 0);
}

D3DXVECTOR3 CMouse::GetMousePos()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x * REVERSE_RATIO, (float)pt.y * REVERSE_RATIO, 0.f);
}
