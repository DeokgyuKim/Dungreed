#pragma once
#include "EffectIMP.h"
class CNoAnimateIMP :
	public CEffectIMP
{
private:
	CNoAnimateIMP();

public:
	virtual ~CNoAnimateIMP();

public:
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(const D3DXMATRIX& matWorld)		override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release()		override;

public:
	static CNoAnimateIMP* Create(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const float& fLifeTime);

private:
	float	m_fLifeTime;
	float	m_fTimeCount;
};

