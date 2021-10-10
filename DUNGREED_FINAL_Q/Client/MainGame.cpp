#include "stdafx.h"
#include "MainGame.h"
#include "Terrain.h"
#include "Player.h"
#include "SoundMgr.h"


CMainGame::CMainGame()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance()),
	m_pSubject(CSubject::GetInstance()),
	m_pSceneMgr(CSceneMgr::GetInstance()),
	m_fTimeCount(0.f), m_iFPS(0), m_szFPS(L"")
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Update()
{	
	CSoundMgr::Get_Instance()->Update();
	CMouse::GetInstance()->Update();
	m_pTimeMgr->Update();
	m_pKeyMgr->Update();
	m_pSceneMgr->Update();
	//CSoundMgr::Get_Instance()->Update();
}

void CMainGame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
}

void CMainGame::Render()
{
	m_pDeviceMgr->Render_Begin();

	// �������� �� ���̿� ������Ʈ ������ �ڵ� ����.
	m_pSceneMgr->Render();
	RenderFPS();
	if(CSceneMgr::GetInstance()->Get_SceneID() != CSceneMgr::SCENE_LOGO)
		CMouse::GetInstance()->Render();
	
	m_pDeviceMgr->Render_End();		
}

HRESULT CMainGame::Initialize()
{
	HRESULT hr = m_pDeviceMgr->InitDevice(MODE_FULL);
	FAILED_CHECK_MSG_RETURN(hr, L"InitDevice Failed", E_FAIL);
	m_pSceneMgr->GetInstance()->ChangeScene(CSceneMgr::SCENE_LOGO);

	srand(unsigned(time(NULL)));
	return S_OK;
}

void CMainGame::Release()
{		
	m_pAstarMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pSubject->DestroyInstance();
	m_pKeyMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
	m_pSceneMgr->DestroyInstance();
	CMouse::GetInstance()->DestroyInstance();
	CUiMgr::GetInstance()->DestroyInstance();
}

void CMainGame::RenderFPS()
{
	++m_iFPS;

	m_fTimeCount += m_pTimeMgr->GetDeltaTime();

	if (1.f <= m_fTimeCount)
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		m_iFPS = 0;
		m_fTimeCount = 0.f;
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 600.f, 100.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);

	// ���̷�Ʈ ��Ʈ�� ���ڿ� ����� ������ �� �� �´� �ؽ�ó�� ���� ����Ѵ�. �̳� ������.
	//m_pDeviceMgr->GetFont()->DrawText(
	//	m_pDeviceMgr->GetSprite(),
	//	m_szFPS, /* ����� ���ڿ� */
	//	lstrlen(m_szFPS), /* ���� */
	//	nullptr, /* RECT ������ */
	//	0,
	//	D3DCOLOR_ARGB(255, 0, 255, 0));
}

CMainGame* CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
