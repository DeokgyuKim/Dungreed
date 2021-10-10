#include "stdafx.h"
#include "Home.h"

#include "Player.h"
#include "GunMan.h"
#include "RapierMan.h"

#include "Sword.h"
#include "Gun.h"
#include "Spear.h"
#include "Stamp.h"
#include "FireSword.h"

#include "SkullDog.h"
#include "Guardian.h"
#include "Bat.h"
#include "Bansh.h"
#include "IceMagician.h"
#include "Niflheim.h"

#include "Shop.h"

#include "SoundMgr.h"

CHome::CHome()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance()),
	m_pSubject(CSubject::GetInstance())
{
}


CHome::~CHome()
{
	Release();
}

HRESULT CHome::Initiailize()
{

	// Terrain Object
	//hr = m_pObjectMgr->AddObject(TERRAIN, CTerrain::Create());
	//FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);
	CTileMgr::GetInstance()->Initialize(L"../Data/MapData_Home.dat");
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(L"MyTown.wav");
	HRESULT hr;

	m_pObjectMgr->DeleteLst(OBJ_TILE);
	m_pObjectMgr->DeleteLst(OBJ_SHOP);
	m_pObjectMgr->DeleteLst(OBJ_MONSTER);
	m_pObjectMgr->DeleteLst(OBJ_BOSS);
	m_pObjectMgr->DeleteLst(OBJ_ICEPILLAR);
	m_pObjectMgr->DeleteLst(OBJ_PLAYER_HITBOX);
	m_pObjectMgr->DeleteLst(OBJ_MONSTER_HITBOX);
	m_pObjectMgr->DeleteLst(OBJ_EFFECT);

	// Player Object
	if (m_pObjectMgr->GetPlayer() == nullptr)
	{

		hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CGunMan::Create(CTileMgr::GetInstance()->Get_Tile()[1100]->vPos, nullptr));
		hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CRapierMan::Create(CTileMgr::GetInstance()->Get_Tile()[1106]->vPos, nullptr));

		CObj* pPlayer = CPlayer::Create(CTileMgr::GetInstance()->Get_Tile()[1081]->vPos, CSword::Create());
		hr = m_pObjectMgr->AddObject(OBJ_PLAYER, pPlayer);
		FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);
		CInputMgr::GetInstance()->Set_Actor(pPlayer);
		CUiMgr::GetInstance()->SetPlayer(SCAST(CPlayer*, pPlayer));
		SCAST(CPlayer*, pPlayer)->Observer_PlayerType();
	}
	else
	{
		switch (SCAST(CPlayer*, m_pObjectMgr->GetPlayer())->Get_PlayerType())
		{
		case PLAYER::NORMAL_PLAYER:
			hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CGunMan::Create(CTileMgr::GetInstance()->Get_Tile()[1100]->vPos, nullptr));
			hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CRapierMan::Create(CTileMgr::GetInstance()->Get_Tile()[1106]->vPos, nullptr));
			break;
		case PLAYER::GUNMAN_PLAYER:
			hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CPlayer::Create(CTileMgr::GetInstance()->Get_Tile()[1100]->vPos, nullptr));
			hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CRapierMan::Create(CTileMgr::GetInstance()->Get_Tile()[1106]->vPos, nullptr));
			break;
		case PLAYER::RAPIERMAN_PLAYER:
			hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CGunMan::Create(CTileMgr::GetInstance()->Get_Tile()[1100]->vPos, nullptr));
			hr = m_pObjectMgr->AddObject(OBJ_PLAYER, CPlayer::Create(CTileMgr::GetInstance()->Get_Tile()[1106]->vPos, nullptr));
			break;
		default:
			break;
		}
		CObjectMgr::GetInstance()->GetPlayer()->Set_Pos(CTileMgr::GetInstance()->Get_Tile()[1081]->vPos);
	}



	hr = m_pObjectMgr->AddObject(OBJ_SHOP, CShop::Create(CTileMgr::GetInstance()->Get_Tile()[1090]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Shop Create Failed", E_FAIL);
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBat::Create(CTileMgr::GetInstance()->Get_Tile()[690]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBat::Create(CTileMgr::GetInstance()->Get_Tile()[709]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	VEC3 vPos = CTileMgr::GetInstance()->Get_Tile()[1116]->vPos;
	m_tRectDoor = { (LONG)vPos.x - 2, (LONG)vPos.y - 32, (LONG)vPos.x + 2, (LONG)vPos.y };

	m_bChangeScene = false;
	m_fTimeCountClose = 0.f;

	m_bOpenScene = true;
	m_fTimeCountOpen = 0.f;

	m_fGateCount = 0.f;
	return S_OK;
}

int CHome::Update()
{
	if (m_bChangeScene)
	{
		m_fTimeCountClose += GET_TIME;

		if (m_fTimeCountClose >= 2.f)
		{
			CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_STAGE1);
			return SCENE_CHANGE;
		}
	}
	else
	{
		if (m_bOpenScene)
		{
			m_fTimeCountOpen += GET_TIME;
			if (m_fTimeCountOpen >= 1.f)
			{
				m_bOpenScene = false;
			}
		}
		m_pObjectMgr->Update();
	}
	return 0;
}

void CHome::LateUpdate()
{
	m_pObjectMgr->LateUpdate();

	RECT rc = {};
	if (IntersectRect(&rc, m_pObjectMgr->GetPlayer()->Get_Rect(), &m_tRectDoor))
	{
		m_bChangeScene = true;

	}
}

void CHome::Render()
{
	if(m_bChangeScene)
		m_pObjectMgr->Render(false);
	else
		m_pObjectMgr->Render();
	MATRIX matScale, matTrans, matWorld;
	if (m_bOpenScene)
	{
		MATSCAILING(&matScale, (1.f - m_fTimeCountOpen) * 1.2f, (1.f - m_fTimeCountOpen) * 1.2f, 1.f);
		MATTRANSLATION(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Sceen", &matWorld, L"SceenChange", 0, 255, RENDER::MID);
	}
	if (m_bChangeScene)
	{
		if (m_fTimeCountClose <= 1.f)
		{
			m_fGateCount += 28 * GET_TIME;
			MATTRANSLATION(&matTrans, (m_tRectDoor.left + m_tRectDoor.right) * 0.5f - CScrollMgr::Get_Scroll().x, m_tRectDoor.bottom - CScrollMgr::Get_Scroll().y, 0.f);
			CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"DungeonGate", &matTrans, L"Gate", (int)m_fGateCount, 255, RENDER::BOTTOM);
		}
		if (m_fTimeCountClose >= 0.5f)
		{
			MATSCAILING(&matScale, (m_fTimeCountClose - 0.5f) * 1.2f, (m_fTimeCountClose - 0.5f) * 1.2f, 1.f);
			MATTRANSLATION(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
			matWorld = matScale * matTrans;
			CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Sceen", &matWorld, L"SceenChange", 0, 255, RENDER::MID);
		}
	}
}

void CHome::Release()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}
