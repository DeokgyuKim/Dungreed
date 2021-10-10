#include "stdafx.h"
#include "IceBullet.h"
#include "SoundMgr.h"

CIceBullet::CIceBullet()
{
}


CIceBullet::~CIceBullet()
{
	Release();
}

int CIceBullet::Update()
{
	m_fTimeCount += GET_TIME;
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	if (m_fTimeCount >= 0.2f && !m_bCreate)
	{
		m_bCreate = true;
		Set_Frame(L"IceBullet", L"IceBulletIdle", 0.f, 1.f);
	}
	else if (m_fTimeCount < 5.f && m_fTimeCount > 0.2f)
	{
		CBullet::Update();
	}
	else if (!m_tInfo.bDead && m_fTimeCount >= 5.f)
	{
		m_tInfo.bDead = true;
		Set_Frame(L"IceBullet", L"DestoryIceBullet", 0.f, 3.f);
	}
	CObj::UpdateRect();
	return NO_EVENT;
}

void CIceBullet::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CIceBullet::Render()
{
	if (m_tInfo.bDead && !CObjectMgr::GetInstance()->GetReverse())
		return;
	MATRIX	matRotate, matTrans, matWorld;

	float fCos = VEC3DOT(&m_tInfo.vDir, &VEC3(1.f, 0.f, 0.f));
	float fAcos = acosf(fCos);
	fAcos = TODEGREE(fAcos);
	if (m_tInfo.vDir.y < 0.f)
		fAcos = 360.f - fAcos;

	MATROTATIONZ(&matRotate, TORADIAN(fAcos));
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);

	matWorld = matRotate * matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::MID);
}

HRESULT CIceBullet::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 200;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 14.f, 14.f, 0.f };

	m_tInfo.iAtt = 1;
	m_tInfo.iHp = 0;

	Set_Frame(L"IceBullet", L"CreateIceBullet", 0.f, 3.f);

	m_tInfo.bDead = false;
	m_bCreate = false;

	m_fTimeCount = 0.f;
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyDungeonOut", CSoundMgr::CHANNELID::MONSTER);
	return S_OK;
}

void CIceBullet::Release()
{
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT,
		m_tInfo.vPos + VEC3(0.f, m_tInfo.vSize.y * 0.5f, 0.f), 0.4f, L"IceBullet", L"DestoryIceBullet", 3.f);

}

void CIceBullet::FrameUpdate()
{
	if (m_tFrame.bReverse)
		m_tFrame.fStart -= m_tFrame.fCount * 5.f * CTimeMgr::GetInstance()->GetDeltaTime();
	else
		m_tFrame.fStart += m_tFrame.fCount * 5.f * CTimeMgr::GetInstance()->GetDeltaTime();

	if (m_tFrame.fStart > m_tFrame.fCount)
		m_tFrame.fStart = 0.f;
	if (m_tFrame.fStart < 0)
		m_tFrame.fStart = m_tFrame.fCount - 0.00001f;
}

CIceBullet * CIceBullet::Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CIceBullet* pInstance = new CIceBullet;
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

CIceBullet * CIceBullet::CreateImmediately(VEC3 vPos, float fSpeed, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CIceBullet* pInstance = new CIceBullet;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vDir = vDir;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_tInfo.fSpeed = fSpeed;
	pInstance->m_eHitBoxType = eType;
	pInstance->m_fTimeCount = 0.2f;

	return pInstance;
}
