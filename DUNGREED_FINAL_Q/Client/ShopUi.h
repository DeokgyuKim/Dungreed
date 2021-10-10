#pragma once
#include "Obj.h"
class CShopUi :
	public CObj
{
	DECLARE_SINGLETON(CShopUi)
private:
	CShopUi();
	virtual ~CShopUi();
public:
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render() override;
	CObj*			GetItem(int iIndex);
	void			PushItem(int iIndex, CObj* pObj) {
		m_vecItem[iIndex] = pObj;
	}
	RECT*			GetRect() { return m_tItemRect; }
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
private:
	vector<CObj*>	m_vecItem;
	RECT			m_tItemRect[6];
	int				m_iIndex;
};

