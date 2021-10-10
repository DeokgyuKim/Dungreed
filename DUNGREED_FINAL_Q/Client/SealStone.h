#pragma once
#include "Obj.h"
class CSealStone :
	public CObj
{
public:
	CSealStone();
	virtual ~CSealStone();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	void	OnSeal();
public:
	static CSealStone* Create(VEC3 vPos);
private:
	bool	m_bOn;
};

