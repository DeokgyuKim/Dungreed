#include "stdafx.h"
#include "GuardianState.h"
#include "HitBox.h"


CGuardianState::CGuardianState()
{
}


CGuardianState::~CGuardianState()
{
}

CGuardianIdle * CGuardianIdle::Instance()
{
	static CGuardianIdle Instance;
	return &Instance;
}

void CGuardianIdle::Enter(CGuardian * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"Guardian", L"GuardianIdle", 0.f, 4.f);
}

void CGuardianIdle::Execute(CGuardian * pOwner)
{
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount >= 1.f)
	{
		if (pOwner->Attack())
		{
			pOwner->Get_FSM()->ChangeState(CGuardianAttack::Instance());
			return;
		}
		if (pOwner->Searching())
		{
			pOwner->Get_FSM()->ChangeState(CGuardianFollow::Instance());
			return;
		}
	}
}

void CGuardianIdle::Exit(CGuardian * pOwner)
{
}

CGuardianFollow * CGuardianFollow::Instance()
{
	static CGuardianFollow Instance;
	return &Instance;
}

void CGuardianFollow::Enter(CGuardian * pOwner)
{
	m_bCrush = true;
	m_bFollow = false;
	m_bShow = false;
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"Guardian", L"GuardianCrush", 0.f, 7.f);
}

void CGuardianFollow::Execute(CGuardian * pOwner)
{
	m_fTimeCount += GET_TIME / CObjectMgr::GetInstance()->CountMonster(MONSTER::MELEE_MONSTER, MELEE_MONSTER::GUARDIAN);
	if (m_bCrush && m_fTimeCount >= 1.f)
	{
		pOwner->Set_Frame(L"Guardian", L"GuardianEffect", 0.f, 7.f);
		pOwner->Set_Pos(pOwner->Get_Info()->vPos - VEC3(30.f, -30.f, 0.f));
		m_bCrush = false;
		m_bFollow = true;
	}
	if (m_bFollow && m_fTimeCount >= 2.f)
	{
		pOwner->Set_Pos(pOwner->Get_PlayerPos());
		pOwner->Set_Frame(L"Guardian", L"GuardianEffect", 6.99f, 7.f);
		pOwner->Set_Frame_Reverse(true);
		m_bFollow = false;
		m_bShow = true;
	}
	if (m_bShow && m_fTimeCount >= 3.f)
	{
		//pOwner->Move_Pos(VEC3(10.f, -30.f, 0.f));
		if (pOwner->Attack())
		{
			pOwner->Get_FSM()->ChangeState(CGuardianAttack::Instance());
			return;
		}
		else
		{
			pOwner->Get_FSM()->ChangeState(CGuardianIdle::Instance());
			return;
		}
	}
}

void CGuardianFollow::Exit(CGuardian * pOwner)
{
}

CGuardianAttack * CGuardianAttack::Instance()
{
	static CGuardianAttack Instance;
	return &Instance;
}

void CGuardianAttack::Enter(CGuardian * pOwner)
{
	m_fTimeCount = 0.f;
	m_bAtt = false;
	pOwner->Set_Frame(L"Guardian", L"GuardianAttack", 0.f, 8.f);
}

void CGuardianAttack::Execute(CGuardian * pOwner)
{
	m_fTimeCount += GET_TIME;
	if (!m_bAtt && m_fTimeCount >= 0.5f)
	{
		CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CHitBox::Create(pOwner->Get_Info()->vPos, VEC3(48.f, 32.f, 0.f), VEC3(0.f, 0.f, 0.f),
			pOwner->Get_Info()->iAtt, HITBOX::MONSTER));
		m_bAtt = true;
	}
	if (m_fTimeCount >= 1.f)
	{
		pOwner->Get_FSM()->ChangeState(CGuardianIdle::Instance());
		return;
	}
}

void CGuardianAttack::Exit(CGuardian * pOwner)
{
}
