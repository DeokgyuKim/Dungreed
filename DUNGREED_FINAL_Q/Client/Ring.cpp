#include "stdafx.h"
#include "Ring.h"
#include "PlayerObserver.h"

CRing::CRing()
{
	m_pObserver = CPlayerObserver::Create();
	CSubject::GetInstance()->Subscribe(m_pObserver);
}


CRing::~CRing()
{
	CSubject::GetInstance()->UnSubscribe(m_pObserver);
}
