#pragma once
#include "Obj.h"

class CMonster :
	public CObj
{
public:
	CMonster();
	virtual ~CMonster();
public:
	virtual bool	Attack() PURE;
	virtual bool	Searching() PURE;
	virtual void	Following() PURE;
	MONSTER::TYPE	Get_MonType() { return m_eMonType; }
public:
	virtual void	Render() override;
	void			RenderHp();
public:
	inline void		Set_Look(bool bLook) { m_bLook = bLook; }
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
protected:
	void			CreateMeleeHit(VEC3 vDir);
protected:
	CObserver*		m_pObserver;
	MONSTER::TYPE	m_eMonType;
	bool			m_bLook;
	float			m_fLookTime;
	int				m_iFullHp;
};

