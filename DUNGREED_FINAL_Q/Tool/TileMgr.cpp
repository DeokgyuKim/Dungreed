#include "stdafx.h"
#include "TileMgr.h"
#include "ToolView.h"
#include "MainFrm.h"

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
		CTextureMgr::GetInstance()->Load_Texture(CDeviceMgr::GetInstance()->GetDevice(),
			L"../Resource/Texture/Stage/Terrain/ImgTile/ImgTile%d.png", MULTI_TEXTURE, L"Tile", L"Tile", 40);
		CTextureMgr::GetInstance()->Load_Texture(CDeviceMgr::GetInstance()->GetDevice(),
			L"../Resource/Texture/Stage/Terrain/OptionTile/OptionTile%d.png", MULTI_TEXTURE, L"Option", L"Tile", 3);
		for(int i = 0; i < TILE_COUNT_Y; ++i)
			for (int j = 0; j < TILE_COUNT_X; ++j)
			{
				TILE_INFO* pTile = new TILE_INFO;
				pTile->vPos = { FLOAT(j * TILECX) + TILECX * 0.5f, FLOAT(i * TILECY) + TILECY, 0.f };
				pTile->byDrawID = 0;
				pTile->byOption = 0;
				pTile->m_tRect = { (LONG)pTile->vPos.x - TILECX / 2, (LONG)pTile->vPos.y - TILECY ,
					(LONG)pTile->vPos.x + TILECX / 2 , (LONG)pTile->vPos.y - TILECY + 1 };
				m_vecTile.push_back(pTile);
			}
		CreateGraph();
	}
	else
	{

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
	//	CTextureMgr::Get_Instance()->Render(TEXTURE::MULTI_TEXTURE, L"Tile", &matWorld, L"Tile", pElement->iDrawID);
	//}

	//CObj* pPlayer = CObjectMgr::GetInstance()->GetPlayer();
	VEC3 vPos = { m_pView->GetScrollPos(0) + WINCX_TOOL * 0.5f, m_pView->GetScrollPos(1) + WINCY_TOOL * 0.5f, 0 };

	int iCullX = (int)(vPos.x / TILECX);
	int iCullY = (int)(vPos.y / TILECY);

	//컬링 후 출력 개수
	const int iCullSizeX = WINCX_TOOL / TILECX + 2;
	const int iCullSizeY = WINCY_TOOL / TILECY + 3;


	int iCullXMin = iCullX - iCullSizeX / 2;
	if (iCullXMin < 0)
		iCullXMin = 0;
	int iCullXMax = iCullX + iCullSizeX / 2;
	if (iCullXMax >= TILE_COUNT_X)
		iCullXMax = TILE_COUNT_X;

	int iCullYMin = iCullY - iCullSizeY / 2;
	if (iCullYMin < 0)
		iCullYMin = 0;
	int iCullYMax = iCullY + iCullSizeY / 2;
	if (iCullYMax >= TILE_COUNT_Y)
		iCullYMax = TILE_COUNT_Y;

	for (int i = iCullYMin; i < iCullYMax; ++i)
	{
		for (int j = iCullXMin; j < iCullXMax; ++j)
		{
			int iIndex = i * TILE_COUNT_X + j;
			MATRIX matWorld;

			VEC3 vTilePos = m_vecTile[iIndex]->vPos;
			vTilePos.x -= m_pView->GetScrollPos(0);
			vTilePos.y -= m_pView->GetScrollPos(1);

			MATTRANSLATION(&matWorld, vTilePos.x, vTilePos.y, vTilePos.z);
			CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Tile", &matWorld, L"Tile", m_vecTile[iIndex]->byDrawID);
			if (m_vecTile[iIndex]->byOption == 1)
			{
				MATRIX matScale;
				MATSCAILING(&matScale, 0.25f, 0.25f, 1.f);
				matWorld = matScale * matWorld;
				CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Option", &matWorld, L"Tile", 1);
			}
			if (m_vecTile[iIndex]->byOption == 2)
			{
				MATRIX matScale;
				MATSCAILING(&matScale, 0.25f, 0.25f, 1.f);
				matWorld = matScale * matWorld;
				CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Option", &matWorld, L"Tile", 2);
			}
		}
	}
	TCHAR szIndex[MIN_STR] = L"";
	MATRIX matWorld;
	MATTRANSLATION(&matWorld, 0.f, 0.f, 0.f);
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	swprintf_s(szIndex, L"%d(%d, %d)",Get_TileIndex(VEC3((float)m_vPt.x, (float)m_vPt.y, 0.f)), m_vPt.x, m_vPt.y);
	CDeviceMgr::GetInstance()->GetFont()->DrawText(CDeviceMgr::GetInstance()->GetSprite(), szIndex, lstrlen(szIndex),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CTileMgr::MiniRender()
{
	for (auto& pTile : m_vecTile)
	{
		MATRIX matTrans, matScale, matWorld;
		MATTRANSLATION(&matTrans, pTile->vPos.x, pTile->vPos.y, pTile->vPos.z);
		MATSCAILING(&matScale, 0.2f, 0.2f, 1.f);
		matWorld = matTrans * matScale;

		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Tile", &matWorld, L"Tile", pTile->byDrawID);
	}
}

void CTileMgr::Release()
{
	for (auto pElement : m_vecTile)
		delete pElement;
	m_vecTile.clear();
}

HRESULT CTileMgr::SaveData(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwBytes = 0;

	for (size_t i = 0; i < m_vecTile.size(); ++i)
		WriteFile(hFile, m_vecTile[i], sizeof(TILE_INFO), &dwBytes, nullptr);

	CloseHandle(hFile);

	return S_OK;
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

		m_vecTile.push_back(new TILE_INFO(tInfo));
	}

	CloseHandle(hFile);

	return S_OK;
}

void CTileMgr::TileChange(VEC3 vPos, BYTE byDrawID, BYTE byOption)
{
	int Index = Get_TileIndex(vPos);
	if (Index == -1)
		return;
	if(byDrawID != 255)
		m_vecTile[Index]->byDrawID = byDrawID;
	m_vecTile[Index]->byOption = byOption;
	if (byOption == 2)
	{
		m_vecTile[Index]->m_tRect.right = m_vecTile[Index]->m_tRect.left + 16;
		m_vecTile[Index]->m_tRect.bottom = m_vecTile[Index]->m_tRect.top + 16;
	}
}

int CTileMgr::Get_TileIndex(VEC3 vPos)
{
	int iCullX = (int)(vPos.x / TILECX);
	int iCullY = (int)(vPos.y / TILECY);

	if (iCullX >= TILE_COUNT_X || iCullY >= TILE_COUNT_Y)
		return -1;

	return iCullY * TILE_COUNT_X + iCullX;
}
