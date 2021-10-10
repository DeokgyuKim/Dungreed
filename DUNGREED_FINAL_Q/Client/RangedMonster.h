#pragma once
#include "Monster.h"
class CRangedMonster :
	public CMonster
{
public:
	CRangedMonster();
	virtual ~CRangedMonster();
public:
	virtual bool	Attack() override;
	virtual bool	Searching() override;
	virtual void	Following() override;
	RANGED_MONSTER::TYPE Get_RangedType() { return m_eRangedMonType; }
public:
	virtual void	UpdateBoxs() PURE;
protected:
	RECT					m_tSearchingBox;
	RECT					m_tAttRangeBox;
	RANGED_MONSTER::TYPE	m_eRangedMonType;
};

