#pragma once
#include "State.h"
#include "Bat.h"

class CBatState
{
public:
	CBatState();
	~CBatState();
};

class CBatIdle
	: public State<CBat>
{
	float	m_fTimeCount;
	~CBatIdle() {}
public:
	static CBatIdle* Instance();
	virtual void Enter(CBat* pOwner) override;
	virtual void Execute(CBat* pOwner) override;
	virtual void Exit(CBat* pOwner) override;
};

class CBatFollow
	: public State<CBat>
{
	float	m_fTimeCount;
	~CBatFollow() {}
public:
	static CBatFollow* Instance();
	virtual void Enter(CBat* pOwner) override;
	virtual void Execute(CBat* pOwner) override;
	virtual void Exit(CBat* pOwner) override;
};

class CBatAttack
	: public State<CBat>
{
	float		m_fTimeCount;
	~CBatAttack() {}
public:
	static CBatAttack* Instance();
	virtual void Enter(CBat* pOwner) override;
	virtual void Execute(CBat* pOwner) override;
	virtual void Exit(CBat* pOwner) override;
};