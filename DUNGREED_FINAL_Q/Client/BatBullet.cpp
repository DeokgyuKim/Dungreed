#include "stdafx.h"
#include "BatBullet.h"


CBatBullet::CBatBullet()
{
}


CBatBullet::~CBatBullet()
{
}

int CBatBullet::Update()
{
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount < 0.5f)
	{

	}
	else if (m_fTimeCount < 3.f)
	{
		Set_Frame(L"RangeBallBullet", L"Idle", 0.f, 1.f);
		CBullet::Update();
	}
	else
		m_tInfo.bDead = true;
	CObj::UpdateRect();
	return NO_EVENT;
}

void CBatBullet::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CBatBullet::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 300;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 14.f, 14.f, 0.f };

	m_tInfo.iAtt = 5;
	m_tInfo.iHp = 0;

	Set_Frame(L"RangeBallBullet", L"Create", 0.f, 5.f);

	m_tInfo.bDead = false;

	m_fTimeCount = 0.f;
	return S_OK;
}

void CBatBullet::Release()
{
}

CBatBullet * CBatBullet::Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CBatBullet* pInstance = new CBatBullet;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;// + vDir * 10.f;
	pInstance->m_tInfo.vDir = vDir;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_eHitBoxType = eType;

	return pInstance;
}
