#pragma once
#include "Obj.h"
class CRing :
	public CObj
{
public:
	CRing();
	virtual ~CRing();
public:
	virtual void EffectRing() PURE;
public:
	RING::TYPE	GetRingType() { return m_eRingType; }
protected:
	RING::TYPE	m_eRingType;
	CObserver*	m_pObserver;
};

