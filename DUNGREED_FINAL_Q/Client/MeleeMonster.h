#pragma once
#include "Monster.h"
class CMeleeMonster :
	public CMonster
{
public:
	CMeleeMonster();
	virtual ~CMeleeMonster();
public:
	virtual bool	Attack() override;
	virtual bool	Searching() override;
	virtual void	Following() override;
	MELEE_MONSTER::TYPE	Get_MeleeMonType() { return m_eMeleeMonType; }
public:
	virtual void	UpdateBoxs() PURE;
protected:
	RECT				m_tSearchingBox;
	RECT				m_tAttRangeBox;
	MELEE_MONSTER::TYPE	m_eMeleeMonType;
};

