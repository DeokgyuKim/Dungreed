#pragma once
#include "Bullet.h"
class CStarBullet :
	public CBullet
{
private:
	CStarBullet();
	virtual ~CStarBullet();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CStarBullet* Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
private:
	float	m_fTimeCount;
	int		m_iAfterImage;
	CObj*	m_pTarget;
};

