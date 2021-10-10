#pragma once
#include "Effect.h"

class CNormalEffect : public CEffect
{
private:
	CNormalEffect();

public:
	virtual ~CNormalEffect();

public:
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CNormalEffect* Create(const D3DXVECTOR3& vPos, CEffectIMP* pImp);
};

