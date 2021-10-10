#pragma once
#include "State.h"
#include "Guardian.h"

class CGuardianState :
	public State<CGuardian>
{
public:
	CGuardianState();
	virtual ~CGuardianState();
};

class CGuardianIdle
	: public State<CGuardian>
{
	float	m_fTimeCount;
	~CGuardianIdle() {}
public:
	static CGuardianIdle* Instance();
	virtual void Enter(CGuardian* pOwner) override;
	virtual void Execute(CGuardian* pOwner) override;
	virtual void Exit(CGuardian* pOwner) override;
};

class CGuardianFollow
	: public State<CGuardian>
{
	bool	m_bCrush;
	bool	m_bFollow;
	bool	m_bShow;
	float	m_fTimeCount;
	~CGuardianFollow() {}
public:
	static CGuardianFollow* Instance();
	virtual void Enter(CGuardian* pOwner) override;
	virtual void Execute(CGuardian* pOwner) override;
	virtual void Exit(CGuardian* pOwner) override;
};

class CGuardianAttack
	: public State<CGuardian>
{
	float		m_fTimeCount;
	bool		m_bAtt;
	VEC3		m_vPos;
	~CGuardianAttack() {}
public:
	static CGuardianAttack* Instance();
	virtual void Enter(CGuardian* pOwner) override;
	virtual void Execute(CGuardian* pOwner) override;
	virtual void Exit(CGuardian* pOwner) override;
};