#pragma once
#include "SubItem.h"
class CShield :
	public CSubItem
{
private:
	CShield();
	virtual ~CShield();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CShield* Create();
};

