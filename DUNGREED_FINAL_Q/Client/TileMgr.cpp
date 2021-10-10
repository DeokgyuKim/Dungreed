#include "stdafx.h"
#include "TileMgr.h"
#include "Obj.h"

IMPLEMENT_SINGLETON(CTileMgr)

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
	Release();
}

HRESULT CTileMgr::Initialize(TCHAR * pFilePath)
{
	if (pFilePath == nullptr)
	{
		for(int i = 0; i < TILE_COUNT_Y; ++i)
			for (int j = 0; j < TILE_COUNT_X; ++j)
			{
				TILE_INFO* pTile = new TILE_INFO;
				pTile->vPos = { FLOAT(j * TILECX), FLOAT(i * TILECY), 0.f };
				pTile->byDrawID = 0;
				pTile->byOption = 0;
				pTile->m_tRect = { (LONG)pTile->vPos.x - TILECX / 2, (LONG)pTile->vPos.y - TILECY / 2 ,
					(LONG)pTile->vPos.x + TILECX / 2 , (LONG)pTile->vPos.y - TILECY / 2 + 1 };
				m_vecTile.push_back(pTile);
			}
		CreateGraph();
	}
	else
	{
		this->LoadData(pFilePath);
		CreateGraph();
	}
	return S_OK;
}

HRESULT CTileMgr::CreateGraph()
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
				if ((i % 2) && (0 == m_vecTile[iIndex - TILE_COUNT_X]->byOption))
					m_vecGraph[iIndex].push_back(m_vecTile[iIndex - TILE_COUNT_X]);
				else if (!(i % 2) && (0 == m_vecTile[iIndex - (TILE_COUNT_X + 1)]->byOption))
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

void CTileMgr::Render()
{
	//for (auto pElement : m_vecTile)
	//{
	//	MATRIX matWorld;
	//	MATTRANSLATION(&matWorld, pElement->vPos.x, pElement->vPos.y, pElement->vPos.z);
	//	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Terrain", &matWorld, L"ImgTile", pElement->byDrawID);
	//}

	CObj* pPlayer = CObjectMgr::GetInstance()->GetPlayer();
	VEC3 vPos = CScrollMgr::Get_Scroll();//pPlayer->Get_Info()->vPos;
	
	int iCullX = (int)(vPos.x / TILECX);
	int iCullY = (int)(vPos.y / TILECY);
	
	//컬링 후 출력 개수
	const int iCullSizeX = WINCX / TILECX + 2;
	const int iCullSizeY = WINCY / TILECY + 3;
	
	
	int iCullXMin = iCullX;
	if (iCullXMin < 0)
		iCullXMin = 0;
	int iCullXMax = iCullX + iCullSizeX;
	if (iCullXMax >= TILE_COUNT_X)
		iCullXMax = TILE_COUNT_X;
	
	int iCullYMin = iCullY;
	if (iCullYMin < 0)
		iCullYMin = 0;
	int iCullYMax = iCullY + iCullSizeY;
	if (iCullYMax >= TILE_COUNT_Y)
		iCullYMax = TILE_COUNT_Y;
	
	for (int i = iCullYMin; i < iCullYMax; ++i)
	{
		for (int j = iCullXMin; j < iCullXMax; ++j)
		{
			int iIndex = i * TILE_COUNT_X + j;
			MATRIX matWorld;
	
			VEC3 vTilePos = m_vecTile[iIndex]->vPos;
			vTilePos -= CScrollMgr::Get_Scroll();
	
			MATTRANSLATION(&matWorld, vTilePos.x, vTilePos.y, vTilePos.z);
			if (m_vecTile[iIndex]->byDrawID != 0)
				CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Terrain", &matWorld, L"ImgTile", m_vecTile[iIndex]->byDrawID);
			//if (m_vecTile[iIndex]->byOption == 1)
			//{
			//	MATRIX matScale;
			//	MATSCAILING(&matScale, 0.25f, 0.25f, 1.f);
			//	matWorld = matScale * matWorld;
			//	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Terrain", &matWorld, L"OptionTile", 1);
			//}
			//if (m_vecTile[iIndex]->byOption == 2)
			//{
			//	MATRIX matScale;
			//	MATSCAILING(&matScale, 0.25f, 0.25f, 1.f);
			//	matWorld = matScale * matWorld;
			//	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Terrain", &matWorld, L"OptionTile", 2);
			//}
		}
	}
	

}

void CTileMgr::Release()
{
	for (auto pElement : m_vecTile)
		delete pElement;
	m_vecTile.clear();
}

HRESULT CTileMgr::LoadData(const TCHAR* pFilePath)
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
		TILE_INFO* p = new TILE_INFO(tInfo);
		m_vecTile.push_back(p);
	}

	CloseHandle(hFile);

	return S_OK;
}

bool CTileMgr::IsCollision(VEC3 vPos, RECT & rc)
{
	int iCullX = (int)(vPos.x / TILECX);
	int iCullY = (int)(vPos.y / TILECY) + 1;

	const int iCullSizeX = 2;
	const int iCullSizeY = 2;


	int iCullXMin = iCullX - iCullSizeX / 2;
	if (iCullXMin < 0)
		iCullXMin = 0;
	int iCullXMax = iCullX + iCullSizeX / 2;
	if (iCullXMax >= TILE_COUNT_X)
		iCullXMax = TILE_COUNT_X - 1;

	int iCullYMin = iCullY;
	if (iCullYMin < 0)
		iCullYMin = 0;
	int iCullYMax = iCullY + iCullSizeY / 2;
	if (iCullYMax >= TILE_COUNT_Y)
		iCullYMax = TILE_COUNT_Y - 1;
	
	for (int i = iCullYMin; i < iCullYMax; ++i)
	{
		for (int j = iCullXMin; j < iCullXMax; ++j)
		{
			int iIndex = i * TILE_COUNT_X + j;
			RECT temp = {};
			if (m_vecTile[iIndex]->byOption == 1 && IntersectRect(&temp, &rc, &m_vecTile[iIndex]->m_tRect))
			{
				return true;
			}
		}
	}
	return false;
}

int CTileMgr::Get_TileIndex(VEC3 vPos)
{
	int iCullX = (int)(vPos.x / TILECX);
	int iCullY = (int)(vPos.y / TILECY);

	return iCullY * TILE_COUNT_X + iCullX;
}
