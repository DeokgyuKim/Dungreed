#include "stdafx.h"
#include "BatState.h"


CBatState::CBatState()
{
}


CBatState::~CBatState()
{
}

CBatIdle * CBatIdle::Instance()
{
	static CBatIdle Instance;
	return &Instance;
}

void CBatIdle::Enter(CBat * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"Bat", L"BatIdle", 0.f, 7.f);
}

void CBatIdle::Execute(CBat * pOwner)
{
	m_fTimeCount += GET_TIME / CObjectMgr::GetInstance()->CountMonster(MONSTER::RANGED_MONSTER, MELEE_MONSTER::END, RANGED_MONSTER::BAT);
	if (m_fTimeCount >= 1.f)
		if (pOwner->Searching())
		{
			pOwner->Get_FSM()->ChangeState(CBatFollow::Instance());
			return;
		}
}

void CBatIdle::Exit(CBat * pOwner)
{
}

CBatFollow * CBatFollow::Instance()
{
	static CBatFollow Instance;
	return &Instance;
}

void CBatFollow::Enter(CBat * pOwner)
{
	m_fTimeCount = 0.f;
}

void CBatFollow::Execute(CBat * pOwner)
{
	if (!pOwner->Searching())
	{
		pOwner->Get_FSM()->ChangeState(CBatIdle::Instance());
		return;
	}
	if (pOwner->Attack())
	{
		pOwner->Get_FSM()->ChangeState(CBatAttack::Instance());
		return;
	}

	pOwner->Following();
}

void CBatFollow::Exit(CBat * pOwner)
{
}

CBatAttack * CBatAttack::Instance()
{
	static CBatAttack Instance;
	return &Instance;
}

void CBatAttack::Enter(CBat * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"Bat", L"BatAttack", 0.f, 10.f);
	pOwner->CreateBullet();
}

void CBatAttack::Execute(CBat * pOwner)
{
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount >= 1.f)
	{
		pOwner->Get_FSM()->ChangeState(CBatIdle::Instance());
		return;
	}
}

void CBatAttack::Exit(CBat * pOwner)
{
}
