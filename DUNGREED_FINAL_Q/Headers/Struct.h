#pragma once

typedef struct tagPathInfo
{
	wstring wstrObjectKey;
	wstring wstrStateKey;
	wstring wstrRelativePath;
	int iFileCount;
}PATH_INFO;

typedef struct tagUnitInfo
{
	wstring wstrName;
	int iAtt, iDef;
	int iJobIndex, iItem;
}UNIT_INFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	BYTE byDrawID;
	BYTE byOption;
	int iMyIndex;
	int iParentIndex;
	RECT m_tRect;
}TILE_INFO;

typedef struct tagTextureInfo
{
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO	tImgInfo;
}TEX_INFO;

typedef struct tagInfo
{
	//D3DXVECTOR2: 2���� ����
	//D3DXVECTOR3: 3���� ����
	//D3DXVECTOR4: 3���� ����

	VEC3	vPos;			// ��ġ����
	VEC3	vDir;			// ���⺤��
	VEC3	vRenderDir;
	VEC3	vSize;
	float	fSpeed;
	int		iAtt;
	int		iHp;
	int		m_iGold;
	bool	bDead;
	D3DXMATRIX matWorld;		// �������

}INFO;

typedef struct tagFrameInfo {
	TCHAR*		pObjectKey;
	TCHAR*		pStateKey;
	float		fStart;
	float		fCount;
	bool		bReverse;
}FRAME;