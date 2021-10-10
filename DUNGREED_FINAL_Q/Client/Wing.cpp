#include "stdafx.h"
#include "Wing.h"
#include "PlayerObserver.h"

CWing::CWing()
{
}


CWing::~CWing()
{
}

int CWing::Update()
{
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	bool bRenderRight = GET_PLAYER_OB->GetPlayerRenderRight();

	if (!bRenderRight)
		m_tInfo.vPos = vPos + VEC3(11.f, -4.f, 0.f);
	else
		m_tInfo.vPos = vPos - VEC3(11.f, 4.f, 0.f);


	return NO_EVENT;
}

void CWing::LateUpdate()
{
	CObj::SaveOldInfo();
}

void CWing::Render()
{
	//matWorld
	MATRIX	matScale, matTrans, matWorld;
	if(GET_PLAYER_OB->GetPlayerRenderRight())
		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	else
		D3DXMatrixScaling(&matScale, -0.7f, 0.7f, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale *  matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
}

HRESULT CWing::Initialize()
{
	m_eType = OBJ::SUBITEM;
	m_eSubItemType = SUBITEM::WING;
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	m_tInfo.vSize = { 11.f, 16.f, 0.f };
	m_tInfo.m_iGold = 1000;

	Set_Frame(L"Item", L"Wing", 0.f, 1.f);
	return S_OK;
}

void CWing::Release()
{
}

CWing * CWing::Create()
{
	CWing* pInstance = new CWing;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

