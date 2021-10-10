#pragma once
#include "Obj.h"
typedef struct tagAngle {
	float fLookAngle;
	float fSwingAngle;
}ANGLE;
class CWeapon :
	public CObj
{
public:
	CWeapon();
	virtual ~CWeapon();
public:
	virtual void	Attack()				PURE;
	virtual void	AttackCameraShaking()	PURE;
public:
	inline float	Get_AttSpeed() { return m_fAttSpeed; }
	inline void		Set_boolAtt(bool bAtt) { m_bAtt = bAtt; }
protected:
	float	m_fAttSpeed;
	float	m_fAttTime;
	bool	m_bAtt;
	float	m_fCoolTime;
	float	m_fCoolTimeCount;
	CObserver*	m_pObserver;
};

