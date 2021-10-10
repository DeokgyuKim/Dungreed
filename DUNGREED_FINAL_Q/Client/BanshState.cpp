#include "stdafx.h"
#include "BanshState.h"


CBanshState::CBanshState()
{
}


CBanshState::~CBanshState()
{
}

CBanshIdle * CBanshIdle::Instance()
{
	static CBanshIdle Instance;
	return &Instance;
}

void CBanshIdle::Enter(CBansh * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"Bansh", L"BanshIdle", 0.f, 6.f);
}

void CBanshIdle::Execute(CBansh * pOwner)
{
	m_fTimeCount += GET_TIME / CObjectMgr::GetInstance()->CountMonster(MONSTER::RANGED_MONSTER, MELEE_MONSTER::END, RANGED_MONSTER::BANSH);
	if (m_fTimeCount >= 2.f)
		if (pOwner->Searching())
		{
			pOwner->Get_FSM()->ChangeState(CBanshFollow::Instance());
			return;
		}
}

void CBanshIdle::Exit(CBansh * pOwner)
{
}

CBanshFollow * CBanshFollow::Instance()
{
	static CBanshFollow Instance;
	return &Instance;
}

void CBanshFollow::Enter(CBansh * pOwner)
{
	m_fTimeCount = 0.f;
}

void CBanshFollow::Execute(CBansh * pOwner)
{
	if (!pOwner->Searching())
	{
		pOwner->Get_FSM()->ChangeState(CBanshIdle::Instance());
		return;
	}
	if (pOwner->Attack())
	{
		pOwner->Get_FSM()->ChangeState(CBanshAttack::Instance());
		return;
	}

	pOwner->Following();
}

void CBanshFollow::Exit(CBansh * pOwner)
{
}

CBanshAttack * CBanshAttack::Instance()
{
	static CBanshAttack Instance;
	return &Instance;
}

void CBanshAttack::Enter(CBansh * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"Bansh", L"BanshAttack", 0.f, 6.f);
	pOwner->CreateBullet();
}

void CBanshAttack::Execute(CBansh * pOwner)
{
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount >= 1.f)
	{
		pOwner->Get_FSM()->ChangeState(CBanshIdle::Instance());
		return;
	}
}

void CBanshAttack::Exit(CBansh * pOwner)
{
}
