#include "stdafx.h"
#include "NiflheimState.h"


CNiflheimState::CNiflheimState()
{
}


CNiflheimState::~CNiflheimState()
{
}

int CNiflheim_IDLE::m_iPattCount = 0;

CNiflheim_IDLE * CNiflheim_IDLE::Instance()
{
	static CNiflheim_IDLE Instance;
	return &Instance;
}

void CNiflheim_IDLE::Enter(CNiflheim *pOwner)
{
	m_fTimeCount = 0.f;
	if (++m_iPattCount >= 6)
		m_iPattCount = 0;
	pOwner->Set_Frame(L"Boss", L"BossIdle", 0.f, 6.f);
	if (pOwner->IsAttackPossible())
		pOwner->MakeIcePillar();
}

void CNiflheim_IDLE::Execute(CNiflheim *pOwner)
{
	m_fTimeCount += GET_TIME;
	pOwner->GongIce(90.f * GET_TIME);
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	if (m_fTimeCount >= 2.f)
	{
		switch (m_iPattCount)
		{
		case 0:
			pOwner->Get_FSM()->ChangeState(CNiflheim_PATT1::Instance());
			return;
			break;
		case 1:
			pOwner->Get_FSM()->ChangeState(CNiflheim_PATT2::Instance());
			return;
			break;
		case 2:
			pOwner->Get_FSM()->ChangeState(CNiflheim_PATT3::Instance());
			return;
			break;
		case 3:
			pOwner->Get_FSM()->ChangeState(CNiflheim_PATT4::Instance());
			return;
			break;
		case 4:
			pOwner->Get_FSM()->ChangeState(CNiflheim_PATT5::Instance());
			return;
			break;
		case 5:
			pOwner->Get_FSM()->ChangeState(CNiflheim_PATT6::Instance());
			return;
			break;
		default:
			break;
		}
		
	}
}

void CNiflheim_IDLE::Exit(CNiflheim *pOwner)
{
}

CNiflheim_STUN * CNiflheim_STUN::Instance()
{
	static CNiflheim_STUN Instance;
	return &Instance;
}

void CNiflheim_STUN::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_iOldHp = pOwner->Get_Info()->iHp;
	pOwner->Set_Frame(L"Boss", L"BossStun", 0.f, 6.f);
}

void CNiflheim_STUN::Execute(CNiflheim * pOwner)
{
	m_fTimeCount += GET_TIME;
	if(m_fTimeCount >= 1.f / 6.f * 5.f)
		pOwner->SetStopFrame(true);
	if (pOwner->Get_Info()->iHp <= m_iOldHp - BOSS_HP / 4 || m_fTimeCount >= 5.f)
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}
}

void CNiflheim_STUN::Exit(CNiflheim * pOwner)
{
	pOwner->SetStopFrame(false);
}

CNiflheim_PATT1 * CNiflheim_PATT1::Instance()
{
	static CNiflheim_PATT1 Instance;
	return &Instance;
}

void CNiflheim_PATT1::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_fShotCount = 0.f;
}

void CNiflheim_PATT1::Execute(CNiflheim * pOwner)
{
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	m_fTimeCount += GET_TIME;
	m_fShotCount += GET_TIME;
	pOwner->GongIce(90.f * GET_TIME);
	if (m_fShotCount >= 0.1f)
	{
		pOwner->ShotIce();
		m_fShotCount = 0.f;
	}

	if (m_fTimeCount >= 3.f)
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}
}

void CNiflheim_PATT1::Exit(CNiflheim * pOwner)
{
}

CNiflheim_PATT2 * CNiflheim_PATT2::Instance()
{
	static CNiflheim_PATT2 Instance;
	return &Instance;
}

void CNiflheim_PATT2::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_fShotCount = 0.f;
}

void CNiflheim_PATT2::Execute(CNiflheim * pOwner)
{
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	m_fTimeCount += GET_TIME;
	m_fShotCount += GET_TIME;
	pOwner->ZaIce(720.f * GET_TIME);
	if (m_fShotCount >= 0.1f)
	{
		pOwner->ShotIce();
		m_fShotCount = 0.f;
	}

	if (m_fTimeCount >= 4.f)
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}
}

void CNiflheim_PATT2::Exit(CNiflheim * pOwner)
{
}

CNiflheim_PATT3 * CNiflheim_PATT3::Instance()
{
	static CNiflheim_PATT3 Instance;
	return &Instance;
}

void CNiflheim_PATT3::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_fShotCount = 0.f;
	pOwner->SetPatt3IceAngle();
}

void CNiflheim_PATT3::Execute(CNiflheim * pOwner)
{
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	m_fTimeCount += GET_TIME;
	if (m_fTimeCount < 1.f)
	{
		pOwner->SetIceDist(200.f * GET_TIME);
		m_vTargetPos = pOwner->Get_PlayerPos();
	}
	else if (m_fTimeCount < 2.f)
	{
		m_fShotCount += GET_TIME;
		pOwner->ZaIce(720.f * GET_TIME);
		if (m_fShotCount > 0.05f)
		{
			pOwner->ShotTargetIce(m_vTargetPos);
			m_fShotCount = 0.f;
		}
	}
	else if (m_fTimeCount < 2.1f)
	{
		pOwner->ZaIce(360.f * GET_TIME);
		m_vTargetPos = pOwner->Get_PlayerPos();
	}
	else if (m_fTimeCount < 3.f)
	{
		m_fShotCount += GET_TIME;
		pOwner->ZaIce(360.f * GET_TIME);
		if (m_fShotCount > 0.05f)
		{
			pOwner->ShotTargetIce(m_vTargetPos);
			m_fShotCount = 0.f;
		}
	}
	else if (m_fTimeCount < 3.1f)
	{
		pOwner->ZaIce(360.f * GET_TIME);
		m_vTargetPos = pOwner->Get_PlayerPos();
	}
	else if (m_fTimeCount < 4.f)
	{
		m_fShotCount += GET_TIME;
		pOwner->ZaIce(360.f * GET_TIME);
		if (m_fShotCount > 0.05f)
		{
			pOwner->ShotTargetIce(m_vTargetPos);
			m_fShotCount = 0.f;
		}
	}
	else if (m_fTimeCount < 4.1f)
	{
		pOwner->ZaIce(360.f * GET_TIME);
		m_vTargetPos = pOwner->Get_PlayerPos();
	}
	else if (m_fTimeCount < 5.f)
	{
		m_fShotCount += GET_TIME;
		pOwner->ZaIce(360.f * GET_TIME);
		if (m_fShotCount > 0.05f)
		{
			pOwner->ShotTargetIce(m_vTargetPos);
			m_fShotCount = 0.f;
		}
	}
	else if (m_fTimeCount < 6.f)
	{
		pOwner->ZaIce(360.f * GET_TIME);
	}
	else if (m_fTimeCount < 7.f)
	{
		pOwner->SetIceDist(-200.f * GET_TIME);
		pOwner->SetIceZaAngle(0.f);
	}
	else
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}
}

void CNiflheim_PATT3::Exit(CNiflheim * pOwner)
{
	pOwner->SetPatt3IceAngleEnd();
}

CNiflheim_PATT4 * CNiflheim_PATT4::Instance()
{
	static CNiflheim_PATT4 Instance;
	return &Instance;
}

void CNiflheim_PATT4::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_fShotCount = 0.f;
	m_bRight = false;
}

void CNiflheim_PATT4::Execute(CNiflheim * pOwner)
{
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	pOwner->GongIce(90.f * GET_TIME);
	m_fTimeCount += GET_TIME;
	m_fShotCount += GET_TIME;
	if (m_fShotCount > 1.5f)
	{
		VEC3 vPos = pOwner->Get_PlayerPos();
		if (m_bRight)
			vPos.x = 0;
		if (!m_bRight)
			vPos.x = TILECX * TILE_COUNT_X;
		pOwner->ShotSpear(vPos, m_bRight);
		m_bRight = !m_bRight;
		m_fShotCount = 0.f;
	}
	if (m_fTimeCount > 7.6f)
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}
}

void CNiflheim_PATT4::Exit(CNiflheim * pOwner)
{
}

CNiflheim_PATT5 * CNiflheim_PATT5::Instance()
{
	static CNiflheim_PATT5 Instance;
	return &Instance;
}

void CNiflheim_PATT5::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_fShotCount = 0.f;
}

void CNiflheim_PATT5::Execute(CNiflheim * pOwner)
{
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	pOwner->GongIce(90.f * GET_TIME);
	m_fTimeCount += GET_TIME;
	m_fShotCount += GET_TIME;
	if (m_fTimeCount < 5.5f)
	{
		if (m_fShotCount > 1.0f)
		{
			VEC3 vPos = pOwner->Get_PlayerPos();
			vPos.y = 0.f;
			pOwner->ShotIcicle(vPos);
			m_fShotCount = 0.f;
		}
	}
	else
	{
		for (int i = 0; i < TILECX * TILE_COUNT_X; i += TILECX * 5)
		{
			VEC3 vPos = {(float)i, 0.f, 0.f};
			pOwner->ShotIcicle(vPos);
		}
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}

}

void CNiflheim_PATT5::Exit(CNiflheim * pOwner)
{
}

CNiflheim_PATT6 * CNiflheim_PATT6::Instance()
{
	static CNiflheim_PATT6 Instance;
	return &Instance;
}

void CNiflheim_PATT6::Enter(CNiflheim * pOwner)
{
	m_fTimeCount = 0.f;
	m_fShotCount = 0.f;
	m_bMoveEnd = false;
}

void CNiflheim_PATT6::Execute(CNiflheim * pOwner)
{
	if (pOwner->IsAttackPossible())
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_STUN::Instance());
		return;
	}
	m_fTimeCount += GET_TIME;
	
	if (m_fTimeCount < 0.5f)
	{
		VEC3 vDir = { 1.f, -sqrtf(2.f), 0.f };
		VEC3NORMAL(&vDir, &vDir);
		pOwner->Move_Pos(vDir * 500 * GET_TIME);
	}
	else if (m_fTimeCount < 3.f)
	{
		pOwner->CameraShakingOn(true, int(m_fTimeCount) + 1);
		if (!m_bMoveEnd && m_fTimeCount < 2.f)
		{
			pOwner->SetIceDist(25.f);
			pOwner->SetPatt6IceAngle();
			m_bMoveEnd = true;
		}
		pOwner->SetPatt6IceCharginAngle();
		m_vTargetPos = pOwner->Get_PlayerPos() - pOwner->Get_Info()->vPos;
		m_fShotCount = 0.f;
		if (m_bMoveEnd && m_fTimeCount > 2.1f)
		{
			pOwner->Set_Frame(L"Boss", L"BossAttack", 0.f, 11.f);
			m_bMoveEnd = false;
		}
	}
	else if (m_fTimeCount < 6.f)
	{
		pOwner->SetStopFrame(true);
		pOwner->SetPatt6IceAngle();
		VEC3NORMAL(&m_vTargetPos, &m_vTargetPos);
		pOwner->CameraShakingOn(true, 10);
		m_fShotCount += GET_TIME;
		if (m_fShotCount >= 0.05f)
		{
			pOwner->ShotDirIce(m_vTargetPos, 400.f);
			m_fShotCount = 0.f;
		}
	}
	else if (m_fTimeCount < 7.f)
	{
		if (!m_bMoveEnd)
		{
			pOwner->Set_Frame(L"Boss", L"BossIdle", 0.f, 6.f);
			m_bMoveEnd = true;
		}
		pOwner->SetStopFrame(false);
		pOwner->CameraShakingOn(false, 0);
	}
	else if (m_fTimeCount < 7.5f)
	{
		VEC3 vDir = { 1.f, -sqrtf(2.f), 0.f };
		VEC3NORMAL(&vDir, &vDir);
		pOwner->Move_Pos(-vDir * 500 * GET_TIME);
	}
	else
	{
		pOwner->Get_FSM()->ChangeState(CNiflheim_IDLE::Instance());
		return;
	}
}

void CNiflheim_PATT6::Exit(CNiflheim * pOwner)
{
	pOwner->SetIceDist(-25.f);
	pOwner->SetPatt6IceAngleEnd();
}
