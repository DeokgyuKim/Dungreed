#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO* CMultiTexture::GetTexInfo(
	const wstring& wstrStateKey/* = L""*/, 
	int iIndex/* = 0*/) const
{
	auto iter_find = m_MultiTexture.find(wstrStateKey);

	if(m_MultiTexture.end() == iter_find)
		return nullptr;

	if (0 > iIndex || iter_find->second.size() <= (size_t)iIndex)
		return nullptr;

	return iter_find->second[iIndex];
}

HRESULT CMultiTexture::LoadTexture(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey/* = L""*/, 
	int iCount/* = 0*/)
{
	auto iter_find = m_MultiTexture.find(wstrStateKey);

	if (m_MultiTexture.end() != iter_find)
		return E_FAIL;

	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(tImgInfo));

	LPDIRECT3DTEXTURE9	pTexture = nullptr;
	TCHAR szFullPath[MAX_STR] = L"";

	TEX_INFO* pTexInfo = nullptr;

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		// 이미지 정보를 얻어오는 함수.
		HRESULT hr = D3DXGetImageInfoFromFile(szFullPath, &tImgInfo);
		FAILED_CHECK_MSG_RETURN(hr, L"D3DXGetImageInfoFromFile Failed", E_FAIL);

		// 이미지 로드 함수. (IDirect3DTexture9 COM 객체 생성)
		hr = D3DXCreateTextureFromFileEx(
			pGraphicDev,
			szFullPath,
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

		pTexInfo = new TEX_INFO;
		pTexInfo->pTexture = pTexture;
		pTexInfo->tImgInfo = tImgInfo;

		m_MultiTexture[wstrStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& MyPair : m_MultiTexture)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), 
			[](auto& pTexInfo)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		});

		MyPair.second.clear();
	}

	m_MultiTexture.clear();
}

CMultiTexture* CMultiTexture::Create(
	LPDIRECT3DDEVICE9 pGraphicDev, 
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey, 
	int iCount)
{
	NULL_CHECK_RETURN(pGraphicDev, nullptr);

	CMultiTexture* pInstance = new CMultiTexture;

	if (FAILED(pInstance->LoadTexture(pGraphicDev, wstrFilePath, wstrStateKey, iCount)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
