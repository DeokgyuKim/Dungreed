#include "stdafx.h"
#include "SpeedRing.h"
#include "PlayerObserver.h"

CSpeedRing::CSpeedRing()
{
}


CSpeedRing::~CSpeedRing()
{
}

int CSpeedRing::Update()
{
	return NO_EVENT;
}

void CSpeedRing::LateUpdate()
{
}

void CSpeedRing::Render()
{
}

HRESULT CSpeedRing::Initialize()
{
	m_eType = OBJ::RING;
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	m_tInfo.vSize = { 9.f, 9.f, 0.f };
	m_eRingType = RING::SPEED;
	m_tInfo.m_iGold = 2000;

	Set_Frame(L"Ring", L"SpeedRing", 0.f, 1.f);
	return S_OK;
}

void CSpeedRing::Release()
{
}

CSpeedRing * CSpeedRing::Create()
{
	CSpeedRing* pInstance = new CSpeedRing;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CSpeedRing::EffectRing()
{
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	vPos += VEC3(0.f, -8.f, 0.f);

	list<CObj*>* pLst = CObjectMgr::GetInstance()->GetLst(OBJ_MONSTER);
	for (auto& pMon : *pLst)
		pMon->Set_Speed();

	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, vPos, 0.4f, L"Ring", L"SpeedFx",
		12, 0.f, RENDER::MID);
}

