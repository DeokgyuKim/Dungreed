#include "stdafx.h"
#include "InputMgr.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CInputMgr)

CInputMgr::CInputMgr()
{
}

CInputMgr::~CInputMgr()
{
	SafeDelete<Command*>(Jump);
	SafeDelete<Command*>(MoveLeft);
	SafeDelete<Command*>(MoveRight);
	SafeDelete<Command*>(MoveDown);
	SafeDelete<Command*>(Attack);
	SafeDelete<Command*>(Dash);
}

void CInputMgr::Initialize()
{
	Jump		= new PlayerJump;
	MoveLeft	= new PlayerMoveLeft;
	MoveDown	= new PlayerMoveDown;
	MoveRight	= new PlayerMoveRight;
	Attack		= new PlayerAttack;
	Dash		= new PlayerDash;

	m_dwKeySet[KEY::JUMP] = KEY_W;
	m_dwKeySet[KEY::MOVE_LEFT] = KEY_A;
	m_dwKeySet[KEY::MOVE_RIGHT] = KEY_D;
	m_dwKeySet[KEY::MOVE_DOWN] = KEY_S;
	m_dwKeySet[KEY::ATTACK] = KEY_LBUTTON;
	m_dwKeySet[KEY::DASH] = KEY_RBUTTON;
}

void CInputMgr::GetKeyDown()
{
	switch (SCAST(CPlayer*, m_pActor)->GetState())
	{
	case CPlayer::PLAYER_STATE::DASH:
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::ATTACK]))
			Attack->Execute(m_pActor);
		break;
	case CPlayer::PLAYER_STATE::JUMP:
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::MOVE_LEFT]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::RUN);
			MoveLeft->Execute(m_pActor);

		}
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::MOVE_RIGHT]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::RUN);
			MoveRight->Execute(m_pActor);

		}
		if (CKeyMgr::GetInstance()->KeyUp(m_dwKeySet[KEY::MOVE_LEFT]) ||
			CKeyMgr::GetInstance()->KeyUp(m_dwKeySet[KEY::MOVE_RIGHT]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::IDLE);
		}
		if (CKeyMgr::GetInstance()->KeyDown(m_dwKeySet[KEY::DASH]))
		{
			Dash->Execute(m_pActor);
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::DASH);
		}
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::ATTACK]))
			Attack->Execute(m_pActor);
		break;
	default:
		if (CKeyMgr::GetInstance()->KeyDown(m_dwKeySet[KEY::JUMP]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::JUMP);
			Jump->Execute(m_pActor);
		}
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::MOVE_LEFT]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::RUN);
			MoveLeft->Execute(m_pActor);

		}
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::MOVE_RIGHT]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::RUN);
			MoveRight->Execute(m_pActor);

		}
		if (CKeyMgr::GetInstance()->KeyUp(m_dwKeySet[KEY::MOVE_LEFT]) ||
			CKeyMgr::GetInstance()->KeyUp(m_dwKeySet[KEY::MOVE_RIGHT]))
		{
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::IDLE);
		}

		if (CKeyMgr::GetInstance()->KeyDown(m_dwKeySet[KEY::MOVE_DOWN]))
		{
			MoveDown->Execute(m_pActor);
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::JUMP);
		}
		if (CKeyMgr::GetInstance()->KeyDown(m_dwKeySet[KEY::DASH]))
		{
			Dash->Execute(m_pActor);
			SCAST(CPlayer*, m_pActor)->ChangeState(CPlayer::PLAYER_STATE::DASH);
		}
		if (CKeyMgr::GetInstance()->KeyPressing(m_dwKeySet[KEY::ATTACK]))
			Attack->Execute(m_pActor);
		break;
	}
}

void CInputMgr::Set_Actor(CObj * pObj)
{
	m_pActor = pObj;
}

void CInputMgr::ChangeKey()
{
	//if (m_dwKeySet[KEY::MOVEUP] == KEY_W)
	//{
	//	m_dwKeySet[KEY::MOVEUP] = KEY_UP;
	//	m_dwKeySet[KEY::MOVELEFT] = KEY_LEFT;
	//	m_dwKeySet[KEY::MOVEDOWN] = KEY_RIGHT;
	//	m_dwKeySet[KEY::MOVERIGHT] = KEY_DOWN;
	//}
	//else
	//{
	//	m_dwKeySet[KEY::MOVEUP] = KEY_W;
	//	m_dwKeySet[KEY::MOVELEFT] = KEY_A;
	//	m_dwKeySet[KEY::MOVEDOWN] = KEY_S;
	//	m_dwKeySet[KEY::MOVERIGHT] = KEY_D;
	//}
}
