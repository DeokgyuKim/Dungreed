#include "stdafx.h"
#include "SubItem.h"
#include "PlayerObserver.h"

CSubItem::CSubItem()
{
	m_pObserver = CPlayerObserver::Create();
	CSubject::GetInstance()->Subscribe(m_pObserver);
	m_tInfo.m_iGold = 100;
}

CSubItem::~CSubItem()
{
	CSubject::GetInstance()->UnSubscribe(m_pObserver);
}
