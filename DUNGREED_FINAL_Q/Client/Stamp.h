#pragma once
#include "Weapon.h"
class CStamp :
	public CWeapon
{
private:
	CStamp();
	virtual ~CStamp();
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
	static CStamp*		Create();
};