#include "stdafx.h"
#include "Box.h"

#include "Shield.h"
#include "TimeRing.h"
#include "Wing.h"
#include "SpeedRing.h"
#include "Stamp.h"
#include "Dart.h"
#include "FireSword.h"

#include "SoundMgr.h"

CBox::CBox()
{
}


CBox::~CBox()
{
	Release();
}

int CBox::Update()
{
	if (m_tInfo.iHp <= 0)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	return NO_EVENT;
}

void CBox::LateUpdate()
{
	CObj::UpdateRect();
}

void CBox::Render()
{
	//matWorld
	MATRIX	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale *  matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
}

HRESULT CBox::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::BOX;

	m_tInfo.fSpeed = 1;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 29.f, 18.f, 0.f };

	m_tInfo.iAtt = 0;
	m_tInfo.iHp = 50;

	Set_Frame(L"NormalTreasure", L"StashClosed", 0.f, 1.f);

	m_tInfo.bDead = false;

	return S_OK;
}

void CBox::Release()
{

	CSoundMgr::Get_Instance()->PlaySoundW(L"MyChest.wav", CSoundMgr::CHANNELID::UI);
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos, 1.f, L"NormalTreasure", L"StashOpened", 1.f, 0.f, RENDER::BOTTOM);
	switch (m_eBoxType)
	{
	case BOX::SHIELD:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"MagicShield", L"Idle", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CShield::Create());
		break;
	case BOX::TIMERING:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"Ring", L"TimeRing", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CTimeRing::Create());
		break;
	case BOX::WING:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"Item", L"Wing", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CWing::Create());
		break;
	case BOX::SPEED:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"Ring", L"SpeedRing", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CSpeedRing::Create());
		break;
	case BOX::STAMP:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"Stamp", L"Stamp", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CStamp::Create());
		break;
	case BOX::DART:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"Bomerang", L"BomerangIdle", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CDart::Create());
		break;
	case BOX::FIRESWORD:
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos + VEC3(0.f, -20.f, 0.f), 1.f, L"FireSword", L"FireDragonKillerIdle", 1.f, 0.f, RENDER::BOTTOM);
		if (CInventory::GetInstance()->GetIndex() != -1)
			CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), CFireSword::Create());
		break;
	default:
		break;
	}
	
}

CBox * CBox::Create(VEC3 vPos, BOX::TYPE eType)
{
	CBox* pInstance = new CBox;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_eBoxType = eType;
	return pInstance;
}

bool CBox::Attack()
{
	return false;
}

bool CBox::Searching()
{
	return false;
}

void CBox::Following()
{
}

