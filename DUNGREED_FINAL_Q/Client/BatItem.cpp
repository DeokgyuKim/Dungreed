#include "stdafx.h"
#include "BatItem.h"
#include "PlayerObserver.h"
#include "RevolverBullet.h"

#include "SoundMgr.h"

CBatItem::CBatItem()
{
}


CBatItem::~CBatItem()
{
}

int CBatItem::Update()
{
	if (!m_bCreate)
	{
		m_tInfo.vPos = GET_PLAYER_OB->GetPlayerPos() + VEC3(-15, -15, 0.f);
		m_bCreate = true;
	}
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	
	if (VEC3LENGTH(&(m_tInfo.vPos - vPos)) >= 40)
	{
		//이동
		m_tInfo.vDir = vPos - m_tInfo.vPos;
		VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);

		m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
	}
	else
	{
		m_fTimeCount += GET_TIME;
		if (m_fTimeCount >= 1.f)
		{
			//공격
			CObj* pTarget = CObjectMgr::GetInstance()->GetTarget(m_tInfo.vPos);
			if (pTarget != nullptr)
			{
				m_tInfo.vDir = pTarget->Get_Info()->vPos - m_tInfo.vPos;
				VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);

				CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CRevolverBullet::Create(m_tInfo.vPos, m_tInfo.vDir, m_tInfo.iAtt, HITBOX::PLAYER));
				CSoundMgr::Get_Instance()->PlaySoundW(L"MyBatAtt.wav", CSoundMgr::CHANNELID::PLAYER);
				m_fTimeCount = 0.f;
			}
		}
	}
	


	return NO_EVENT;
}

void CBatItem::LateUpdate()
{
	CObj::UpdateRect();
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CBatItem::Render()
{
	//matWorld
	MATRIX	matScale, matTrans, matWorld;
	if(m_tInfo.vDir.x < 0)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale *  matTrans;

	//텍스쳐 렌더
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
}

HRESULT CBatItem::Initialize()
{
	m_eType = OBJ::SUBITEM;
	m_eSubItemType = SUBITEM::BAT;
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	m_tInfo.vSize = { 32.f, 32.f, 0.f };
	m_tInfo.fSpeed = 150.f;
	m_tInfo.iAtt = 10;
	m_tInfo.vPos = GET_PLAYER_OB->GetPlayerPos() + VEC3(-15, -15, 0.f);
	m_fTimeCount = 1.f;
	m_bCreate = false;
	m_tInfo.m_iGold = 1500;

	Set_Frame(L"BatItem", L"BatMove", 0.f, 6.f);
	return S_OK;
}

void CBatItem::Release()
{
}

CBatItem * CBatItem::Create()
{
	CBatItem* pInstance = new CBatItem;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

