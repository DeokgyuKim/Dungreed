#pragma once
#include "Obj.h"

// �߻���

class CEffectIMP;
class CEffect : public CObj
{
protected:
	CEffect();

public:
	virtual ~CEffect();

public:
	// CObj��(��) ���� ��ӵ�
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

