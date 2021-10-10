#include "stdafx.h"
#include "NoAnimateIMP.h"


CNoAnimateIMP::CNoAnimateIMP()
	: m_fLifeTime(0.f), m_fTimeCount(0.f)
{
}


CNoAnimateIMP::~CNoAnimateIMP()
{
	Release();
}

int CNoAnimateIMP::Update()
{
	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CNoAnimateIMP::LateUpdate()
{
}

void CNoAnimateIMP::Render(const D3DXMATRIX & matWorld)
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, 3);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CNoAnimateIMP::Initialize()
{
	return S_OK;
}

void CNoAnimateIMP::Release()
{
}

CNoAnimateIMP* CNoAnimateIMP::Create(
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	const float& fLifeTime)
{
	CNoAnimateIMP* pInstance = new CNoAnimateIMP;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;
	pInstance->m_fLifeTime = fLifeTime;

	return pInstance;
}
