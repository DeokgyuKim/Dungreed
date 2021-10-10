#pragma once
#include "Sword.h"
class CFireSword :
	public CSword
{
private:
	CFireSword();
	virtual ~CFireSword();
public:
	virtual void Attack() override;
protected:
	virtual HRESULT Initialize() override;
public:
	static CFireSword* Create();
};

