#include "stdafx.h"
#include "Bullet.h"


CBullet::CBullet()
{
}


CBullet::~CBullet()
{
}

int CBullet::Update()
{
	m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
	return NO_EVENT;
}

void CBullet::Render()
{
	MATRIX	matWorld;
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matWorld, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::MID);
}

void CBullet::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	switch (eType)
	{
	case OBJ::PLAYER:
		m_tInfo.bDead = true;
		break;
	case OBJ::MONSTER:
		m_tInfo.bDead = true;
		break;
	case OBJ::ICE_PILLAR:
		m_tInfo.bDead = true;
		break;
	case OBJ::BOSS:
		m_tInfo.bDead = true;
		break;
	default:
		break;
	}
}
