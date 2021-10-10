#include "stdafx.h"
#include "RangedMonster.h"
#include "PlayerObserver.h"

CRangedMonster::CRangedMonster()
{
}


CRangedMonster::~CRangedMonster()
{
}

bool CRangedMonster::Attack()
{
	UpdateBoxs();
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();
	POINT pt = { (LONG)vPlayerPos.x, (LONG)vPlayerPos.y - TILECY / 2 };
	return (bool)(PtInRect(&m_tAttRangeBox, pt));
}

bool CRangedMonster::Searching()
{
	UpdateBoxs();
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();
	POINT pt = { (LONG)vPlayerPos.x, (LONG)vPlayerPos.y - TILECY / 2 };
	return (bool)(PtInRect(&m_tSearchingBox, pt));
}

void CRangedMonster::Following()
{
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();
	m_tInfo.vDir = vPlayerPos - m_tInfo.vPos;
	VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
}