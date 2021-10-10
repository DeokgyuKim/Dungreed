#include "stdafx.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
	: m_pSDK(nullptr), m_pGraphicDev(nullptr)
{
}


CDeviceMgr::~CDeviceMgr()
{
	Release();
}

LPDIRECT3DDEVICE9 CDeviceMgr::GetDevice() const
{
	return m_pGraphicDev;
}

LPD3DXSPRITE CDeviceMgr::GetSprite() const
{
	return m_pSprite;
}

LPD3DXFONT CDeviceMgr::GetFont() const
{
	return m_pFont;
}

LPD3DXLINE CDeviceMgr::GetLine() const
{
	return m_pLine;
}

HRESULT CDeviceMgr::InitDevice(DISPLAY_MODE eMode)
{
	// 장치 초기화 과정.
	// 1. SDK 객체 생성 (IDirect3D9)
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_MSG_RETURN(m_pSDK, L"Direct3DCreate9 Failed", E_FAIL);

	// 2. SDK 객체를 통해 현재 사용할 장치를 조사.
	D3DCAPS9 d3dcaps; // 장치 정보
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// HAL(Hardware Abstraction Layer, 하드웨어 추상 계층)
	// 현재 장치 정보를 HAL에서 얻어내는 함수.
	HRESULT hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 2-1. 위에서 얻어낸 장치 정보를 통해 현재 장치가 "버텍스 프로세싱"을 지원하는지 확인.
	// 버텍스 프로세싱: 정점 변환 + 조명 연산

	DWORD vp = 0; // 장치 초기화 옵션
	
	// 장치가 버텍스 프로세싱을 지원하는지?
	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING; // 지원한다면 장치의 버텍스 프로세싱을 사용.
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	// 3. SDK 객체를 통해 장치 객체(IDirect3DDevice9) 생성.
	D3DPRESENT_PARAMETERS d3dpp; // 렌더링 환경 정보.
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = WINCX * RATIO;	// 후면버퍼 가로크기
	d3dpp.BackBufferHeight = WINCY * RATIO;	// 후면버퍼 세로크기
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // 후면버퍼 픽셀포맷
	d3dpp.BackBufferCount = 1; // 후면버퍼 개수
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // 멀티 샘플링
	d3dpp.MultiSampleQuality = 0;	// 품질
	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 스왑체인 방식을 사용한다.
	d3dpp.hDeviceWindow = g_hWnd; // 기본 출력 대상.
 	d3dpp.Windowed = eMode; // TRUE: 창 모드, FALSE: 전체화면 모드
	d3dpp.EnableAutoDepthStencil = TRUE; // 장치가 깊이버퍼(Z버퍼)와 스텐실버퍼를 자동으로 관리한다.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // 전체화면일 때 모니터 주사율
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		g_hWnd, vp, &d3dpp, &m_pGraphicDev);
	FAILED_CHECK_MSG_RETURN(hr, L"CreateDevice Failed", E_FAIL);

	hr = D3DXCreateSprite(m_pGraphicDev, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	D3DXFONT_DESC	tFontInfo;	// 폰트 정보
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 20;
	tFontInfo.Height = 30;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"둥근모꼴"); // 서체

	hr = D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	hr = D3DXCreateLine(m_pGraphicDev, &m_pLine);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateLine Failed", E_FAIL);

	m_pLine->SetWidth(2.f);

	return S_OK;
}

void CDeviceMgr::Render_Begin()
{
	// 렌더링 과정
	// 1. 후면버퍼를 비운다.
	m_pGraphicDev->Clear(0, nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	// 2. 후면버퍼에 그리기를 시작한다.
	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);	// 알파 블렌딩 상태로 그린다.
}

void CDeviceMgr::Render_End()
{
	// 3. 후면버퍼에 그리기를 마친다.
	m_pSprite->End();
	m_pGraphicDev->EndScene();

	// 4. 후면버퍼를 전면버퍼로 출력한다.
	m_pGraphicDev->Present(nullptr, nullptr, nullptr, nullptr);
}

void CDeviceMgr::Release()
{
	// 각 COM객체마다 AddRef와 Release 멤버 함수를 지니고 있다.
	// AddRef: Reference Count 증가
	// Release: Reference Count 감소
	// 현재 COM객체가 갖고 있는 Reference Count의 값이 0일 때 소멸된다.
	if (m_pLine->Release())
		::MessageBox(0, L"m_pLine Release Failed", L"System Error", MB_OK);
	if (m_pFont->Release())
		::MessageBox(0, L"m_pFont Release Failed", L"System Error", MB_OK);
	if(m_pSprite->Release())
		::MessageBox(0, L"m_pSprite Release Failed", L"System Error", MB_OK);
	if (m_pGraphicDev->Release())
		::MessageBox(0, L"m_pGraphicDev Release Failed", L"System Error", MB_OK);
	if(m_pSDK->Release())
		::MessageBox(0, L"m_pSDK Release Failed", L"System Error", MB_OK);
}
