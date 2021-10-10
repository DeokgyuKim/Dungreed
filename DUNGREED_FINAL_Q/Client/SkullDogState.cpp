#include "stdafx.h"
#include "SkullDogState.h"


CSkullDogState::CSkullDogState()
{
}


CSkullDogState::~CSkullDogState()
{
}

CSkullDogIdle * CSkullDogIdle::Instance()
{
	static CSkullDogIdle Instance;
	return &Instance;
}

void CSkullDogIdle::Enter(CSkullDog * pOwner)
{
	m_fTimeCount = 0.f;
	pOwner->Set_Frame(L"SkullDog", L"SkullDogIdle", 0.f, 5.f);
}

void CSkullDogIdle::Execute(CSkullDog * pOwner)
{
	m_fTimeCount += GET_TIME / CObjectMgr::GetInstance()->CountMonster(MONSTER::MELEE_MONSTER, MELEE_MONSTER::SKULLDOG);
	if (m_fTimeCount >= 1.f)
	{
		if (pOwner->Searching())
		{
			pOwner->Get_FSM()->ChangeState(CSkullDogFollow::Instance());
			return;
		}
	}
}

void CSkullDogIdle::Exit(CSkullDog * pOwner)
{
}

CSkullDogFollow * CSkullDogFollow::Instance()
{
	static CSkullDogFollow Instance;
	return &Instance;
}

void CSkullDogFollow::Enter(CSkullDog * pOwner)
{
	pOwner->Set_Frame(L"SkullDog", L"SkullDogRun", 0.f, 7.f);
}

void CSkullDogFollow::Execute(CSkullDog * pOwner)
{

	if (!pOwner->Searching())
	{
		pOwner->Get_FSM()->ChangeState(CSkullDogIdle::Instance());
		return;
	}
	if (pOwner->Attack())
	{
		pOwner->Get_FSM()->ChangeState(CSkullDogAttack::Instance());
		return;
	}

	pOwner->Following();
}

void CSkullDogFollow::Exit(CSkullDog * pOwner)
{
	pOwner->Set_Dir(pOwner->FindDir());
}

CSkullDogAttack * CSkullDogAttack::Instance()
{
	static CSkullDogAttack Instance;
	return &Instance;
}

void CSkullDogAttack::Enter(CSkullDog * pOwner)
{
	m_fTimeCount = 0.f;
	m_vPos = pOwner->Get_Info()->vPos;
	pOwner->Set_Frame(L"SkullDog", L"SkullDogJump", 0.f, 1.f);
	pOwner->Set_Att(10);
}

void CSkullDogAttack::Execute(CSkullDog * pOwner)
{
	m_fTimeCount += PI * GET_TIME * 5.f;

	if (m_fTimeCount >= PI)
	{
		pOwner->Get_FSM()->ChangeState(CSkullDogIdle::Instance());
		return;
	}

	VEC3 vMove = pOwner->Get_Info()->vDir * pOwner->Get_Info()->fSpeed * 2.3f * GET_TIME;
	vMove.y -= 3.f * cosf(m_fTimeCount);
	pOwner->Move_Pos(vMove);
}

void CSkullDogAttack::Exit(CSkullDog * pOwner)
{
	m_vPos = { pOwner->Get_Info()->vPos.x, m_vPos.y, pOwner->Get_Info()->vPos.z };
	pOwner->Set_Pos(m_vPos);
}
