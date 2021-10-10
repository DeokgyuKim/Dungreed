#include "stdafx.h"
#include "NormalEffect.h"
#include "EffectIMP.h"
#include "ScrollMgr.h"

CNormalEffect::CNormalEffect()
{
}


CNormalEffect::~CNormalEffect()
{
	Release();
}

int CNormalEffect::Update()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::Get_Scroll().x,
		m_tInfo.vPos.y - CScrollMgr::Get_Scroll().y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	return m_pImp->Update();
}

void CNormalEffect::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CNormalEffect::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CNormalEffect::Initialize()
{
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	return S_OK;
}

void CNormalEffect::Release()
{
}

CNormalEffect* CNormalEffect::Create(const D3DXVECTOR3& vPos, CEffectIMP* pImp)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CNormalEffect* pInstance = new CNormalEffect;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_pImp = pImp;
	pInstance->m_tInfo.vPos = vPos;

	return pInstance;
}
