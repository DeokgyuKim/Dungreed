#pragma once
#include "EffectIMP.h"
class CAnimateIMP : public CEffectIMP
{
private:
	CAnimateIMP();

public:
	virtual ~CAnimateIMP();

public:
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render(const D3DXMATRIX& matWorld)		override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release()		override;

public:
	static CAnimateIMP* Create(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey,
		const float& fFrameStart,
		const float& fMaxFrameCount);

private:
	float		m_fFrameStart;
	float		m_fMaxFrameCount;
};

