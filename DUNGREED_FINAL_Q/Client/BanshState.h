#pragma once
#include "Bansh.h"
#include "State.h"

class CBanshState
{
public:
	CBanshState();
	~CBanshState();
};

class CBanshIdle
	: public State<CBansh>
{
	float	m_fTimeCount;
	~CBanshIdle() {}
public:
	static CBanshIdle* Instance();
	virtual void Enter(CBansh* pOwner) override;
	virtual void Execute(CBansh* pOwner) override;
	virtual void Exit(CBansh* pOwner) override;
};

class CBanshFollow
	: public State<CBansh>
{
	float	m_fTimeCount;
	~CBanshFollow() {}
public:
	static CBanshFollow* Instance();
	virtual void Enter(CBansh* pOwner) override;
	virtual void Execute(CBansh* pOwner) override;
	virtual void Exit(CBansh* pOwner) override;
};

class CBanshAttack
	: public State<CBansh>
{
	float		m_fTimeCount;
	~CBanshAttack() {}
public:
	static CBanshAttack* Instance();
	virtual void Enter(CBansh* pOwner) override;
	virtual void Execute(CBansh* pOwner) override;
	virtual void Exit(CBansh* pOwner) override;
};