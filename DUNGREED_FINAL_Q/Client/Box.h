#pragma once
#include "Monster.h"
class CBox :
	public CMonster
{
private:
	CBox();
	virtual ~CBox();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CBox* Create(VEC3 vPos, BOX::TYPE eType);
private:
	BOX::TYPE	m_eBoxType;
public:
	virtual bool Attack() override;
	virtual bool Searching() override;
	virtual void Following() override;
};

