#include "stdafx.h"
#include "AnimateIMP.h"


CAnimateIMP::CAnimateIMP()
	: m_fFrameStart(0.f),
	m_fMaxFrameCount(0.f)
{
}


CAnimateIMP::~CAnimateIMP()
{
	Release();
}

int CAnimateIMP::Update()
{
	m_fFrameStart += m_fMaxFrameCount * m_pTimeMgr->GetDeltaTime();

	if (m_fFrameStart >= m_fMaxFrameCount)
		return DEAD_OBJ;

	return NO_EVENT;
}

void CAnimateIMP::LateUpdate()
{
}

void CAnimateIMP::Render(const D3DXMATRIX& matWorld)
{	
	m_pDeviceMgr->GetSprite()->End(); // ������� �׷����� D3DRS_DESTBLEND
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND); // ���⼭���� D3DRS_SRCBLEND

	// D3DBLENDOP_ADD: ������ �̿��� ȥ�չ��.
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

	// D3DRS_DESTBLEND: ���� �ĸ���ۿ� �׷����ִ� �ȼ�
	// D3DBLEND_INVSRCCOLOR: D3DBLEND_SRCCOLOR�� ���� ��.
	// (0, 255, 0, 255) * (1, 1, 1, 0) = (0, 255, 0, 0)
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	// D3DRS_SRCBLEND: ���� �׷����� �ȼ�
	// D3DBLEND_SRCCOLOR(ȥ�պ���): Src�ȼ��� ���� �ִ� RGBA ���� 0 ~ 1 ������ ȯ���� ���� ��.
	// (0, 0, 0, 255) * (0, 0, 0, 1) = (0, 0, 0, 255)
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);

	// �����ȼ� = (Dest�ȼ� * ȥ�պ���) + (Src�ȼ� * ȥ�պ���)
	// �����ȼ� = (D3DRS_DESTBLEND * D3DBLEND_INVSRCCOLOR) + (D3DRS_SRCBLEND * D3DBLEND_SRCCOLOR)

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjectKey, m_wstrStateKey, (int)m_fFrameStart);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDeviceMgr->GetSprite()->End(); // ��������� ���ĺ��� �����ض�!
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

HRESULT CAnimateIMP::Initialize()
{
	return S_OK;
}

void CAnimateIMP::Release()
{
}

CAnimateIMP* CAnimateIMP::Create(
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	const float& fFrameStart, 
	const float& fMaxFrameCount)
{
	CAnimateIMP* pInstance = new CAnimateIMP;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_wstrObjectKey = wstrObjectKey;
	pInstance->m_wstrStateKey = wstrStateKey;
	pInstance->m_fFrameStart = fFrameStart;
	pInstance->m_fMaxFrameCount = fMaxFrameCount;

	return pInstance;
}
