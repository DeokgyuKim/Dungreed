#include "stdafx.h"
#include "MyEffect.h"


CMyEffect::CMyEffect()
{
}


CMyEffect::~CMyEffect()
{
	Release();
}

int CMyEffect::Update()
{
	if (m_fLifeTime >= m_fDeadTime)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
		return DEAD_OBJ;

	m_fLifeTime += GET_TIME;



	return NO_EVENT;
}

void CMyEffect::LateUpdate()
{
	if (m_tFrame.bReverse)
		m_tFrame.fStart -= m_tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime() / m_fLateCount;
	else
		m_tFrame.fStart += m_tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime() / m_fLateCount;

	if (m_tFrame.fStart > m_tFrame.fCount)
		m_tFrame.fStart = 0.f;
	if (m_tFrame.fStart < 0)
		m_tFrame.fStart = m_tFrame.fCount - 0.00001f;
}

void CMyEffect::Render()
{
	MATRIX matWorld;
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matWorld, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);

	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
}

HRESULT CMyEffect::Initialize()
{
	m_eType = OBJ::OBJ_EFFECT;
	m_tInfo.bDead = false;
	m_fLifeTime = 0.f;
	m_fLateCount = 1.f;
	return S_OK;
}

void CMyEffect::Release()
{
}

CMyEffect * CMyEffect::Create(VEC3 vPos, float fLifeTime, TCHAR * wstrObjectKey, TCHAR * wstrStateKey, float fCount)
{
	CMyEffect*	pInstance = new CMyEffect;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	pInstance->m_fLifeTime = 0.f;
	pInstance->m_fDeadTime = fLifeTime;
	pInstance->Set_Frame(wstrObjectKey, wstrStateKey, 0.f, fCount);
	pInstance->m_fLateCount = fLifeTime;

	return pInstance;
}

