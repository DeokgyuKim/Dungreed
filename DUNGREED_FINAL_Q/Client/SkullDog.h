#pragma once
#include "MeleeMonster.h"
#include "StateMachine.h"

class CSkullDog :
	public CMeleeMonster
{
private:
	CSkullDog();
	virtual ~CSkullDog();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual void UpdateBoxs() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	VEC3	FindDir();
public:
	static CSkullDog* Create(VEC3 vPos);
private:
	StateMachine<CSkullDog>*	m_pState;
public:
	inline StateMachine<CSkullDog>* Get_FSM() { return m_pState; }
};

