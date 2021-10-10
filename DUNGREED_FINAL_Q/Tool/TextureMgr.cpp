#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)
CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO * CTextureMgr::Get_TextInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, int iIndex) const
{
	auto& iter = m_mapTexture.find(wstrObjectKey);
	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CTextureMgr::Load_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrFilePath, TEXTURE_TYPE eType, const wstring & wstrObjectKey, const wstring & wstrStateKey, int iCount)
{
	CTexture*	pTexture = NULL;
	bool		bExist = (m_mapTexture.end() != m_mapTexture.find(wstrObjectKey));

	switch (eType)
	{
	case SINGLE_TEXTURE:
		if (!bExist)
		{
			pTexture = CSingleTexture::Create(pGraphicDev, wstrFilePath);
			NULL_CHECK_RETURN(pTexture, E_FAIL);
			m_mapTexture[wstrObjectKey] = pTexture;
		}
		else
			return E_FAIL;
		break;
	case MULTI_TEXTURE:
		if (!bExist)
		{
			pTexture = CMultiTexture::Create(pGraphicDev, wstrFilePath, wstrStateKey, iCount);
			NULL_CHECK_RETURN(pTexture, E_FAIL);
			m_mapTexture[wstrObjectKey] = pTexture;
		}
		else
		{
			HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(pGraphicDev, wstrFilePath, wstrStateKey, iCount);
			FAILED_CHECK_RETURN(hr, E_FAIL);
		}
		break;
	}
	return S_OK;
}

HRESULT CTextureMgr::Render(TEXTURE_TYPE eType, const wstring & wstrObjectKey, MATRIX * matWorld, const wstring & wstrStateKey, int iCount)
{
	if (m_mapTexture.end() == m_mapTexture.find(wstrObjectKey))
	{
		::MessageBox(g_hWnd, wstrObjectKey.c_str(), L"System Error!", MB_OK);
		return E_FAIL;
	}

	const TEX_INFO* pTexInfo = NULL;
	pTexInfo = m_mapTexture[wstrObjectKey]->GetTexInfo(wstrStateKey, iCount);
	NULL_CHECK_RETURN(pTexInfo, E_FAIL);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = (float)pTexInfo->tImgInfo.Height;// *0.5f;

	if(matWorld != NULL)
		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(matWorld);
	HRESULT hr = CDeviceMgr::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &VEC3(fCenterX, fCenterY, 0.f), nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	FAILED_CHECK_MSG_RETURN(hr, L"Draw Failed", E_FAIL);

	return S_OK;
}
HRESULT CTextureMgr::LoadFromPathInfoFile(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath)
{
	wifstream fin;

	fin.open(pFilePath);

	if (fin.fail())
		return E_FAIL;

	TCHAR szObjectKey[MAX_STR] = L"";
	TCHAR szStateKey[MAX_STR] = L"";
	TCHAR szCount[MAX_STR] = L"";
	TCHAR szPath[MAX_STR] = L"";

	while (true)
	{
		fin.getline(szObjectKey, MAX_STR, '|');
		fin.getline(szStateKey, MAX_STR, '|');
		fin.getline(szCount, MAX_STR, '|');
		fin.getline(szPath, MAX_STR);

		if (fin.eof())
			break;

		HRESULT hr = Load_Texture(
			pGraphicDev,
			szPath,
			MULTI_TEXTURE,
			szObjectKey,
			szStateKey,
			_ttoi(szCount));

		if (FAILED(hr))
		{
			fin.close();
			return E_FAIL;
		}
	}

	fin.close();

	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& Pair : m_mapTexture)
		SafeDelete(Pair.second);
	m_mapTexture.clear();
}
