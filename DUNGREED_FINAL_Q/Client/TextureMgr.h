#pragma once
class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	CTextureMgr();
	~CTextureMgr();
public:
	const TEX_INFO*	Get_TextInfo(const wstring& wstrObjectKey, const wstring& wstrStateKey = L"",
		int iIndex = 0) const;

	HRESULT	Load_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrFilePath, TEXTURE_TYPE eType,
		const wstring& wstrObjectKey, const wstring& wstrStateKey = L"", int iCount = 0);
	HRESULT	Render(TEXTURE_TYPE eType, const wstring& wstrObjectKey, MATRIX* matWorld = NULL, const wstring& wstrStateKey = L"", int iCount = 0, DWORD dwAlpha = 255, RENDER::POS ePos = RENDER::BOTTOM);
	HRESULT CTextureMgr::LoadFromPathInfoFile( LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath);
	int	GetCount() { return m_iCount; }
private:
	void Release();
private:
	map<wstring, CTexture*>	m_mapTexture;
	int m_iCount;	//MAX 142
};

