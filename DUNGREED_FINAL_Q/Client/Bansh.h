#pragma once
#include "RangedMonster.h"
#include "StateMachine.h"

class CBansh :
	public CRangedMonster
{
private:
	CBansh();
	virtual ~CBansh();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	virtual void	UpdateBoxs() override;
	void			CreateBullet();
public:
	static CBansh*	Create(VEC3 vPos);
	StateMachine<CBansh>*	m_pState;
public:
	inline StateMachine<CBansh>* Get_FSM() { return m_pState; }
};

