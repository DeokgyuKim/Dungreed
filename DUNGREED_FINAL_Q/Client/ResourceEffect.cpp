#include "stdafx.h"
#include "ResourceEffect.h"


CResourceEffect::CResourceEffect()
{
}


CResourceEffect::~CResourceEffect()
{
	Release();
}

int CResourceEffect::Update()
{
	FrameUpdate();
	if (CMyEffect::Update() == DEAD_OBJ)
		return DEAD_OBJ;
	return NO_EVENT;
}

void CResourceEffect::Render()
{
	MATRIX matRotate, matTrans, matWorld;

	MATROTATIONZ(&matRotate, TORADIAN(m_fRotate));

	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);


	matWorld = matRotate * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, m_eRenderPos);
}

HRESULT CResourceEffect::Initialize()
{
	CMyEffect::Initialize();
	m_eEffType = EFFECTMGR::RESOURCE_EFFECT;
	m_fLateCount = 1.f;
	return S_OK;
}

void CResourceEffect::Release()
{
}

CResourceEffect * CResourceEffect::Create(VEC3 vPos, float fLifeTime, TCHAR * wstrObjectKey, TCHAR * wstrStateKey,
	float fCount, float fRotate, RENDER::POS ePos)
{
	CResourceEffect* pInstance = new CResourceEffect;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	pInstance->m_tInfo.vPos = vPos;
	pInstance->Set_Frame(wstrObjectKey, wstrStateKey, 0.f, fCount);
	pInstance->m_fDeadTime = fLifeTime;
	pInstance->m_fRotate = fRotate;
	pInstance->m_eRenderPos = ePos;

	return pInstance;
}
