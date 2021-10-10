#include "stdafx.h"
#include "Weapon.h"
#include "PlayerObserver.h"

CWeapon::CWeapon()
{
	m_pObserver = CPlayerObserver::Create();
	CSubject::GetInstance()->Subscribe(m_pObserver);
	m_tInfo.m_iGold = 100;
}


CWeapon::~CWeapon()
{
	CSubject::GetInstance()->UnSubscribe(m_pObserver);
}
