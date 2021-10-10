#pragma once
#include "HitBox.h"
class CBullet :
	public CHitBox
{
public:
	CBullet();
	virtual ~CBullet();
public:
	virtual int Update() override;
	virtual void Render() override;
public:
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
};

