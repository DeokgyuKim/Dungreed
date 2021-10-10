#include "stdafx.h"
#include "LineMgr.h"

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
}

void CLineMgr::DrawBox(RECT& rc)
{
#ifdef _DEBUG
	CDeviceMgr::GetInstance()->GetLine()->SetWidth(2.f);
	CDeviceMgr::GetInstance()->GetSprite()->End();

	D3DXVECTOR2 vBox[5] = {};
	vBox[0] = { (float)rc.left - CScrollMgr::Get_Scroll().x, (float)rc.top - CScrollMgr::Get_Scroll().y };
	vBox[1] = { (float)rc.left - CScrollMgr::Get_Scroll().x, (float)rc.bottom - CScrollMgr::Get_Scroll().y };
	vBox[2] = { (float)rc.right - CScrollMgr::Get_Scroll().x, (float)rc.bottom - CScrollMgr::Get_Scroll().y };
	vBox[3] = { (float)rc.right - CScrollMgr::Get_Scroll().x, (float)rc.top - CScrollMgr::Get_Scroll().y };
	vBox[4] = { (float)rc.left - CScrollMgr::Get_Scroll().x, (float)rc.top - CScrollMgr::Get_Scroll().y };

	for (int i = 0; i < 5; ++i)
		vBox[i] *= 4.f;
	CDeviceMgr::GetInstance()->GetLine()->Draw(vBox, 5, D3DCOLOR_XRGB(255, 0, 0));

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
#endif
}

void CLineMgr::DrawBoxNoScroll(RECT & rc)
{
#ifdef _DEBUG
	CDeviceMgr::GetInstance()->GetLine()->SetWidth(2.f);
	CDeviceMgr::GetInstance()->GetSprite()->End();

	D3DXVECTOR2 vBox[5] = {};
	vBox[0] = { (float)rc.left, (float)rc.top };
	vBox[1] = { (float)rc.left, (float)rc.bottom };
	vBox[2] = { (float)rc.right, (float)rc.bottom };
	vBox[3] = { (float)rc.right, (float)rc.top };
	vBox[4] = { (float)rc.left, (float)rc.top };

	for (int i = 0; i < 5; ++i)
		vBox[i] *= 4.f;
	CDeviceMgr::GetInstance()->GetLine()->Draw(vBox, 5, D3DCOLOR_XRGB(255, 0, 0));

	CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
#endif
}
