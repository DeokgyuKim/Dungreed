#include "stdafx.h"
#include "HitBox.h"


CHitBox::CHitBox()
{
}


CHitBox::~CHitBox()
{
}

HRESULT CHitBox::Initialize()
{
	m_tInfo.bDead = false;
	m_eType = OBJ::HITBOX;
	m_iCount = 0;
	return S_OK;
}

int CHitBox::Update()
{
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_tInfo.bDead = true;
	return NO_EVENT;
}

void CHitBox::LateUpdate()
{
}

void CHitBox::Render()
{
	CLineMgr::DrawBox(m_tRect);
}

int CHitBox::ReverseUpdate()
{
	if (m_lstOldInfo.size() > 0)
	{
		m_tInfo = m_lstOldInfo.back().first;
		m_tFrame = m_lstOldInfo.back().second;
		m_lstOldInfo.pop_back();
		if (m_lstOldInfo.size() > 0)
			m_lstOldInfo.pop_back();
		if (m_lstOldInfo.empty())
			//m_lstOldInfo.emplace_back(make_pair(m_tInfo, m_tFrame));
			return DEAD_OBJ;
	}
	return NO_EVENT;
}

void CHitBox::Release()
{
}

CHitBox * CHitBox::Create(VEC3 vPos, VEC3 vSize, VEC3 vDir, int iAtt, HITBOX::TYPE eType)
{
	CHitBox*	pInstance = new CHitBox;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vSize = vSize;
	pInstance->m_tInfo.vDir = vDir;
	pInstance->m_tInfo.iAtt = iAtt;
	pInstance->m_eHitBoxType = eType;
	pInstance->UpdateRect();

	return pInstance;
}
