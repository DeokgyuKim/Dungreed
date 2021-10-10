#pragma once
#include "Command.h"
class CPlayerCommand
{
public:
	CPlayerCommand();
	~CPlayerCommand();
};

class PlayerMoveLeft
	: public Command
{
public:
	PlayerMoveLeft() {}
	virtual ~PlayerMoveLeft() {}
	virtual void Execute(CObj * pActor) override;
};

class PlayerMoveRight
	: public Command
{
public:
	PlayerMoveRight() {}
	virtual ~PlayerMoveRight() {}
	virtual void Execute(CObj * pActor) override;
};


class PlayerMoveDown
	: public Command
{
public:
	PlayerMoveDown() {}
	virtual ~PlayerMoveDown() {}
	virtual void Execute(CObj * pActor) override;
};


class PlayerJump
	: public Command
{
public:
	PlayerJump() {}
	virtual ~PlayerJump() {}
	virtual void Execute(CObj * pActor) override;
};

class PlayerAttack
	: public Command
{
public:
	PlayerAttack() {}
	virtual ~PlayerAttack() {}
	virtual void Execute(CObj * pActor) override;
};

class PlayerDash
	: public Command
{
public:
	PlayerDash() {}
	virtual ~PlayerDash() {}
	virtual void Execute(CObj * pActor) override;
};
