#pragma once
#include "RangedMonster.h"
#include "StateMachine.h"
class CIceMagician :
	public CRangedMonster
{
private:
	CIceMagician();
	virtual ~CIceMagician();
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
	static CIceMagician*	Create(VEC3 vPos);
	StateMachine<CIceMagician>*	m_pState;
public:
	inline StateMachine<CIceMagician>* Get_FSM() { return m_pState; }
};

