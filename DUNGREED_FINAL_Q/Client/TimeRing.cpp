#include "stdafx.h"
#include "TimeRing.h"
#include "PlayerObserver.h"

CTimeRing::CTimeRing()
{
}


CTimeRing::~CTimeRing()
{
}

int CTimeRing::Update()
{
	return NO_EVENT;
}

void CTimeRing::LateUpdate()
{
}

void CTimeRing::Render()
{
}

HRESULT CTimeRing::Initialize()
{
	m_eType = OBJ::RING;
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	m_tInfo.vSize = { 9.f, 9.f, 0.f };
	m_eRingType = RING::TIME;
	m_tInfo.m_iGold = 3000;

	Set_Frame(L"Ring", L"TimeRing", 0.f, 1.f);
	return S_OK;
}

void CTimeRing::Release()
{
}

CTimeRing * CTimeRing::Create()
{
	CTimeRing* pInstance = new CTimeRing;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CTimeRing::EffectRing()
{
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	vPos += VEC3(0.f, -8.f, 0.f);

	CObjectMgr::GetInstance()->DeleteLst(OBJ_EFFECT);

	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, vPos, 0.4f, L"Ring", L"TimeFx",
		12, 0.f, RENDER::MID);

	CObjectMgr::GetInstance()->TimeRingEffect();
}

