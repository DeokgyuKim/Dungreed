#pragma once
#include "Bullet.h"
class CIcicle :
	public CBullet
{
private:
	CIcicle();
	virtual ~CIcicle();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CIcicle* Create(VEC3 vPos, int iAtt, HITBOX::TYPE eType);
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
private:
	float	m_fTimeCount;
	bool	m_bCreate;
};

