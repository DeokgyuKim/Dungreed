#include "stdafx.h"
#include "IceMagicianBullet.h"


CIceMagicianBullet::CIceMagicianBullet()
{
}


CIceMagicianBullet::~CIceMagicianBullet()
{
}

int CIceMagicianBullet::Update()
{
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount >= 1.f && !m_bShot)
	{
		CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CHitBox::Create(m_tInfo.vPos, m_tInfo.vSize, m_tInfo.vDir, m_tInfo.iAtt, HITBOX::MONSTER));
		m_bShot = true;
	}
	
	return NO_EVENT;
}

void CIceMagicianBullet::LateUpdate()
{
	this->FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CIceMagicianBullet::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 50;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 30.f, 30.f, 0.f };

	m_tInfo.iAtt = 5;
	m_tInfo.iHp = 0;

	Set_Frame(L"IceCrystal", L"Create", 0.f, 21.f);

	m_tInfo.bDead = false;
	m_bShot = false;

	m_fTimeCount = 0.f;
	return S_OK;
}

void CIceMagicianBullet::Release()
{
}

void CIceMagicianBullet::FrameUpdate()
{
	if (m_tFrame.bReverse)
		m_tFrame.fStart -= 15.f * CTimeMgr::GetInstance()->GetDeltaTime();
	else
		m_tFrame.fStart += 15.f * CTimeMgr::GetInstance()->GetDeltaTime();

	if (m_tFrame.fStart > m_tFrame.fCount)
	{
		m_tFrame.fStart = 0.f;
		m_tInfo.bDead = true;
	}
	if (m_tFrame.fStart < 0)
	{
		m_tFrame.fStart = m_tFrame.fCount - 0.00001f;
		m_tInfo.bDead = true;
	}
}

CIceMagicianBullet * CIceMagicianBullet::Create(VEC3 vPos, int iAtt, HITBOX::TYPE eType)
{
	CIceMagicianBullet* pInstance = new CIceMagicianBullet;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_eHitBoxType = eType;
	return pInstance;
}
