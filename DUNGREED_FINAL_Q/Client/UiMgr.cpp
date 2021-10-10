#include "stdafx.h"
#include "UiMgr.h"
#include "Player.h"
#include "Inventory.h"

IMPLEMENT_SINGLETON(CUiMgr)

CUiMgr::CUiMgr()
	: m_pInven(CInventory::GetInstance())
{
	SetFrame(m_tHpBarWaveFrame, L"HpBar", L"LifeWave", 0.f, 7.f);
}


CUiMgr::~CUiMgr()
{
}

void CUiMgr::RenderUi()
{
	m_pBoss = CObjectMgr::GetInstance()->GetBoss();
	if (m_pBoss != nullptr)
		RenderBossUi();

	RenderHpBar();
	RenderDashBar();
	RenderDamagedEffect();
	
}

void CUiMgr::RenderHpBar()
{
	MATSCAILING(&matScale, 1.f, 1.f, 1.f);
	MATTRANSLATION(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matScale * matTrans;

	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"HpBar", &matWorld, L"PlayerLifeBack", 0, 255, RENDER::LEFTTOP);

	MATSCAILING(&matScale, (float)m_pPlayer->Get_Info()->iHp / (float)PLAYER_HP, 1.f, 1.f);
	matWorld = matScale * matTrans;

	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"HpBar", &matWorld, L"LifeBar", 0, 255, RENDER::LEFTTOP);

	MATSCAILING(&matScale, 1.f, 1.f, 1.f);
	MATTRANSLATION(&matTrans, (float)m_pPlayer->Get_Info()->iHp - 5.f, 3.f, 0.f);
	matWorld = matScale * matTrans;

	FrameUpdate(m_tHpBarWaveFrame);

	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tHpBarWaveFrame.pObjectKey, &matWorld, m_tHpBarWaveFrame.pStateKey,
		(int)m_tHpBarWaveFrame.fStart, 255, RENDER::LEFTTOP);

	MATSCAILING(&matScale, 1.f, 1.f, 1.f);
	MATTRANSLATION(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matScale * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"HpBar", &matWorld, L"PlayerLifeBase", 0, 255, RENDER::LEFTTOP);
}

void CUiMgr::RenderDashBar()
{
	MATSCAILING(&matScale, 1.f, 1.f, 1.f);
	MATTRANSLATION(&matTrans, 5.f, 18.f, 0.f);
	matWorld = matScale * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"DashBar", &matWorld, L"DashCountBase", 0, 255, RENDER::LEFTTOP);

	int iDashGauge = m_pPlayer->Get_DashCount();

	for (int i = 0; i < iDashGauge; ++i)
	{
		MATTRANSLATION(&matTrans, 10.f * i + 7.f, 20.f, 0.f);
		matWorld = matScale * matTrans;
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"DashBar", &matWorld, L"DashCount", 0, 255, RENDER::LEFTTOP);
	}
}

void CUiMgr::RenderDamagedEffect()
{
	if (m_pPlayer->Get_DamagedRenderUi())
	{
		MATSCAILING(&matScale, WINCX / 400.f, WINCY / 300.f, 1.f);
		MATTRANSLATION(&matTrans, 0.f, 0.f, 0.f);
		matWorld = matScale * matTrans;
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"UI", &matWorld, L"NormalHit", 0, 255, RENDER::LEFTTOP);
	}
}

void CUiMgr::RenderBossUi()
{
	MATSCAILING(&matScale, (95.f / 74.f) * ((float)m_pBoss->Get_Info()->iHp / (float)BOSS_HP), (25.f / 16.f), 1.f);
	MATTRANSLATION(&matTrans, WINCX * 0.5f - 35.f, 148.f, 0.f);
	matWorld = matScale * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"HpBar", &matWorld, L"LifeBar", 0, 255, RENDER::LEFTTOP);
	
	
	MATTRANSLATION(&matTrans, WINCX * 0.5f - 58.f, 150.f, 0.f);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Boss", &matTrans, L"HpBarBase", 0, 255, RENDER::LEFTTOP);
}

void CUiMgr::RenderInventroy()
{
	MATTRANSLATION(&matTrans, WINCX - 123.f, 0.f, 0.f);
	m_pInven->Render(&matTrans);
}
