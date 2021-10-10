#include "stdafx.h"
#include "PlayerCommand.h"
#include "Player.h"

CPlayerCommand::CPlayerCommand()
{
}


CPlayerCommand::~CPlayerCommand()
{
}

void PlayerMoveLeft::Execute(CObj * pActor)
{
	SCAST(CPlayer*, pActor)->MoveLeft();
}

void PlayerMoveRight::Execute(CObj * pActor)
{
	SCAST(CPlayer*, pActor)->MoveRight();
}

void PlayerMoveDown::Execute(CObj * pActor)
{
	SCAST(CPlayer*, pActor)->MoveDown();
}

void PlayerJump::Execute(CObj * pActor)
{
	SCAST(CPlayer*, pActor)->Jump();
}

void PlayerAttack::Execute(CObj * pActor)
{
	SCAST(CPlayer*, pActor)->Attack();
}

void PlayerDash::Execute(CObj * pActor)
{
	SCAST(CPlayer*, pActor)->Dash();
}