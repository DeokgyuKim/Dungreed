#pragma once
#include "Bullet.h"

class CBanshBullet :
	public CBullet
{
private:
	CBanshBullet();
	~CBanshBullet();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CBanshBullet* Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
private:
	float	m_fTimeCount;
};

