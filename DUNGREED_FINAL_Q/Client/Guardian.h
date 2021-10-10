#pragma once
#include "MeleeMonster.h"
#include "StateMachine.h"

class CGuardian
	: public CMeleeMonster
{
public:
	CGuardian();
	virtual ~CGuardian();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual void UpdateBoxs() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CGuardian* Create(VEC3 vPos);
public:
	VEC3	Get_PlayerPos();
private:
	StateMachine<CGuardian>*	m_pState;
public:
	inline StateMachine<CGuardian>* Get_FSM() { return m_pState; }
};

