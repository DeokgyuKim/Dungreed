#pragma once
#include "Obj.h"

class CMyEffect
	: public CObj
{
protected:
	CMyEffect();
	virtual ~CMyEffect();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CMyEffect*	Create(VEC3 vPos, float fLifeTime, TCHAR* wstrObjectKey, TCHAR* wstrStateKey, float fCount);
protected:
	float				m_fLifeTime;
	float				m_fDeadTime;
	float				m_fLateCount;
	EFFECTMGR::TYPE		m_eEffType;
};

