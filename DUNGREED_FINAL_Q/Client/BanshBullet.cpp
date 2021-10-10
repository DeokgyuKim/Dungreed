#include "stdafx.h"
#include "BanshBullet.h"


CBanshBullet::CBanshBullet()
{
}


CBanshBullet::~CBanshBullet()
{
}

int CBanshBullet::Update()
{
	if (m_tInfo.bDead)
		if(m_fTimeCount > 3.f)
			return DEAD_OBJ;
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount < 2.f)
	{
		CBullet::Update();
	}
	else if(!m_tInfo.bDead)
	{
		m_tInfo.bDead = true;
		Set_Frame(L"BanshBullet", L"Dead", 0.f, 6.f);
	}
	CObj::UpdateRect();
	return NO_EVENT;
}

void CBanshBullet::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CBanshBullet::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 50;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 14.f, 14.f, 0.f };

	m_tInfo.iAtt = 5;
	m_tInfo.iHp = 0;

	Set_Frame(L"BanshBullet", L"Idle", 0.f, 4.f);

	m_tInfo.bDead = false;

	m_fTimeCount = 0.f;
	return S_OK;
}

void CBanshBullet::Release()
{
}

CBanshBullet * CBanshBullet::Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CBanshBullet* pInstance = new CBanshBullet;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vDir = vDir;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_eHitBoxType = eType;

	return pInstance;
}
