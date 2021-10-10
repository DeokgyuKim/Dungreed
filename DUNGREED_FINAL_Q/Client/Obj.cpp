#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
{
	m_tInfo.bDead = false;
}


CObj::~CObj()
{
}

void CObj::FrameUpdate()
{
	if(m_tFrame.bReverse)
		m_tFrame.fStart -= m_tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime();
	else
		m_tFrame.fStart += m_tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime();

	if (m_tFrame.fStart > m_tFrame.fCount)
		m_tFrame.fStart = 0.f;
	if (m_tFrame.fStart < 0)
		m_tFrame.fStart = m_tFrame.fCount - 0.00001f;
}

void CObj::UpdateRect()
{
	m_tRect.left = LONG(m_tInfo.vPos.x - (int(m_tInfo.vSize.x) >> 1));
	m_tRect.top = LONG(m_tInfo.vPos.y - (int(m_tInfo.vSize.y)));
	m_tRect.right = LONG(m_tInfo.vPos.x + (int(m_tInfo.vSize.x) >> 1));
	m_tRect.bottom = LONG(m_tInfo.vPos.y);
}

bool CObj::CheckDead()
{
	if(m_tInfo.bDead)
		if(m_lstOldInfo.size() <= 0)
			return true;
	return false;
}

void CObj::Render(MATRIX* matWorld, RENDER::POS ePos)
{
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, matWorld, m_tFrame.pStateKey, 0,
		255, ePos);
}

int CObj::ReverseUpdate()
{
	if (m_lstOldInfo.size() > 0)
	{
		m_tInfo = m_lstOldInfo.back().first;
		m_tFrame = m_lstOldInfo.back().second;
		m_lstOldInfo.pop_back();
		if (m_lstOldInfo.size() > 0)
			m_lstOldInfo.pop_back();
		if (m_lstOldInfo.empty())
			m_lstOldInfo.emplace_back(make_pair(m_tInfo, m_tFrame));
	}
	return NO_EVENT;
}

void CObj::ReverseLateUpdate()
{
}
