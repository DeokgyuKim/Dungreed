#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", 
		int iIndex = 0) const override;

public:
	virtual HRESULT LoadTexture(
		LPDIRECT3DDEVICE9 pGraphicDev, 
		const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		int iCount = 0) override;

private:
	virtual void Release() override;

public:
	static CMultiTexture* Create(
		LPDIRECT3DDEVICE9 pGraphicDev,
		const wstring& wstrFilePath,
		const wstring& wstrStateKey = L"",
		int iCount = 0);

private:
	typedef vector<TEX_INFO*>			VEC_TEXINFO;
	typedef map<wstring, VEC_TEXINFO>	MAP_TEXINFO;
	MAP_TEXINFO		m_MultiTexture;
};

