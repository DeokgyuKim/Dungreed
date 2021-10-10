#include "stdafx.h"
#include "Bansh.h"
#include "BanshState.h"
#include "BanshBullet.h"
#include "PlayerObserver.h"
#include "SoundMgr.h"

CBansh::CBansh()
{
}


CBansh::~CBansh()
{
	Release();
}

int CBansh::Update()
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

void CBansh::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

HRESULT CBansh::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::RANGED_MONSTER;
	m_eRangedMonType = RANGED_MONSTER::BANSH;

	m_tInfo.fSpeed = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 20.f, 18.f, 0.f };

	m_tInfo.iAtt = 5;
	m_tInfo.iHp = 50;
	m_iFullHp = m_tInfo.iHp;

	Set_Frame(L"Bansh", L"BanshIdle", 0.f, 6.f);

	m_tInfo.bDead = false;

	m_pState = new StateMachine<CBansh>(this);
	m_pState->SetCurrentState(CBanshIdle::Instance());

	return S_OK;
}

void CBansh::Release()
{
	SafeDelete<StateMachine<CBansh>*>(m_pState);
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos - VEC3(0.f, 11.f, 0.f), 1.0f, L"Money", L"Bullion",
		7, 0.f, RENDER::MID);
	CInventory::GetInstance()->PlusGold(100);
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyDropMoney.wav", CSoundMgr::CHANNELID::MONSTER);
}

void CBansh::UpdateBoxs()
{
	m_tAttRangeBox = { (LONG)m_tInfo.vPos.x - TILECX * 5,
		(LONG)m_tInfo.vPos.y - TILECY * 5,
		(LONG)m_tInfo.vPos.x + TILECX * 5,
		(LONG)m_tInfo.vPos.y + TILECY * 5 };
	m_tSearchingBox = { (LONG)m_tInfo.vPos.x - TILECX * 10,
		(LONG)m_tInfo.vPos.y - TILECY * 10,
		(LONG)m_tInfo.vPos.x + TILECX * 10,
		(LONG)m_tInfo.vPos.y + TILECY * 10 };
}

void CBansh::CreateBullet()
{
	VEC3 vPos = { 20.f, 0.f, 0.f };
	MATRIX matRotate;
	for (int i = 0; i < 360; i += 18)
	{
		MATROTATIONZ(&matRotate, TORADIAN(i));
		VEC3 vTemp, vDir;
		VEC3_T_COORD(&vTemp, &vPos, &matRotate);
		VEC3NORMAL(&vDir, &vTemp);
		CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, (CBanshBullet::Create(m_tInfo.vPos + VEC3(0.f, -10.f, 0.f), vDir, m_tInfo.iAtt, HITBOX::MONSTER)));
		CSoundMgr::Get_Instance()->PlaySoundW(L"MyBanshScream.wav", CSoundMgr::CHANNELID::MONSTER);
	}
}

CBansh * CBansh::Create(VEC3 vPos)
{
	CBansh* pInstance = new CBansh;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	return pInstance;
}

