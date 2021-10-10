#pragma once
#include "Monster.h"
#include "StateMachine.h"

class CIcePillar;
class CNiflheim :
	public CMonster
{
private:
	CNiflheim();
	virtual ~CNiflheim();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	virtual bool Attack() override;
	virtual bool Searching() override;
	virtual void Following() override;
public:
	void		 MakeIcePillar();
	void		 ShotIce();
	void		 ShotSpear(VEC3 vPos, bool bRight);
	void		 ShotIcicle(VEC3 vPos);
	void		 ShotDirIce(VEC3 vDir, float fSpeed);
	void		 ShotTargetIce(VEC3 vPos);
	void		 ShotTargetIceImmediately(VEC3 vPos);
	void		 ZaIce(float fAngle);
	void		 GongIce(float fAngle);

	void		 CameraShakingOn(bool bShake, int iValue) { m_bShake = bShake; m_iShakeValue = iValue; }

	void		 SetIceZaAngle(float fAngle);
	void		 SetIceGongAngle(float fAngle);

	void		 SetIceDist(float fDist);
	void		 SetPatt3IceAngle();
	void		 SetPatt3IceAngleEnd();

	void		 SetPatt6IceAngle();
	void		 SetPatt6IceCharginAngle();
	void		 SetPatt6IceAngleEnd();

	void		 SetStopFrame(bool bStop) { m_bStopFrame = bStop; }
public:
	inline bool	IsAttackPossible() { 
		if (m_lstIcePillar.empty()) return true;  
		return false;
	}
	list<CIcePillar*>* Get_IcePillar() { return &m_lstIcePillar; }
	VEC3	Get_PlayerPos();
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
public:
	static CNiflheim* Create(VEC3 vPos);
private:
	list<CIcePillar*> m_lstIcePillar;
	StateMachine<CNiflheim>*	m_pState;
	bool						m_bStopFrame;
	bool						m_bShake;
	int							m_iShakeValue;
	float						m_fMakeCount;
	bool						m_bMake;
	float						m_fDeadCount;
public:
	inline StateMachine<CNiflheim>* Get_FSM() { return m_pState; }
};

