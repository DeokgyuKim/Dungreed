#pragma once
#include "Obj.h"

// 추상층

class CEffectIMP;
class CEffect : public CObj
{
protected:
	CEffect();

public:
	virtual ~CEffect();

public:
	// CObj을(를) 통해 상속됨
	virtual int Update()		PURE;
	virtual void LateUpdate()	PURE;
	virtual void Render()		PURE;

private:
	virtual HRESULT Initialize() PURE;
	virtual void Release() override;

protected:
	CEffectIMP*	m_pImp;	
	INFO		m_tInfo;
};

