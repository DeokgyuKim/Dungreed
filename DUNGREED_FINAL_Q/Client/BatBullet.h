#pragma once
#include "Bullet.h"
class CBatBullet :
	public CBullet
{
public:
	CBatBullet();
	virtual ~CBatBullet();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	inline const float Get_Time() const { return m_fTimeCount; }
public:
	static CBatBullet* Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
private:
	float	m_fTimeCount;
};

