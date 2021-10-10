#include "stdafx.h"
#include "StarBullet.h"


CStarBullet::CStarBullet()
{
}


CStarBullet::~CStarBullet()
{
	Release();
}

int CStarBullet::Update()
{
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount < 3.f)
	{
		m_pTarget = CObjectMgr::GetInstance()->GetTarget(m_tInfo.vPos);
		if (m_pTarget == nullptr)
		{
			m_tInfo.bDead = true;
			return DEAD_OBJ;
		}

		VEC3 vToTarget = m_pTarget->Get_Info()->vPos - m_tInfo.vPos;
		VEC3 vCross;
		VEC3CROSS(&vCross, &vToTarget, &m_tInfo.vDir);

		if (vCross.z > 0)
		{
			MATRIX matRotate;
			MATROTATIONZ(&matRotate, TORADIAN(-2.f));
			VEC3_T_NORMAL(&m_tInfo.vDir, &m_tInfo.vDir, &matRotate);
		}
		else
		{
			MATRIX matRotate;
			MATROTATIONZ(&matRotate, TORADIAN(2.f));
			VEC3_T_NORMAL(&m_tInfo.vDir, &m_tInfo.vDir, &matRotate);
		}

		m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
	}
	else
		m_tInfo.bDead = true;

	if(++m_iAfterImage % 5 == 0)
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, m_tInfo.vSize.y * 0.5f, 0.f), 1.0f, m_tFrame.pObjectKey, m_tFrame.pStateKey, m_tFrame.fStart);


	CObj::UpdateRect();
	return NO_EVENT;
}

void CStarBullet::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CStarBullet::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 100;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 14.f, 14.f, 0.f };

	m_tInfo.iAtt = 10;
	m_tInfo.iHp = 0;

	m_iAfterImage = 0;

	Set_Frame(L"Stamp", L"StarBullet", 0.f, 4.f);

	m_tInfo.bDead = false;

	m_fTimeCount = 0.f;
	return S_OK;
}

void CStarBullet::Release()
{
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, 
		m_tInfo.vPos + VEC3(0.f, m_tInfo.vSize.y * 0.5f, 0.f), 0.4f, m_tFrame.pObjectKey, L"StarBulletFX", 7.f);
}

CStarBullet * CStarBullet::Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CStarBullet* pInstance = new CStarBullet;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_eHitBoxType = eType;
	pInstance->m_tInfo.vDir = vDir;

	return pInstance;
}

void CStarBullet::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
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
