#include "stdafx.h"
#include "Shop.h"


CShop::CShop()
{
}


CShop::~CShop()
{
	Release();
}

int CShop::Update()
{
	return NO_EVENT;
}

void CShop::LateUpdate()
{
	CObj::UpdateRect();
	this->FrameUpdate();
}

void CShop::Render()
{
	//matWorld
	MATRIX	matScale, matRotateZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotateZ, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matRotateZ * matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));

	D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotateZ, 0.f);
	vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matRotateZ * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrameNpc.pObjectKey, &matWorld, m_tFrameNpc.pStateKey, int(m_tFrameNpc.fStart));
	CLineMgr::DrawBox(m_tRect);
}

HRESULT CShop::Initialize()
{
	m_tInfo.vSize = { 100.f, 100.f, 0.f };
	m_tInfo.bDead = false;
	m_eType = OBJ::SHOP;
	
	Set_Frame(L"Shop", L"ShopNormal", 0.f, 1.f);
	m_tFrameNpc.pObjectKey = L"Shop";
	m_tFrameNpc.pStateKey = L"ShopNPC";
	m_tFrameNpc.fStart = 0.f;
	m_tFrameNpc.fCount = 4.f;
	m_tFrameNpc.bReverse = false;

	return S_OK;
}

void CShop::Release()
{
}

CShop * CShop::Create(VEC3 vPos)
{
	CShop* pInstance = new CShop;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}

	pInstance->m_tInfo.vPos = vPos;
	return pInstance;
}

void CShop::FrameUpdate()
{
	if (m_tFrameNpc.bReverse)
		m_tFrameNpc.fStart -= m_tFrameNpc.fCount * CTimeMgr::GetInstance()->GetDeltaTime();
	else
		m_tFrameNpc.fStart += m_tFrameNpc.fCount * CTimeMgr::GetInstance()->GetDeltaTime();

	if (m_tFrameNpc.fStart > m_tFrameNpc.fCount)
		m_tFrameNpc.fStart = 0.f;
	if (m_tFrameNpc.fStart < 0)
		m_tFrameNpc.fStart = m_tFrameNpc.fCount - 0.00001f;

}
