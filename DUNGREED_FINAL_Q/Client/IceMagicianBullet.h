#pragma once
#include "Bullet.h"
class CIceMagicianBullet :
	public CBullet
{
private:
	CIceMagicianBullet();
	virtual ~CIceMagicianBullet();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	virtual void	FrameUpdate();
public:
	static CIceMagicianBullet* Create(VEC3 vPos, int iAtt, HITBOX::TYPE eType);
private:
	float	m_fTimeCount;
	bool	m_bShot;
};

