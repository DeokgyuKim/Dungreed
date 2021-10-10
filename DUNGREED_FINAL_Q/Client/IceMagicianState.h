#pragma once
#include "State.h"
#include "IceMagician.h"

class CIceMagicianState
{
public:
	CIceMagicianState();
	~CIceMagicianState();
};

class CIceMagicianIdle
	: public State<CIceMagician>
{
	float	m_fTimeCount;
	~CIceMagicianIdle() {}
public:
	static CIceMagicianIdle* Instance();
	virtual void Enter(CIceMagician* pOwner) override;
	virtual void Execute(CIceMagician* pOwner) override;
	virtual void Exit(CIceMagician* pOwner) override;
};

class CIceMagicianAttack
	: public State<CIceMagician>
{
	float		m_fTimeCount;
	bool		m_bShot;
	~CIceMagicianAttack() {}
public:
	static CIceMagicianAttack* Instance();
	virtual void Enter(CIceMagician* pOwner) override;
	virtual void Execute(CIceMagician* pOwner) override;
	virtual void Exit(CIceMagician* pOwner) override;
};