#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO* CSingleTexture::GetTexInfo(
	const wstring& wstrStateKey, 
	int iIndex) const
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::LoadTexture(
	LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey/* = L""*/, 
	int iCount/* = 0*/)
{
	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(tImgInfo));

	// 이미지 정보를 얻어오는 함수.
	HRESULT hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

	LPDIRECT3DTEXTURE9	pTexture = nullptr;

	// 이미지 로드 함수. (IDirect3DTexture9 COM 객체 생성)
	hr = D3DXCreateTextureFromFileEx(
		pGraphicDev, 
		wstrFilePath.c_str(), 
		tImgInfo.Width,		/* 생성될 텍스처의 가로 크기 */
		tImgInfo.Height,	/* 생성될 텍스처의 세로 크기 */
		tImgInfo.MipLevels, /* 밉맵 단계 */
		0, /* Usage: 텍스처 사용방식 */
		tImgInfo.Format, /* 텍스처의 픽셀 포맷 */
		D3DPOOL_MANAGED, /* 메모리 보관 방식 */
		D3DX_DEFAULT,	/* 필터링 방식 */
		D3DX_DEFAULT,	/* 필터링 방식 */
		0,	/* 제거할 색상 키 */
		nullptr, /* D3DXIMAGE_INFO의 포인터 */
		nullptr, /* PALETTEENTRY의 포인터 */
		&pTexture);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateTextureFromFileEx Failed", E_FAIL);

	m_pTexInfo = new TEX_INFO;
	m_pTexInfo->pTexture = pTexture;
	m_pTexInfo->tImgInfo = tImgInfo;

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	SafeDelete(m_pTexInfo);
}

CSingleTexture* CSingleTexture::Create(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	const wstring& wstrFilePath)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);
	CSingleTexture* pInstance = new CSingleTexture;

	if (FAILED(pInstance->LoadTexture(pGraphicDev, wstrFilePath)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
