#include "stdafx.h"
#include "ScrollMgr.h"

D3DXVECTOR3 CScrollMgr::m_vScrollMove = {};

CScrollMgr::CScrollMgr()
{
}


CScrollMgr::~CScrollMgr()
{
}

D3DXVECTOR3 CScrollMgr::Get_Scroll()
{
	// TODO: 여기에 반환 구문을 삽입합니다.

	return m_vScrollMove;
}

void CScrollMgr::Set_Scroll(const D3DXVECTOR3& vMove)
{
	m_vScrollMove += vMove;
	//if (m_vScrollMove.x > TILECX * TILE_COUNT_X - WINCX)
	//	m_vScrollMove.x = TILECX * TILE_COUNT_X - WINCX;
	//if (m_vScrollMove.y > TILECY * TILE_COUNT_Y - WINCY)
	//	m_vScrollMove.y = TILECY * TILE_COUNT_Y - WINCY;
	//if (m_vScrollMove.x < 0)
	//	m_vScrollMove.x = 0;
	//if (m_vScrollMove.y < 0)
	//	m_vScrollMove.y = 0;
}

void CScrollMgr::Initialize(const D3DXVECTOR3 & vMove)
{
	if (VEC3LENGTH(&(m_vScrollMove - vMove)) < 2.f)
		return;
	m_vScrollMove = vMove;
	if (m_vScrollMove.x > TILECX * TILE_COUNT_X - WINCX)
		m_vScrollMove.x = TILECX * TILE_COUNT_X - WINCX;
	if (m_vScrollMove.y > TILECY * TILE_COUNT_Y - WINCY)
		m_vScrollMove.y = TILECY * TILE_COUNT_Y - WINCY;
	if (m_vScrollMove.x < 0)
		m_vScrollMove.x = 0;
	if (m_vScrollMove.y < 0)
		m_vScrollMove.y = 0;
}
