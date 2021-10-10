#pragma once
#include "Obj.h"
class CInventory :
	public CObj
{
	DECLARE_SINGLETON(CInventory)
private:
	CInventory();
	virtual ~CInventory();
public:
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render() override;
	void			Render(MATRIX* matParentWorld);
	CObj*			GetItem(int iIndex);
	void			PushItem(int iIndex, CObj* pObj) {
		m_vecItem[iIndex] = pObj;
	}
	int				GetIndex();
	int				GetGold() { return m_iGold; }
	void			PlusGold(int iGold) { m_iGold += iGold; }
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
private:
	vector<CObj*>	m_vecItem;
	RECT			m_tItemRect[20];
	int				m_iIndex;
	int				m_iGold;
};

