#pragma once
#include "Bullet.h"
class CIceBullet :
	public CBullet
{
private:
	CIceBullet();
	virtual ~CIceBullet();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	virtual void	FrameUpdate();
public:
	static CIceBullet* Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
	static CIceBullet* CreateImmediately(VEC3 vPos, float fSpeed, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
private:
	float	m_fTimeCount;
	bool	m_bCreate;
};

