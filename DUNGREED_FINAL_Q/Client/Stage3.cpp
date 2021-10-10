#include "stdafx.h"
#include "Stage3.h"

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

CStage3::CStage3()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance()),
	m_pSubject(CSubject::GetInstance())
{
}


CStage3::~CStage3()
{
}

HRESULT CStage3::Initiailize()
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
	CTileMgr::GetInstance()->Initialize(L"../Data/MapData_Stage3.dat");

	// Player Object
	m_pObjectMgr->GetPlayer()->Set_Pos(CTileMgr::GetInstance()->Get_Tile()[1081]->vPos);



	
	HRESULT hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CIceMagician::Create(CTileMgr::GetInstance()->Get_Tile()[928]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBansh::Create(CTileMgr::GetInstance()->Get_Tile()[938]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CIceMagician::Create(CTileMgr::GetInstance()->Get_Tile()[948]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBansh::Create(CTileMgr::GetInstance()->Get_Tile()[793]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CIceMagician::Create(CTileMgr::GetInstance()->Get_Tile()[633]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CIceMagician::Create(CTileMgr::GetInstance()->Get_Tile()[423]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CIceMagician::Create(CTileMgr::GetInstance()->Get_Tile()[413]->vPos));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	//404�ڽ�
	hr = m_pObjectMgr->AddObject(OBJ_MONSTER, CBox::Create(CTileMgr::GetInstance()->Get_Tile()[404]->vPos, BOX::DART));
	FAILED_CHECK_MSG_RETURN(hr, L"Monster Create Failed", E_FAIL);

	m_bChangeScene = false;
	m_fTimeCountClose = 0.f;

	m_bOpenScene = true;
	m_fTimeCountOpen = 0.f;

	return S_OK;
}

int CStage3::Update()
{
	if (m_bChangeScene)
	{
		m_fTimeCountClose += GET_TIME;

		if (m_fTimeCountClose >= 1.0f)
		{
			CSceneMgr::GetInstance()->ChangeScene(CSceneMgr::SCENE_STAGE4);
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

void CStage3::LateUpdate()
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

void CStage3::Render()
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

void CStage3::Release()
{
}
