#include "stdafx.h"
#include "SkullDog.h"
#include "PlayerObserver.h"
#include "StateMachine.h"
#include "SkullDogState.h"

#include "SoundMgr.h"

CSkullDog::CSkullDog()
{
}


CSkullDog::~CSkullDog()
{
	Release();
}

HRESULT CSkullDog::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::MELEE_MONSTER;
	m_eMeleeMonType = MELEE_MONSTER::SKULLDOG;

	m_tInfo.fSpeed = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = {0.f, 0.f, 0.f};
	m_tInfo.vSize = { 20.f, 18.f, 0.f };

	m_tInfo.iAtt = 10;
	m_tInfo.iHp = 50;
	m_iFullHp = m_tInfo.iHp;

	Set_Frame(L"SkullDog", L"SkullDogIdle", 0.f, 5.f);

	m_tInfo.bDead = false;

	m_pState = new StateMachine<CSkullDog>(this);
	m_pState->SetCurrentState(CSkullDogIdle::Instance());

	return S_OK;
}

int CSkullDog::Update()
{
	if (m_tInfo.iHp <= 0)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	UpdateBoxs();
	m_pState->Update();
	Set_Look(Searching());
	return NO_EVENT;
}

void CSkullDog::LateUpdate()
{
	CObj::UpdateRect();
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CSkullDog::UpdateBoxs()
{
	m_tAttRangeBox = { (LONG)m_tInfo.vPos.x - TILECX * 3,
						(LONG)m_tInfo.vPos.y - 20,
						(LONG)m_tInfo.vPos.x + TILECX * 3,
						(LONG)m_tInfo.vPos.y + TILECY };
	m_tSearchingBox = { (LONG)m_tInfo.vPos.x - TILECX * 10,
						(LONG)m_tInfo.vPos.y - (LONG)(TILECY * 1.5f),
						(LONG)m_tInfo.vPos.x + TILECX * 10,
						(LONG)m_tInfo.vPos.y + TILECY };
}

void CSkullDog::Release()
{
	SafeDelete<StateMachine<CSkullDog>*>(m_pState); 
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos - VEC3(0.f, 9.f, 0.f), 1.0f, L"Money", L"Bullion",
		7, 0.f, RENDER::MID);
	CInventory::GetInstance()->PlusGold(100);
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyDropMoney.wav", CSoundMgr::CHANNELID::MONSTER);
}

VEC3 CSkullDog::FindDir()
{
	VEC3 vPlayerPos = GET_PLAYER_OB->GetPlayerPos();

	if (fabs(vPlayerPos.x - m_tInfo.vPos.x) < TILECY * 0.5f)
		return VEC3(0.f, 0.f, 0.f);
	
	VEC3 vDir = { vPlayerPos.x - m_tInfo.vPos.x, 0.f, 0.f };
	VEC3NORMAL(&vDir, &vDir);
	return vDir;
}

CSkullDog * CSkullDog::Create(VEC3 vPos)
{
	CSkullDog* pInstance = new CSkullDog;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	return pInstance;
}
