#include "stdafx.h"
#include "StageCrevas.h"

#include "Player.h"
#include "GunMan.h"
#include "RapierMan.h"

#include "Sword.h"
#include "Gun.h"
#include "Spear.h"
#include "Stamp.h"

#include "SkullDog.h"
#include "Guardian.h"
#include "Bat.h"
#include "Bansh.h"
#include "IceMagician.h"
#include "Niflheim.h"

#include "Box.h"
#include "SealStone.h"

CStageCrevas::CStageCrevas()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance()),
	m_pSubject(CSubject::GetInstance())
{
}


CStageCrevas::~CStageCrevas()
{
}

HRESULT CStageCrevas::Initiailize()
{
	m_pObjectMgr->DeleteLst(OBJ_MONSTER);
	m_pObjectMgr->DeleteLst(OBJ_BOSS);
	m_pObjectMgr->DeleteLst(OBJ_ICEPILLAR);
	m_pObjectMgr->DeleteLst(OBJ_PLAYER_HITBOX);
	m_pObjectMgr->DeleteLst(OBJ_MONSTER_HITBOX);
	m_pObjectMgr->DeleteLst(OBJ_EFFECT);
	// Terrain Object
	//hr = m_pObjectMgr->AddObject(TERRAIN, CTerrain::Create());
	//FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);
	CTileMgr::GetInstance()->Initialize(L"../Data/MapData_StageCrevas.dat");

	// Player Object
	m_pObjectMgr->GetPlayer()->Set_Pos(CTileMgr::GetInstance()->Get_Tile()[35]->vPos);



	
	HRESULT hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBox::Create(CTileMgr::GetInstance()->Get_Tile()[1099]->vPos, BOX::WING));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	VEC3 vPos = { CTileMgr::GetInstance()->Get_Tile()[1024]->vPos.x + 8.f, CTileMgr::GetInstance()->Get_Tile()[1024]->vPos.y, 0.f };
	m_pObjectMgr->AddObject(OBJ_TILE, CSealStone::Create(vPos));
	vPos = { CTileMgr::GetInstance()->Get_Tile()[1104]->vPos.x + 8.f, CTileMgr::GetInstance()->Get_Tile()[1104]->vPos.y, 0.f };
	m_pObjectMgr->AddObject(OBJ_TILE, CSealStone::Create(vPos));
	//
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBat::Create(CTileMgr::GetInstance()->Get_Tile()[428]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	//
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1092]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1096]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1100]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1104]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	//hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1108]->vPos));
	//FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	m_bChangeScene = false;
	m_fTimeCountClose = 0.f;

	m_bOpenScene = true;
	m_fTimeCountOpen = 0.f;

	m_bSeal = false;

	return S_OK;
}

int CStageCrevas::Update()
{
	if (m_bChangeScene)
	{
		m_fTimeCountClose += GET_TIME;

		if (m_fTimeCountClose >= 1.0f)
		{
			CObjectMgr::GetInstance()->SetClear();
			CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_HOME);
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
	if (!m_bSeal && CObjectMgr::GetInstance()->GetPlayer()->Get_Info()->vPos.x < 380)
	{
		for (auto& pTile : *CObjectMgr::GetInstance()->GetLst(OBJ_TILE))
		{
			SCAST(CSealStone*, pTile)->OnSeal();
		}
		m_bSeal = true;
	}
	return 0;
}

void CStageCrevas::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	RECT rc;
	RECT rcSrc = CTileMgr::GetInstance()->Get_Tile()[34]->m_tRect;
	rcSrc.right = rcSrc.left + 48;
	rcSrc.bottom = rcSrc.top + 16;
	if (IntersectRect(&rc, m_pObjectMgr->GetPlayer()->Get_Rect(), &rcSrc) && m_pObjectMgr->GetLst(OBJ_MONSTER)->empty())
	{
		//CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_STAGE2);
		m_bChangeScene = true;
	}
}

void CStageCrevas::Render()
{
	m_pObjectMgr->Render();
	if (m_bOpenScene)
	{
		MATRIX matScale, matTrans, matWorld;
		MATSCAILING(&matScale, (1.f - m_fTimeCountOpen) * 1.2f, (1.f - m_fTimeCountOpen) * 1.2f, 1.f);
		MATTRANSLATION(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Sceen", &matWorld, L"SceenChange", 0, 255, RENDER::MID);
	}
	if (m_bChangeScene)
	{
		MATRIX matScale, matTrans, matWorld;
		MATSCAILING(&matScale, m_fTimeCountClose * 1.2f, m_fTimeCountClose * 1.2f, 1.f);
		MATTRANSLATION(&matTrans, WINCX * 0.5f, WINCY * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Sceen", &matWorld, L"SceenChange", 0, 255, RENDER::MID);
	}
}

void CStageCrevas::Release()
{
}
