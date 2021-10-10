#pragma once
#include "Obj.h"

class CShop
	: public CObj
{
private:
	CShop();
	~CShop();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CShop* Create(VEC3 vPos);
	virtual void	FrameUpdate();
private:
	FRAME m_tFrameNpc;
};

