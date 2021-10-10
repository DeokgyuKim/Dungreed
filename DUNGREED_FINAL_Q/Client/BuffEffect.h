#pragma once
#include "Effect.h"
class CBuffEffect :
	public CEffect
{
private:
	CBuffEffect();

public:
	virtual ~CBuffEffect();

public:
	virtual int Update()		override;
	virtual void LateUpdate()	override;
	virtual void Render()		override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBuffEffect* Create(CEffectIMP* pImp);

private:
	CObjectMgr*	m_pObjectMgr;
	float		m_fAngle;
};

