#pragma once
#include "MyEffect.h"
class CNiflDeadEffect :
	public CMyEffect
{
private:
	CNiflDeadEffect();
	virtual ~CNiflDeadEffect();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
protected:
	virtual HRESULT Initialize() override;
public:
	static CNiflDeadEffect*	Create(VEC3 vPos, float fLifeTime, TCHAR* wstrObjectKey, TCHAR* wstrStateKey, float fCount);
private:
	float m_fTimeCount;
};

