#pragma once
#include "Ring.h"
class CTimeRing :
	public CRing
{
private:
	CTimeRing();
	virtual ~CTimeRing();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CTimeRing* Create();
public:
	virtual void EffectRing() override;
};

