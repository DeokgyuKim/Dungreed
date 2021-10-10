#include "stdafx.h"
#include "MeleeMonster.h"
#include "PlayerObserver.h"

CMeleeMonster::CMeleeMonster()
{
}


CMeleeMonster::~CMeleeMonster()
{
}

bool CMeleeMonster::Attack()
{
	UpdateBoxs();
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();
	POINT pt = { (LONG)vPlayerPos.x, (LONG)vPlayerPos.y - TILECY / 2 };
	return (bool)(PtInRect(&m_tAttRangeBox, pt));
}

bool CMeleeMonster::Searching()
{
	UpdateBoxs();
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();
	POINT pt = { (LONG)vPlayerPos.x, (LONG)vPlayerPos.y - TILECY / 2 };
	return (bool)(PtInRect(&m_tSearchingBox, pt));
}

void CMeleeMonster::Following()
{
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();
	m_tInfo.vDir = { vPlayerPos.x - m_tInfo.vPos.x, 0.f, 0.f };
	VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);

	m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
}
