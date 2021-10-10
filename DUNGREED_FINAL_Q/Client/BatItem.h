#pragma once
#include "SubItem.h"
class CBatItem :
	public CSubItem
{
public:
	CBatItem();
	virtual ~CBatItem();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CBatItem* Create();
private:
	float	m_fTimeCount;
	bool	m_bCreate;
};
