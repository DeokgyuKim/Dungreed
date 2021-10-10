#pragma once

class CObj;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)

private:
	CObjectMgr();
	~CObjectMgr();

public:
	CObj*			GetPlayer();
	CObj*			GetBoss();
	list<CObj*>*	GetLst(OBJECT_TYPE eType) { return &m_ObjectList[eType]; }
	CObj*			GetTarget(VEC3 vPos);
	bool			GetShopOn() { return m_bShop; }
	bool			GetReverse() { return m_bReverse; }
	bool			GetClear() { return m_bClear; }
	int		CountMonster(MONSTER::TYPE eType, MELEE_MONSTER::TYPE eMelType = MELEE_MONSTER::END, RANGED_MONSTER::TYPE eRanType = RANGED_MONSTER::END);
	int		CountHitBox(HITBOX::TYPE eType);
public:
	void	DeleteLst(OBJECT_TYPE eType);
	void	ClearLst(OBJECT_TYPE eType);
public:
	void	ShowShop() { m_bShop = !m_bShop; 
	if (m_bShop) m_bInven = true;
	if (!m_bShop) m_bInven = false;
	}
	void	TimeRingEffect() { m_bReverse = true; }
	void	SetClear() { m_bClear = true; }
public:
	HRESULT AddObject(OBJECT_TYPE eType, CObj* pObject);
	void Update();
	void LateUpdate();
	void Render(bool bPlayerRender = true);

private:
	void Release();

private:
	typedef list<CObj*>	OBJECT_LIST;
	OBJECT_LIST	m_ObjectList[OBJECT_END];
	bool		m_bReverse;
	bool		m_bInven;
	bool		m_bShop;
	bool		m_bClear;
	int			iCount;
};

