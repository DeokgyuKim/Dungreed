#pragma once
#include "State.h"
#include "SkullDog.h"

class CSkullDogState :
	public State<CSkullDog>
{
public:
	CSkullDogState();
	virtual ~CSkullDogState();
};

class CSkullDogIdle
	: public State<CSkullDog>
{
	float	m_fTimeCount;
	~CSkullDogIdle() {}
public:
	static CSkullDogIdle* Instance();
	virtual void Enter(CSkullDog* pOwner) override;
	virtual void Execute(CSkullDog* pOwner) override;
	virtual void Exit(CSkullDog* pOwner) override;
};

class CSkullDogFollow
	: public State<CSkullDog>
{
	~CSkullDogFollow() {}
public:
	static CSkullDogFollow* Instance();
	virtual void Enter(CSkullDog* pOwner) override;
	virtual void Execute(CSkullDog* pOwner) override;
	virtual void Exit(CSkullDog* pOwner) override;
};

class CSkullDogAttack
	: public State<CSkullDog>
{
	float		m_fTimeCount;
	VEC3		m_vPos;
	~CSkullDogAttack() {}
public:
	static CSkullDogAttack* Instance();
	virtual void Enter(CSkullDog* pOwner) override;
	virtual void Execute(CSkullDog* pOwner) override;
	virtual void Exit(CSkullDog* pOwner) override;
};