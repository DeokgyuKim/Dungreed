#include "stdafx.h"
#include "IceMagician.h"
#include "IceMagicianState.h"
#include "IceMagicianBullet.h"
#include "PlayerObserver.h"
#include "SoundMgr.h"

CIceMagician::CIceMagician()
{
}


CIceMagician::~CIceMagician()
{
	Release();
}

int CIceMagician::Update()
{
	if (m_tInfo.iHp <= 0)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_pState->Update();
	UpdateBoxs();
	CObj::UpdateRect();
	Set_Look(Searching());
	return NO_EVENT;
}

void CIceMagician::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CIceMagician::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::RANGED_MONSTER;
	m_eRangedMonType = RANGED_MONSTER::ICEMAGICIAN;

	m_tInfo.fSpeed = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 20.f, 18.f, 0.f };

	m_tInfo.iAtt = 5;
	m_tInfo.iHp = 50;
	m_iFullHp = m_tInfo.iHp;

	Set_Frame(L"IceMagician", L"IceMagicianIdle", 0.f, 6.f);

	m_tInfo.bDead = false;

	m_pState = new StateMachine<CIceMagician>(this);
	m_pState->SetCurrentState(CIceMagicianIdle::Instance());

	return S_OK;
}

void CIceMagician::Release()
{
	SafeDelete<StateMachine<CIceMagician>*>(m_pState);
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos - VEC3(0.f, 15.f, 0.f), 1.0f, L"Money", L"Bullion",
		7, 0.f, RENDER::MID);
	CInventory::GetInstance()->PlusGold(100);
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyDropMoney", CSoundMgr::CHANNELID::MONSTER);
}

void CIceMagician::UpdateBoxs()
{
	m_tAttRangeBox = { (LONG)m_tInfo.vPos.x - TILECX * 8,
		(LONG)m_tInfo.vPos.y - TILECY * 8,
		(LONG)m_tInfo.vPos.x + TILECX * 8,
		(LONG)m_tInfo.vPos.y + TILECY * 8 };
	m_tSearchingBox = { (LONG)m_tInfo.vPos.x - TILECX * 8,
		(LONG)m_tInfo.vPos.y - TILECY * 8,
		(LONG)m_tInfo.vPos.x + TILECX * 8,
		(LONG)m_tInfo.vPos.y + TILECY * 8 };
}

void CIceMagician::CreateBullet()
{
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, (CIceMagicianBullet::Create(vPos, m_tInfo.iAtt, HITBOX::ICEMAGICIAN)));
}

CIceMagician * CIceMagician::Create(VEC3 vPos)
{
	CIceMagician* pInstance = new CIceMagician;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;

	return pInstance;
}
