#pragma once
#include "Weapon.h"

class CSword :
	public CWeapon
{
protected:
	CSword();
	virtual ~CSword();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	virtual int		ReverseUpdate();
	virtual void	ReverseLateUpdate();
public:
	virtual void Attack() override;
	virtual void AttackCameraShaking() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	void			IsSwing();
	virtual void	SaveOldInfo();
	virtual void	DeleteOldInfo() { m_lstOldInfo.clear();  m_lstOldInfo.push_back(make_pair(m_tInfo, m_tFrame)); 
	m_lstOldInfoEx.clear();  m_lstOldInfoEx.push_back(m_tAngle);
	}
public:
	static CSword* Create();
protected:
	bool				m_bUpper;
	ANGLE				m_tAngle;
	list<ANGLE>			m_lstOldInfoEx;
	float				m_fShakeValue;
};

