#include "stdafx.h"
#include "Stage2.h"

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

CStage2::CStage2()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance()),
	m_pSubject(CSubject::GetInstance())
{
}


CStage2::~CStage2()
{
}

HRESULT CStage2::Initiailize()
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
	CTileMgr::GetInstance()->Initialize(L"../Data/MapData_Stage2.dat");

	// Player Object
	m_pObjectMgr->GetPlayer()->Set_Pos(CTileMgr::GetInstance()->Get_Tile()[1081]->vPos);



	
	HRESULT hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBat::Create(CTileMgr::GetInstance()->Get_Tile()[742]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBat::Create(CTileMgr::GetInstance()->Get_Tile()[428]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1092]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1096]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1100]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1104]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CSkullDog::Create(CTileMgr::GetInstance()->Get_Tile()[1108]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	if (m_pObjectMgr->GetClear())
	{
		hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBox::Create(CTileMgr::GetInstance()->Get_Tile()[1090]->vPos, BOX::STAMP));
		FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	}
	else
	{
		hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBox::Create(CTileMgr::GetInstance()->Get_Tile()[1090]->vPos, BOX::TIMERING));
		FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	}

	m_bChangeScene = false;
	m_fTimeCountClose = 0.f;

	m_bOpenScene = true;
	m_fTimeCountOpen = 0.f;

	return S_OK;
}

int CStage2::Update()
{
	if (m_bChangeScene)
	{
		m_fTimeCountClose += GET_TIME;

		if (m_fTimeCountClose >= 1.0f)
		{
			if(CObjectMgr::GetInstance()->GetClear())
				CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_STAGE3);
			else
				CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_CREVAS);
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

void CStage2::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	RECT rc;
	RECT rcSrc = CTileMgr::GetInstance()->Get_Tile()[1119]->m_tRect;
	rcSrc.right = rcSrc.left + 16;
	rcSrc.bottom = rcSrc.top + 16;
	if (IntersectRect(&rc, m_pObjectMgr->GetPlayer()->Get_Rect(), &rcSrc) && m_pObjectMgr->GetLst(OBJ_MONSTER)->empty())
	{
		//CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_STAGE2);
		m_bChangeScene = true;
	}
}

void CStage2::Render()
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

void CStage2::Release()
{
}
