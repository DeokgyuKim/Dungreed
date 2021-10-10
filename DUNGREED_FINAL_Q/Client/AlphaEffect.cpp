#include "stdafx.h"
#include "AlphaEffect.h"


CAlphaEffect::CAlphaEffect()
{
}


CAlphaEffect::~CAlphaEffect()
{
	Release();
}

void CAlphaEffect::LateUpdate()
{
	m_dwAlpha -= (int)((200 / m_fDeadTime) * GET_TIME);
}

void CAlphaEffect::Render()
{
	MATRIX matScale, matWorld;
	if(m_bRenderRight)
		MATSCAILING(&matScale, 1.f, 1.f, 0.f);
	else
		MATSCAILING(&matScale, -1.f, 1.f, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matWorld, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matWorld;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), m_dwAlpha);
}

HRESULT CAlphaEffect::Initialize()
{
	CMyEffect::Initialize();
	m_eEffType = EFFECTMGR::ALPHA_EFFECT;
	m_dwAlpha = 200;
	return S_OK;
}

void CAlphaEffect::Release()
{
}

CAlphaEffect * CAlphaEffect::Create(VEC3 vPos, float fLifeTime, TCHAR * wstrObjectKey, TCHAR * wstrStateKey, float fCount, bool bRenderRight)
{
	CAlphaEffect*	pInstance = new CAlphaEffect;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_fLifeTime = 0.f;
	pInstance->m_fDeadTime = fLifeTime;
	pInstance->Set_Frame(wstrObjectKey, wstrStateKey, 0.f, fCount);
	pInstance->m_bRenderRight = bRenderRight;

	return pInstance;
}
