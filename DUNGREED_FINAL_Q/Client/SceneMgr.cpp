#include "stdafx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "Home.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"
#include "StageCrevas.h"

IMPLEMENT_SINGLETON(CSceneMgr)
CSceneMgr::CSceneMgr()
	: m_eCurScene(SCENE_END)
	, m_eNextScene(SCENE_END)
	, m_pScene(NULL)
	, m_iEvent(0)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Update()
{
	m_iEvent = m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	if (m_iEvent == SCENE_CHANGE)
		return;
	m_pScene->LateUpdate();
}

void CSceneMgr::Render()
{
	if (m_iEvent == SCENE_CHANGE)
	{
		m_iEvent = 0;
	}
	m_pScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}

void CSceneMgr::ChangeScene(SCENEID eNextScene)
{
	m_eNextScene = eNextScene;
	if (m_eNextScene != m_eCurScene)
	{
		SafeDelete(m_pScene);
		switch (eNextScene)
		{
		case CSceneMgr::SCENE_LOGO:
			m_pScene = new CLogo;
			break;
		case CSceneMgr::SCENE_STAGE1:
			m_pScene = new CStage1;
			break;
		case CSceneMgr::SCENE_STAGE2:
			m_pScene = new CStage2;
			break;
		case CSceneMgr::SCENE_STAGE3:
			m_pScene = new CStage3;
			break;
		case CSceneMgr::SCENE_STAGE4:
			m_pScene = new CStage4;
			break;
		case CSceneMgr::SCENE_STAGE5:
			m_pScene = new CStage5;
			break;
		case CSceneMgr::SCENE_HOME:
			m_pScene = new CHome;
			break;
		case CSceneMgr::SCENE_CREVAS:
			m_pScene = new CStageCrevas;
		default:
			break;
		}
		m_pScene->Initiailize();
		m_eCurScene = m_eNextScene;
	}
}
