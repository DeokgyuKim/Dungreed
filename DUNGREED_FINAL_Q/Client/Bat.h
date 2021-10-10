#pragma once
#include "RangedMonster.h"
#include "StateMachine.h"

class CBatBullet;
class CBat :
	public CRangedMonster
{
public:
	CBat();
	virtual ~CBat();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void	Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	virtual void	UpdateBoxs() override;
	void			CreateBullet();
public:
	static CBat* Create(VEC3 vPos);
private:
	StateMachine<CBat>*	m_pState;
	list<CBatBullet*>	m_pBullet;
public:
	inline StateMachine<CBat>* Get_FSM() { return m_pState; }
};

