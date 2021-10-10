#include "stdafx.h"
#include "IceMagicianState.h"


CIceMagicianState::CIceMagicianState()
{
}


CIceMagicianState::~CIceMagicianState()
{
}

CIceMagicianIdle * CIceMagicianIdle::Instance()
{
	static CIceMagicianIdle Instance;
	return &Instance;
}

void CIceMagicianIdle::Enter(CIceMagician * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"IceMagician", L"IceMagicianIdle", 0.f, 6.f);
}

void CIceMagicianIdle::Execute(CIceMagician * pOwner)
{
	m_fTimeCount += GET_TIME / CObjectMgr::GetInstance()->CountMonster(MONSTER::RANGED_MONSTER, MELEE_MONSTER::END, RANGED_MONSTER::ICEMAGICIAN);
	if (m_fTimeCount >= 3.f)
		if (pOwner->Attack())
		{
			pOwner->Get_FSM()->ChangeState(CIceMagicianAttack::Instance());
			return;
		}
}

void CIceMagicianIdle::Exit(CIceMagician * pOwner)
{
}

CIceMagicianAttack * CIceMagicianAttack::Instance()
{
	static CIceMagicianAttack Instance;
	return &Instance;
}

void CIceMagicianAttack::Enter(CIceMagician * pOwner)
{
	m_fTimeCount = 0.f;
	m_bShot = false;
	pOwner->Set_Frame(L"IceMagician", L"IceMagicianAttack", 0.f, 12.f);
}

void CIceMagicianAttack::Execute(CIceMagician * pOwner)
{
	m_fTimeCount += GET_TIME / CObjectMgr::GetInstance()->CountMonster(MONSTER::RANGED_MONSTER, MELEE_MONSTER::END, RANGED_MONSTER::ICEMAGICIAN);
	if (m_fTimeCount >= 0.5f && !m_bShot)
	{
		m_bShot = true;
		pOwner->CreateBullet();
	}
	else if (m_fTimeCount >= 1.f)
	{
		pOwner->Get_FSM()->ChangeState(CIceMagicianIdle::Instance());
		return;
	}
}

void CIceMagicianAttack::Exit(CIceMagician * pOwner)
{
}
