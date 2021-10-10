#include "stdafx.h"
#include "Terrain.h"
#include "Mouse.h"
#include "ScrollMgr.h"

CTerrain::CTerrain()
	: m_pKeyMgr(CKeyMgr::GetInstance())
{
}


CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, (int)i))
			return (int)i;
	}

	return -1;
}

const vector<TILE_INFO*>& CTerrain::GetVecTile() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecTile;
}

const vector<list<TILE_INFO*>>& CTerrain::GetVecGraph() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecGraph;
}

int CTerrain::Update()
{
	CGameObject::LateInit();

	// 초당 300씩 움직여라!
	float fSpeed = 300.f * m_pTimeMgr->GetDeltaTime();	

	//system("cls");
	//cout << m_pTimeMgr->GetDeltaTime() << endl;

	if (0.f > CMouse::GetMousePos().x)
		CScrollMgr::SetScrollMove(D3DXVECTOR3(-fSpeed, 0.f, 0.f));
	if (0.f > CMouse::GetMousePos().y)
		CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, -fSpeed, 0.f));
	if ((float)WINCX < CMouse::GetMousePos().x)
		CScrollMgr::SetScrollMove(D3DXVECTOR3(fSpeed, 0.f, 0.f));
	if ((float)WINCY < CMouse::GetMousePos().y)
		CScrollMgr::SetScrollMove(D3DXVECTOR3(0.f, fSpeed, 0.f));	

	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Terrain", L"Tile", m_vecTile[i]->byDrawID);
		NULL_CHECK(pTexInfo);

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, 
			m_vecTile[i]->vPos.x - CScrollMgr::Get_Scroll().x, 
			m_vecTile[i]->vPos.y - CScrollMgr::Get_Scroll().y,
			0.f);

		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(
			pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CTerrain::Initialize()
{
	m_vecTile.reserve(TILE_COUNT_X * TILE_COUNT_Y);

	FAILED_CHECK_RETURN(LoadData(L"../Data/MapData.dat"), E_FAIL);
	FAILED_CHECK_RETURN(CreateGraph(), E_FAIL);

	return S_OK;
}

HRESULT CTerrain::LateInit()
{
	return S_OK;
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
}

void CTerrain::TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption)
{
	int iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;
	m_vecTile[iIndex]->byOption = byOption;
}

bool CTerrain::IsPicking(const D3DXVECTOR3& vPos, int iIndex)
{
	// 내적을 이용한 방식 (데카르트 기준)

	// 12시, 3시, 6시, 9시 순으로 점 4개를 구한다. 
	D3DXVECTOR3 vPoint[4] =
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f },
		{ m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f },
		{ m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f }
	};

	// 4개의 방향벡터를 구함.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 위에서 구한 방향벡터들의 각 법선벡터를 구함.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// 마우스 위치와 4개의 vPoint와의 방향벡터를 구함.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vNormal과 vMouseDir를 내적해서 모두 음수(둔각)가 나온다면 true
	for (int i = 0; i < 4; ++i)
	{
		// 하나라도 양수(예각)가 나오면 false
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

HRESULT CTerrain::LoadData(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(),
			[](auto& pTile)
		{
			if (pTile)
			{
				delete pTile;
				pTile = nullptr;
			}
		});

		m_vecTile.clear();
	}

	DWORD dwBytes = 0;
	TILE_INFO tInfo = {};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(TILE_INFO), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		m_vecTile.push_back(new TILE_INFO(tInfo));
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CTerrain::CreateGraph()
{
	m_vecGraph.resize(TILE_COUNT_X * TILE_COUNT_Y);

	for (int i = 0; i < TILE_COUNT_Y; ++i)
	{
		for (int j = 0; j < TILE_COUNT_X; ++j)
		{
			int iIndex = i * TILE_COUNT_X + j;

			// 좌상단
			if ((0 != i) && !(0 == j && !(i % 2)))
			{
				if((i % 2) && (0 ==  m_vecTile[iIndex - TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILE_COUNT_X]);
				else if(!(i % 2) && (0 == m_vecTile[iIndex - (TILE_COUNT_X + 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TILE_COUNT_X + 1)]);
			}

			// 우상단
			if ((0 != i) && !(TILE_COUNT_X - 1 == j && (i % 2)))
			{
				if ((i % 2) && (0 == m_vecTile[iIndex - (TILE_COUNT_X - 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - (TILE_COUNT_X - 1)]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex - TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILE_COUNT_X]);
			}

			// 좌하단
			if ((TILE_COUNT_Y - 1 != i) && !(0 == j && !(i % 2)))
			{
				if ((i % 2) && (0 == m_vecTile[iIndex + TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TILE_COUNT_X]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex + (TILE_COUNT_X - 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TILE_COUNT_X - 1)]);
			}

			// 우하단
			if ((TILE_COUNT_Y - 1 != i) && !(TILE_COUNT_X - 1 == j && (i % 2)))
			{
				if ((i % 2) && (0 == m_vecTile[iIndex + (TILE_COUNT_X + 1)]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + (TILE_COUNT_X + 1)]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex + TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex + TILE_COUNT_X]);
			}
		}
	}

	return S_OK;
}

CTerrain* CTerrain::Create()
{
	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
