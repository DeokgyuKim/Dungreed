#pragma once
#include "Ring.h"
class CSpeedRing :
	public CRing
{
private:
	CSpeedRing();
	virtual ~CSpeedRing();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CSpeedRing* Create();
public:
	virtual void EffectRing() override;
};

