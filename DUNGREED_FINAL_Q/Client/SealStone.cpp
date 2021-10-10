#include "stdafx.h"
#include "SealStone.h"
#include "SoundMgr.h"

CSealStone::CSealStone()
{
}


CSealStone::~CSealStone()
{
}

int CSealStone::Update()
{
	return NO_EVENT;
}

void CSealStone::LateUpdate()
{
	CObj::UpdateRect();
	CObj::FrameUpdate();
}

void CSealStone::Render()
{
	//matWorld
	MATRIX	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale *  matTrans;
	
	if (m_bOn)
	{
		//ÅØ½ºÃÄ ·»´õ
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
	}
	else
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 100);
}

HRESULT CSealStone::Initialize()
{
	m_eType = OBJ::TILE;

	ZeroMemory(&m_tInfo, sizeof(m_tInfo));

	m_tInfo.vSize = { 32.f, 32.f, 0.f };
	m_tInfo.bDead = false;

	Set_Frame(L"SealStone", L"SealStoneIdle", 0.f, 8.f);

	return S_OK;
}

void CSealStone::Release()
{
}

void CSealStone::OnSeal()
{
	m_bOn = true;
	CSoundMgr::Get_Instance()->PlaySoundW(L"stoneDoor-sharedassets3.assets-344.wav", CSoundMgr::CHANNELID::UI);
	int Index = CTileMgr::GetInstance()->Get_TileIndex(m_tInfo.vPos + VEC3(3.f, -3.f, 0.f));
	CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X]->byOption = 2;
	CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X - 1]->byOption = 2;
	CTileMgr::GetInstance()->Get_Tile()[Index]->byOption = 2;
	CTileMgr::GetInstance()->Get_Tile()[Index - 1]->byOption = 2;

	CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X]->m_tRect.bottom = CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X]->m_tRect.top + 16;
	CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X - 1]->m_tRect.bottom = CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X - 1]->m_tRect.top + 16;
	CTileMgr::GetInstance()->Get_Tile()[Index]->m_tRect.bottom = CTileMgr::GetInstance()->Get_Tile()[Index]->m_tRect.top + 16;
	CTileMgr::GetInstance()->Get_Tile()[Index - 1]->m_tRect.bottom = CTileMgr::GetInstance()->Get_Tile()[Index - 1]->m_tRect.top + 16;

	CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X]->byDrawID = 17;
	CTileMgr::GetInstance()->Get_Tile()[Index - TILE_COUNT_X - 1]->byDrawID = 17;
	CTileMgr::GetInstance()->Get_Tile()[Index]->byDrawID = 17;
	CTileMgr::GetInstance()->Get_Tile()[Index - 1]->byDrawID = 17;
}



CSealStone * CSealStone::Create(VEC3 vPos)
{
	CSealStone* pInstance = new CSealStone;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	
	//pInstance->m_tInfo.vPos -= VEC3(0.f, -16.f, 0.f);


	


	return pInstance;
}
