#pragma once
#include "Bullet.h"
class CIceSpear :
	public CBullet
{
private:
	CIceSpear();
	virtual ~CIceSpear();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CIceSpear* Create(VEC3 vPos, int iAtt, bool bRight, HITBOX::TYPE eType);
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
private:
	float	m_fTimeCount;
	bool	m_bCreate;
	bool	m_bRight;
};

