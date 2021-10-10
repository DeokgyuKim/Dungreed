#pragma once
#include "Obj.h"
class CSubItem :
	public CObj
{
protected:
	CSubItem();
	virtual ~CSubItem();
public:
	SUBITEM::TYPE	GetSubItemType() { return m_eSubItemType; }
protected:
	SUBITEM::TYPE	m_eSubItemType;
	CObserver*		m_pObserver;
};

