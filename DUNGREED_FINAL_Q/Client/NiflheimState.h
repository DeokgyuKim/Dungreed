#pragma once
#include "State.h"
#include "Niflheim.h"

class CNiflheimState
{
public:
	CNiflheimState();
	~CNiflheimState();
};

class CNiflheim_IDLE
	: public State<CNiflheim>
{
	static int	m_iPattCount;
	float		m_fTimeCount;
	~CNiflheim_IDLE() {};
public:
	static CNiflheim_IDLE* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_STUN
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	int		m_iOldHp;
	~CNiflheim_STUN() {};
public:
	static CNiflheim_STUN* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_PATT1
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	float	m_fShotCount;
	~CNiflheim_PATT1() {};
public:
	static CNiflheim_PATT1* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_PATT2
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	float	m_fShotCount;
	~CNiflheim_PATT2() {};
public:
	static CNiflheim_PATT2* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_PATT3
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	float	m_fShotCount;
	VEC3	m_vTargetPos;
	~CNiflheim_PATT3() {};
public:
	static CNiflheim_PATT3* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_PATT4
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	float	m_fShotCount;
	VEC3	m_vPos;
	VEC3	m_vDir;
	bool	m_bRight;
	~CNiflheim_PATT4() {};
public:
	static CNiflheim_PATT4* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_PATT5
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	float	m_fShotCount;
	VEC3	m_vPos;
	~CNiflheim_PATT5() {};
public:
	static CNiflheim_PATT5* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};

class CNiflheim_PATT6
	: public State<CNiflheim>
{
	float	m_fTimeCount;
	float	m_fShotCount;
	VEC3	m_vTargetPos;
	bool	m_bMoveEnd;
	~CNiflheim_PATT6() {};
public:
	static CNiflheim_PATT6* Instance();
	virtual void Enter(CNiflheim *pOwner) override;
	virtual void Execute(CNiflheim *pOwner) override;
	virtual void Exit(CNiflheim *pOwner) override;
};