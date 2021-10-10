#pragma once
#include "Weapon.h"
class CSpear :
	public CWeapon
{
private:
	CSpear();
	virtual ~CSpear();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	virtual int		ReverseUpdate();
	virtual void	ReverseLateUpdate();
public:
	virtual void Attack() override;
	virtual void AttackCameraShaking() override;
protected:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	void			IsAttack();
	virtual void	SaveOldInfo();
	virtual void	DeleteOldInfo() {
		m_lstOldInfo.clear();  m_lstOldInfo.push_back(make_pair(m_tInfo, m_tFrame));
		m_lstOldInfoEx.clear();  m_lstOldInfoEx.push_back(m_fValue);
	}
public:
	static CSpear*		Create();
public:
	bool				m_bStab;
	float				m_fValue;
	float				m_fShakeValue;
	list<float>			m_lstOldInfoEx;
};

