#include "stdafx.h"
#include "EffectIMP.h"


CEffectIMP::CEffectIMP()
	: m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_wstrObjectKey(L""),
	m_wstrStateKey(L"")
{
}


CEffectIMP::~CEffectIMP()
{
}
