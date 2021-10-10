#pragma once
#include "Bullet.h"
class CObserver;
class CDartBullet :
	public CBullet
{
private:
	CDartBullet();
	virtual ~CDartBullet();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	inline const float Get_Time() const { return m_fTimeCount; }
public:
	static CDartBullet* Create(VEC3 vPos, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
	virtual void		Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
private:
	float			m_fTimeCount;
	bool			m_bReturn;
	CObserver*		m_pObserver;
};

