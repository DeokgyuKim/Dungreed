#pragma once
#include "MyEffect.h"
class CAlphaEffect :
	public CMyEffect
{
protected:
	CAlphaEffect();
	virtual ~CAlphaEffect();
public:
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CAlphaEffect*	Create(VEC3 vPos, float fLifeTime, TCHAR* wstrObjectKey, TCHAR* wstrStateKey, float fCount, bool bRenderRight);
protected:
	DWORD m_dwAlpha;
	bool  m_bRenderRight;
};

