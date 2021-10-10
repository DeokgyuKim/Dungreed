#pragma once
#include "Weapon.h"
typedef struct tagGunInfo {
	ANGLE	m_tAngle;
	int		m_iBulletCount;
}GUNINFO;

class CGun :
	public CWeapon
{
protected:
	CGun();
	virtual ~CGun();
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
	virtual void	SaveOldInfo();
	virtual void	DeleteOldInfo() {
		m_lstOldInfo.clear();  m_lstOldInfo.push_back(make_pair(m_tInfo, m_tFrame));
		m_lstOldInfoEx.clear();  m_lstOldInfoEx.push_back(m_tGunInfo);
	}
public:
	static CGun* Create();
protected:
	GUNINFO				m_tGunInfo;
	list<GUNINFO>		m_lstOldInfoEx;
	bool				m_bReload;
	bool				m_bDoubleShot;
	float				m_fReloadTime;
	float				m_fShakeValue;
};

