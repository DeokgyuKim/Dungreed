#include "stdafx.h"
#include "Logo.h"

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

CLogo::CLogo()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pKeyMgr(CKeyMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance()),
	m_pAstarMgr(CAstarMgr::GetInstance()),
	m_pSubject(CSubject::GetInstance()),
	m_hLoadingThread(nullptr)
{
}


CLogo::~CLogo()
{
	Release();
}

HRESULT CLogo::Initiailize()
{
	CTextureMgr::GetInstance()->LoadFromPathInfoFile(
		CDeviceMgr::GetInstance()->GetDevice(),
		L"../Data/PathInfo_DUNGREED_BASE.txt");

	CSoundMgr::Get_Instance()->Initialize();

	CSoundMgr::Get_Instance()->PlayBGM(L"MyTitle.wav");

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingFunc, this, 0, nullptr);
	NULL_CHECK_RETURN(m_hLoadingThread, E_FAIL);

	InitializeCriticalSection(&m_Critical);

	//// Multi Texture Load
	//HRESULT hr = m_pTextureMgr->LoadFromPathInfoFile(
	//	m_pDeviceMgr->GetDevice(),
	//	L"../Data/PathInfo_DUNGREED.txt");
	//FAILED_CHECK_MSG_RETURN(hr, L"LoadFromPathInfoFile Failed", E_FAIL);
	//CSoundMgr::Get_Instance()->Initialize();


	m_bChangeScene = false;
	m_fTimeCountClose = 0.f;

	m_bOpenScene = true;
	m_fTimeCountOpen = 0.f;

	m_fGateCount = 0.f;

	m_iIndex = 1;
	return S_OK;
}

int CLogo::Update()
{
	if (m_bChangeScene)
	{
		m_fTimeCountClose += GET_TIME;

		if (m_fTimeCountClose >= 2.f)
		{
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
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (m_iIndex == 1)
			{
				WaitForSingleObject(m_hLoadingThread, INFINITE);

				DWORD dwExitCode = 0;
				GetExitCodeThread(m_hLoadingThread, &dwExitCode); // 쓰레드의 종료코드를 얻어오는 함수.

				if (LOAD_FAIL == dwExitCode)
					return LOAD_FAIL;
				m_bChangeScene = true;
			}
			else
				DestroyWindow(g_hWnd);
		}
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_S))
	{
		if (++m_iIndex > 2)
			m_iIndex = 2;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_W))
	{
		if (--m_iIndex < 1)
			m_iIndex = 1;
	}
	return 0;
}

void CLogo::LateUpdate()
{
}

void CLogo::Render()
{
	MATRIX matScale, matTrans, matWorld;
	MATTRANSLATION(&matTrans, 0.f, 0.f, 0.f);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Logo", &matTrans, L"MenuIdle", 0, 255, RENDER::LEFTTOP);

	if (m_iIndex == 1)
	{
		MATTRANSLATION(&matTrans, 160.f, 110.f, 0.f);
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"GameStart", &matTrans, L"PlayOn", 0, 255, RENDER::MID);
		MATTRANSLATION(&matTrans, 160.f, 130.f, 0.f);
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Exit", &matTrans, L"ExitOff", 0, 255, RENDER::MID);
	}
	else
	{
		MATTRANSLATION(&matTrans, 160.f, 110.f, 0.f);
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"GameStart", &matTrans, L"PlayOff", 0, 255, RENDER::MID);
		MATTRANSLATION(&matTrans, 160.f, 130.f, 0.f);
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Exit", &matTrans, L"ExitOn", 0, 255, RENDER::MID);
	}
	int iCount = m_pTextureMgr->GetInstance()->GetCount();
	MATSCAILING(&matScale, iCount / 141.f, 1.f, 0.f);
	//cout << iCount / 141.f << endl;
	MATTRANSLATION(&matTrans, 160.f, 150.f, 0.f);
	matWorld = matScale * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Logo", &matWorld, L"LoadingBar", 0, 255, RENDER::MID);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Logo", &matTrans, L"LoadingBarBase", 0, 255, RENDER::MID);

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

void CLogo::Release()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_Critical);
}

unsigned CLogo::LoadingFunc(void * pParam)
{
	CLogo* pLogo = reinterpret_cast<CLogo*>(pParam);
	NULL_CHECK_RETURN(pLogo, LOAD_FAIL);

	EnterCriticalSection(&pLogo->m_Critical);

	HRESULT hr = CTextureMgr::GetInstance()->LoadFromPathInfoFile(
		CDeviceMgr::GetInstance()->GetDevice(),
		L"../Data/PathInfo_DUNGREED.txt");

	if (FAILED(hr))
	{
		LeaveCriticalSection(&pLogo->m_Critical);

		::MessageBox(0, L"LoadFromPathInfoFile Failed", L"System Error", MB_OK);
		return LOAD_FAIL;
	}

	LeaveCriticalSection(&pLogo->m_Critical);

	return 0;
}
