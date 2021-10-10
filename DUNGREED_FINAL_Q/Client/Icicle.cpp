#include "stdafx.h"
#include "Icicle.h"
#include "SoundMgr.h"

CIcicle::CIcicle()
{
}


CIcicle::~CIcicle()
{
}

int CIcicle::Update()
{
	m_fTimeCount += GET_TIME; 
	if (m_tInfo.bDead)
	{
		if (m_fTimeCount > 1.0f)
			return DEAD_OBJ;
		return NO_EVENT;
	}
	if (m_fTimeCount >= 1.0f && !m_bCreate)
	{
		m_bCreate = true;
		Set_Frame(L"Icicle", L"IcicleIdle", 0.f, 1.f);
	}
	else if (m_fTimeCount < 5.f && m_fTimeCount > 1.0f)
	{
		CBullet::Update();
	}
	else if (!m_tInfo.bDead && m_fTimeCount >= 5.f)
	{
		m_tInfo.bDead = true;
		Set_Frame(L"Icicle", L"IcicleDestroy", 0.f, 3.f);
	}
	CObj::UpdateRect();
	return NO_EVENT;
}

void CIcicle::LateUpdate()
{
	CObj::SaveOldInfo();
}

void CIcicle::Render()
{
	MATRIX matWorld;
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matWorld, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));

	if (m_fTimeCount < 1.f)
	{
		CDeviceMgr::GetInstance()->GetLine()->SetWidth(25.f * m_fTimeCount);
		CDeviceMgr::GetInstance()->GetSprite()->End();
		CDeviceMgr::GetInstance()->GetLine()->Begin();
		D3DXVECTOR2 vTemp[2] = {};
		vTemp[0] = { m_tInfo.vPos.x - CScrollMgr::Get_Scroll().x, -500.f };
		vTemp[1] = { m_tInfo.vPos.x - CScrollMgr::Get_Scroll().x, TILECY * TILE_COUNT_Y + 500.f };

		for (int i = 0; i < 2; ++i)
			vTemp[i] *= 4.f;

		HRESULT hr = CDeviceMgr::GetInstance()->GetLine()->Draw(vTemp, 2, D3DCOLOR_XRGB(255, 0, 0));

		CDeviceMgr::GetInstance()->GetLine()->End();
		CDeviceMgr::GetInstance()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}
	CLineMgr::DrawBox(m_tRect);
}

HRESULT CIcicle::Initialize()
{
	m_eType = OBJ::HITBOX;

	m_tInfo.fSpeed = 400;
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 27.f, 40.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_tInfo.iAtt = 10;
	m_tInfo.iHp = 0;

	Set_Frame(L"Icicle", L"IcicleCreate", 0.f, 9.f);

	m_tInfo.bDead = false;
	m_bCreate = false;

	m_fTimeCount = 0.f;
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyIceSpear.wav", CSoundMgr::CHANNELID::MONSTER);
	return S_OK;
}

void CIcicle::Release()
{
}

CIcicle * CIcicle::Create(VEC3 vPos, int iAtt, HITBOX::TYPE eType)
{
	CIcicle* pInstance = new CIcicle;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_tInfo.vPos.y = 0.f;
	pInstance->m_eHitBoxType = eType;
	return pInstance;
}

void CIcicle::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
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
