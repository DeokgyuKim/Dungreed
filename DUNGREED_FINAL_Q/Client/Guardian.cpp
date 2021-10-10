#include "stdafx.h"
#include "Guardian.h"
#include "GuardianState.h"
#include "PlayerObserver.h"

#include "SoundMgr.h"

CGuardian::CGuardian()
{
}


CGuardian::~CGuardian()
{
	Release();
}

HRESULT CGuardian::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::MELEE_MONSTER;
	m_eMeleeMonType = MELEE_MONSTER::GUARDIAN;

	m_tInfo.fSpeed = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 20.f, 18.f, 0.f };

	m_tInfo.iAtt = 10;
	m_tInfo.iHp = 100;
	m_iFullHp = m_tInfo.iHp;

	Set_Frame(L"Guardian", L"GuardianIdle", 0.f, 4.f);

	m_tInfo.bDead = false;

	m_pState = new StateMachine<CGuardian>(this);
	m_pState->SetCurrentState(CGuardianIdle::Instance());

	return S_OK;
}

int CGuardian::Update()
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

void CGuardian::LateUpdate()
{
	//ÆÈ·Î¿ì »óÅÂ°¡ ¾Æ´Ò¶§¸¸
	CObj::UpdateRect();
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CGuardian::Release()
{
	SafeDelete<StateMachine<CGuardian>*>(m_pState);
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos - VEC3(0.f, 35.f, 0.f), 0.4f, L"Monster", L"MonsterDead",
		12, 0.f, RENDER::MID);
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos - VEC3(0.f, 35.f, 0.f), 1.0f, L"Money", L"Bullion",
		7, 0.f, RENDER::MID);
	CInventory::GetInstance()->PlusGold(100);
	CSoundMgr::Get_Instance()->PlaySoundW(L"MyDropMoney.wav", CSoundMgr::CHANNELID::MONSTER);
}

CGuardian * CGuardian::Create(VEC3 vPos)
{
	CGuardian* pInsatnce = new CGuardian;
	if (FAILED(pInsatnce->Initialize()))
	{
		delete pInsatnce;
		return nullptr;
	}
	pInsatnce->m_tInfo.vPos = vPos;
	return pInsatnce;
}

VEC3 CGuardian::Get_PlayerPos()
{
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();

	int iCullX = (int)vPos.x / TILECX;
	int iCullY = (int)vPos.y / TILECY;

	while (true)
	{
		int iIndex = TILE_COUNT_X * iCullY + iCullX;
		if (iIndex < 0)
		{
			iCullY += 1;
			continue;
		}
		if (iIndex >= TILE_COUNT_X * TILE_COUNT_Y)
			return VEC3{ -1.f, -1.f, -1.f };
		if (CTileMgr::GetInstance()->Get_Tile()[iIndex]->byOption == 1)
		{
			vPos.y = CTileMgr::GetInstance()->Get_Tile()[TILE_COUNT_X * (iCullY - 1) + iCullX]->vPos.y;
			break;
		}
		iCullY += 1;
	}

	if (m_tInfo.vPos.x < vPos.x)
		vPos.x -= TILECX * 0.75f;
	else
		vPos.x += TILECX * 0.75f;
	return vPos;
}

void CGuardian::UpdateBoxs()
{
	m_tAttRangeBox = { (LONG)m_tInfo.vPos.x - TILECX * 2,
		(LONG)m_tInfo.vPos.y - 20,
		(LONG)m_tInfo.vPos.x + TILECX * 2,
		(LONG)m_tInfo.vPos.y + TILECY };
	m_tSearchingBox = { (LONG)m_tInfo.vPos.x - TILECX * 10,
		(LONG)m_tInfo.vPos.y - TILECY * 5,
		(LONG)m_tInfo.vPos.x + TILECX * 10,
		(LONG)m_tInfo.vPos.y + TILECY * 5 };
}
