#include "stdafx.h"
#include "RapierMan.h"
#include "Weapon.h"

CRapierMan::CRapierMan()
{
}


CRapierMan::~CRapierMan()
{
}

HRESULT CRapierMan::Initialize()
{
	CPlayer::Initialize();
	m_ePlayerType = PLAYER::RAPIERMAN_PLAYER;
	return S_OK;
}

int CRapierMan::ReverseUpdate()
{
	CObj::ReverseUpdate();
	if (CInputMgr::GetInstance()->Get_Actor() != this)
		return NO_EVENT;
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->ReverseUpdate();
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->ReverseLateUpdate();
	//스크롤 보정
	CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
	if (m_lstOldInfo.size() == 1)
	{
		ChangeState(PLAYER_STATE::IDLE);
		Set_Frame(L"RapierMan", L"CharIdle", m_tFrame.fStart, 4.f);
		return REVERSE_END;
	}
	return NO_EVENT;
}

CRapierMan * CRapierMan::Create(VEC3 vPos, CWeapon * pWeapon)
{
	CRapierMan* pInstance = new CRapierMan;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pWeapon = pWeapon;
	if (!CObjectMgr::GetInstance()->GetClear())
		pInstance->m_pInven->PushItem(15, pWeapon);
	return pInstance;
}
