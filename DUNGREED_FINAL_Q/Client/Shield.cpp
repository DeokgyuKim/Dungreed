#include "stdafx.h"
#include "Shield.h"
#include "PlayerObserver.h"

CShield::CShield()
{
}


CShield::~CShield()
{
}

int CShield::Update()
{
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	bool bRenderRight = GET_PLAYER_OB->GetPlayerRenderRight();

	if (!bRenderRight)
		m_tInfo.vPos = vPos + VEC3(6.f, -4.f, 0.f);
	else
		m_tInfo.vPos = vPos - VEC3(6.f, 4.f, 0.f);


	return NO_EVENT;
}

void CShield::LateUpdate()
{
	CObj::SaveOldInfo();
}

void CShield::Render()
{
	//matWorld
	MATRIX	matScale,  matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale *  matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
}

HRESULT CShield::Initialize()
{
	m_eType = OBJ::SUBITEM;
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	m_tInfo.vSize = { 9.f, 9.f, 0.f };
	m_eSubItemType = SUBITEM::SHIELD;
	m_tInfo.m_iGold = 100;
	
	Set_Frame(L"MagicShield", L"Idle", 0.f, 1.f);
	return S_OK;
}

void CShield::Release()
{
}

CShield * CShield::Create()
{
	CShield* pInstance = new CShield;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

