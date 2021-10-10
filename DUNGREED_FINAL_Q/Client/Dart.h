#pragma once
#include "Weapon.h"
class CDartBullet;
class CDart :
	public CWeapon
{
private:
	CDart();
	virtual ~CDart();
public:
	virtual int			Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render() override;
public:
	virtual void		Attack() override;
	virtual void		AttackCameraShaking() override;
protected:
	virtual HRESULT		Initialize() override;
	virtual void		Release() override;
public:
	static CDart*		Create();
private:
	CDartBullet*		m_pDartBullet;
};

