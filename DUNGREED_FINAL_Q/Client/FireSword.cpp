#include "stdafx.h"
#include "FireSword.h"
#include "HitBox.h"
#include "PlayerObserver.h"

CFireSword::CFireSword()
{
}


CFireSword::~CFireSword()
{
}

void CFireSword::Attack()
{
	if (!m_bAtt)
	{
		m_bAtt = true;
		m_fAttTime = 0.f;
		m_fShakeValue = 0.f;
		VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
		VEC3NORMAL(&vDir, &vDir);
		CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CHitBox::Create(m_tInfo.vPos + VEC3(0.f, 16.f, 0.f) + vDir * 20, VEC3(40.f, 40.f, 0.f), vDir, m_tInfo.iAtt, HITBOX::PLAYER));
		
		float fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
		float fAcos = acosf(fCos);
		fAcos = TODEGREE(fAcos);

		if (vDir.y < 0)
			fAcos = 360.f - fAcos;
		
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, GET_PLAYER_OB->GetPlayerPos() - VEC3(0.f, 8.f, 0.f) + vDir * 20, 0.4f, L"FireSword", L"Swing",
			6, fAcos, RENDER::MID);
	}
}

HRESULT CFireSword::Initialize()
{
	m_eType = OBJ::FIRE_SWORD;
	m_tInfo.iAtt = 30;
	m_tInfo.vSize = { 13.f, 32.f, 0.f };
	m_fAttSpeed = 4.f;
	m_fAttTime = 0.f;
	m_fShakeValue = 0.f;
	m_bAtt = false;
	m_bUpper = false;
	m_tAngle.fSwingAngle = 0.f;
	m_tAngle.fLookAngle = 0.f;
	m_tInfo.m_iGold = 2000;
	Set_Frame(L"FireSword", L"FireDragonKillerIdle", 0.f, 1.f);
	return S_OK;
}

CFireSword * CFireSword::Create()
{
	CFireSword* pInstance = new CFireSword;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
