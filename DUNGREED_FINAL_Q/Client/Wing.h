#pragma once
#include "SubItem.h"
class CWing :
	public CSubItem
{
private:
	CWing();
	virtual ~CWing();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CWing* Create();
};

