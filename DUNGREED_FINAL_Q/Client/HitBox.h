#pragma once
#include "Obj.h"
class CHitBox :
	public CObj
{
public:
	CHitBox();
	virtual ~CHitBox();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	virtual int		ReverseUpdate();
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CHitBox* Create(VEC3 vPos, VEC3 vSize, VEC3 vDir, int iAtt, HITBOX::TYPE eType);
public:
	inline HITBOX::TYPE	Get_HitBoxType() { return m_eHitBoxType; }
protected:
	HITBOX::TYPE	m_eHitBoxType;
	int				m_iCount;
};

