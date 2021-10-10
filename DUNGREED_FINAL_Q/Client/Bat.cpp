#include "stdafx.h"
#include "Bat.h"
#include "BatBullet.h"
#include "PlayerObserver.h"
#include "BatState.h"
#include "SoundMgr.h"

CBat::CBat()
{
}


CBat::~CBat()
{
	Release();
}

HRESULT CBat::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::RANGED_MONSTER;
	m_eRangedMonType = RANGED_MONSTER::BAT;

	m_tInfo.fSpeed = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 30.f, 30.f, 0.f };

	m_tInfo.iAtt = 5;
	m_tInfo.iHp = 50;
	m_iFullHp = m_tInfo.iHp;

	Set_Frame(L"Bat", L"BatIdle", 0.f, 7.f);

	m_tInfo.bDead = false;

	m_pState = new StateMachine<CBat>(this);
	m_pState->SetCurrentState(CBatIdle::Instance());

	return S_OK;
}

int CBat::Update()
{
	if (m_tInfo.iHp <= 0)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	m_pState->Update();
	UpdateBoxs();
	CObj::UpdateRect();
	Set_Look(Searching());
	if (!m_pBullet.empty())
	{
		for (auto Bullet : m_pBullet)
			Bullet->Update();
		if (m_pBullet.front()->Get_Time() > 0.5f)
		{
			for(auto Bullet : m_pBullet)
				CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, Bullet);
			m_pBullet.clear();
		}
	}
	return NO_EVENT;
}

void CBat::LateUpdate()
{
	if (!m_pBullet.empty())
	{
		for (auto Bullet : m_pBullet)
			Bullet->LateUpdate();
	}
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CBat::Render()
{
	if (m_tInfo.bDead)
		return;
	CMonster::Render();
	if (!m_pBullet.empty())
	{
		for (auto Bullet : m_pBullet)
			Bullet->Render();
	}
}

void CBat::Release()
{
	SafeDelete<StateMachine<CBat>*>(m_pState);
	if (!m_pBullet.empty())
	{
		for (auto Bullet : m_pBullet)
			SafeDelete<CBatBullet*>(Bullet);
		m_pBullet.clear();
	}
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos - VEC3(0.f, 32.f, 0.f), 0.4f, L"Monster", L"MonsterDead",
		12, 0.f, RENDER::MID);
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos - VEC3(0.f, 32.f, 0.f), 1.0f, L"Money", L"Bullion",
		7, 0.f, RENDER::MID);
	CInventory::GetInstance()->PlusGold(100);
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyDropMoney.wav", CSoundMgr::CHANNELID::MONSTER);
}

void CBat::UpdateBoxs()
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

void CBat::CreateBullet()
{
	VEC3 vDir = GET_PLAYER_OB->GetPlayerPos() - (m_tInfo.vPos + VEC3(0.f, -20.f, 0.f));
	VEC3NORMAL(&vDir, &vDir);
	VEC3 vPos = {20.f, 0.f, 0.f};
	MATRIX matRotate;
	for (int i = 0; i < 360; i += 18)
	{
		MATROTATIONZ(&matRotate, TORADIAN(i));
		VEC3 vTemp;
		VEC3_T_COORD(&vTemp, &vPos, &matRotate);
		m_pBullet.emplace_back(CBatBullet::Create(m_tInfo.vPos + VEC3(0.f, -20.f, 0.f) + vTemp, vDir, m_tInfo.iAtt, HITBOX::MONSTER));
	}
}

CBat * CBat::Create(VEC3 vPos)
{
	CBat* pInstance = new CBat;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	return pInstance;
}
