#include "stdafx.h"
#include "DartBullet.h"
#include "PlayerObserver.h"

CDartBullet::CDartBullet()
{
	m_pObserver = CPlayerObserver::Create();
	CSubject::GetInstance()->Subscribe(m_pObserver);
}


CDartBullet::~CDartBullet()
{
	CSubject::GetInstance()->UnSubscribe(m_pObserver);
}

int CDartBullet::Update()
{
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount < 3.f)
	{
		m_tInfo.fSpeed -= 750.f * GET_TIME;
		m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
		if (!m_bReturn && m_tInfo.fSpeed <= 0.f)
		{
			VEC3 vDir = GET_PLAYER_OB->GetPlayerDir();

			vDir *= 0.333f;
			//m_tInfo.vDir += vDir;
			VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
			m_tInfo.vDir = -(vPos - m_tInfo.vPos);
			VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);
			m_tInfo.vDir += vDir;
			VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);
			m_bReturn = true;
		}
	}
	else
		m_tInfo.bDead = true;
	CObj::UpdateRect();
	return NO_EVENT;
}

void CDartBullet::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::FrameUpdate();
	CObj::FrameUpdate();
	CObj::FrameUpdate();
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CDartBullet::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 500;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 37.f, 37.f, 0.f };

	m_tInfo.iAtt = 3;
	m_tInfo.iHp = 0;

	Set_Frame(L"Bomerang", L"BomerangAttack", 0.f, 2.f);

	m_tInfo.bDead = false;
	m_bReturn = false;

	m_fTimeCount = 0.f;
	return S_OK;
}

void CDartBullet::Release()
{
}

CDartBullet * CDartBullet::Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CDartBullet* pInstance = new CDartBullet;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vDir = vDir;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_eHitBoxType = HITBOX::DART;

	return pInstance;
}

void CDartBullet::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	switch (eType)
	{
	case OBJ::PLAYER:
		break;
	case OBJ::MONSTER:
		break;
	case OBJ::ICE_PILLAR:
		break;
	case OBJ::BOSS:
		break;
	default:
		break;
	}
}
