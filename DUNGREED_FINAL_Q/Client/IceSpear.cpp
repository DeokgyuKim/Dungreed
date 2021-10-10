#include "stdafx.h"
#include "IceSpear.h"

#include "SoundMgr.h"

CIceSpear::CIceSpear()
{
}


CIceSpear::~CIceSpear()
{
}

int CIceSpear::Update()
{
	m_fTimeCount += GET_TIME;
	if (m_tInfo.vPos.x > TILECX * TILE_COUNT_X || m_tInfo.vPos.x < 0)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	if (m_fTimeCount < 1.f)
	{

	}
	else if (m_fTimeCount >= 1.0f && !m_bCreate)
	{
		m_bCreate = true;
		Set_Frame(L"IceSpear", L"Ilde", 0.f, 1.f);
	}
	else
	{
		CBullet::Update();
		//cout << m_tInfo.vPos.x << endl;
	}
	CObj::UpdateRect();
	return NO_EVENT;
}

void CIceSpear::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CIceSpear::Render()
{
	MATRIX	matScale, matTrans, matWorld;
	if(m_bRight)
		MATSCAILING(&matScale, 1.f, 1.f, 1.f);
	else
		MATSCAILING(&matScale, -1.f, 1.f, 1.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);

	matWorld = matScale * matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));

	if (m_fTimeCount < 1.f)
	{
		CDeviceMgr::GetInstance()->GetLine()->SetWidth(25.f * m_fTimeCount);
		CDeviceMgr::GetInstance()->GetSprite()->End();
		CDeviceMgr::GetInstance()->GetLine()->Begin();
		D3DXVECTOR2 vTemp[2] = {};
		vTemp[0] = { -500.f - CScrollMgr::Get_Scroll().x, m_tInfo.vPos.y - CScrollMgr::Get_Scroll().y - m_tInfo.vSize.y * 0.5f };
		vTemp[1] = { TILECX * TILE_COUNT_X + 500.f - CScrollMgr::Get_Scroll().x, m_tInfo.vPos.y - CScrollMgr::Get_Scroll().y - m_tInfo.vSize.y * 0.5f };

		for (int i = 0; i < 2; ++i)
			vTemp[i] *= 4.f;

		HRESULT hr = CDeviceMgr::GetInstance()->GetLine()->Draw(vTemp, 2, D3DCOLOR_XRGB(255, 0, 0));

		CDeviceMgr::GetInstance()->GetLine()->End();
		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}
}

HRESULT CIceSpear::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 800;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 111.f, 25.f, 0.f };

	m_tInfo.iAtt = 20;
	m_tInfo.iHp = 0;

	Set_Frame(L"IceSpear", L"Create", 0.f, 13.f);

	m_tInfo.bDead = false;
	m_bCreate = false;

	m_fTimeCount = 0.f;
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyIceSpear.wav", CSoundMgr::CHANNELID::MONSTER);
	return S_OK;
}

void CIceSpear::Release()
{
}

CIceSpear * CIceSpear::Create(VEC3 vPos, int iAtt, bool bRight, HITBOX::TYPE eType)
{
	CIceSpear* pInstance = new CIceSpear;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_bRight = bRight;
	pInstance->m_tInfo.vPos = vPos;
	if (pInstance->m_bRight)
		pInstance->m_tInfo.vDir = { 1.f, 0.f, 0.f };
	else
		pInstance->m_tInfo.vDir = { -1.f, 0.f, 0.f };
	pInstance->m_eHitBoxType = eType;
	return pInstance;
}

void CIceSpear::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	switch (eType)
	{
	case OBJ::PLAYER:
		m_tInfo.iAtt = 0;
		break;
	default:
		break;
	}
}