#pragma once
#include "MyEffect.h"
class CResourceEffect :
	public CMyEffect
{
private:
	CResourceEffect();
	virtual ~CResourceEffect();
public:
	virtual int Update() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CResourceEffect*	Create(VEC3 vPos, float fLifeTime, TCHAR* wstrObjectKey, TCHAR* wstrStateKey, float fCount, float fRotate, RENDER::POS ePos);
private:
	float			m_fRotate;
	RENDER::POS		m_eRenderPos;
};

