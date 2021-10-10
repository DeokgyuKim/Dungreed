#include "stdafx.h"
#include "Inventory.h"
#include "Sword.h"
#include "Gun.h"
#include "Spear.h"
#include "Stamp.h"
#include "Dart.h"
#include "Shield.h"
#include "BatItem.h"
#include "Wing.h"
#include "TimeRing.h"
#include "SpeedRing.h"

#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CInventory)

CInventory::CInventory()
{
	this->Initialize();
	m_iIndex = -1;
	m_vecItem.resize(20);
	m_iGold = 1100;
}


CInventory::~CInventory()
{
}

int CInventory::Update()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int iIndex = i * 5 + j;
			m_tItemRect[iIndex] = { (LONG)(m_tInfo.matWorld._41 + 10.f + j * 22.f),
				(LONG)(m_tInfo.matWorld._42 + 93.f + i * 22.f),
				(LONG)(m_tInfo.matWorld._41 + 10.f + j * 22.f + 19.f),
				(LONG)(m_tInfo.matWorld._42 + 93.f + i * 22.f + 19.f) };
		}
	}
	m_tItemRect[15] = { (LONG)(m_tInfo.matWorld._41) + 16, (LONG)(m_tInfo.matWorld._42) + 35,
		(LONG)(m_tInfo.matWorld._41) + 34, (LONG)(m_tInfo.matWorld._42) + 53 };
	m_tItemRect[16] = { (LONG)(m_tInfo.matWorld._41) + 70, (LONG)(m_tInfo.matWorld._42) + 35,
		(LONG)(m_tInfo.matWorld._41) + 88, (LONG)(m_tInfo.matWorld._42) + 53 };
	m_tItemRect[17] = { (LONG)(m_tInfo.matWorld._41) + 38, (LONG)(m_tInfo.matWorld._42) + 35,
		(LONG)(m_tInfo.matWorld._41) + 56, (LONG)(m_tInfo.matWorld._42) + 53 };
	m_tItemRect[18] = { (LONG)(m_tInfo.matWorld._41) + 92, (LONG)(m_tInfo.matWorld._42) + 35,
		(LONG)(m_tInfo.matWorld._41) + 110, (LONG)(m_tInfo.matWorld._42) + 53 };
	m_tItemRect[19] = { (LONG)(m_tInfo.matWorld._41) + 54, (LONG)(m_tInfo.matWorld._42) + 65,
		(LONG)(m_tInfo.matWorld._41) + 72, (LONG)(m_tInfo.matWorld._42) + 83 };
	if (CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON))
	{
		VEC3 vPos = CMouse::GetMousePos();
		POINT pt = { (LONG)vPos.x, (LONG)vPos.y - 10.f };
		for (int i = 0; i < 20; ++i)
		{
			if (m_vecItem[i] == nullptr)
				continue;
			if (PtInRect(&m_tItemRect[i], pt))
			{
				if(m_vecItem.size() > i)
				m_iIndex = i;
				break;
			}
		}
	}
	if (CKeyMgr::GetInstance()->KeyUp(KEY_LBUTTON))
	{
		if (m_iIndex != -1)
		{
			VEC3 vPos = CMouse::GetMousePos();
			POINT pt = { (LONG)vPos.x, (LONG)vPos.y - 10.f };
			bool bCheck = false;
			if (CObjectMgr::GetInstance()->GetShopOn())
			{
				RECT* pRect = CShopUi::GetInstance()->GetRect();
				for (int i = 0; i < 6; ++i)
				{
					if (PtInRect(&pRect[i], pt))
					{
						if (m_iIndex >= 15)
							break;
						//판매
						PlusGold(m_vecItem[m_iIndex]->Get_Info()->m_iGold);
						delete m_vecItem[m_iIndex];
						m_vecItem[m_iIndex] = nullptr;

						CSoundMgr::Get_Instance()->PlaySoundW(L"MyEquip.wav", CSoundMgr::CHANNELID::UI);
					}
				}
			}
			for (int i = 0; i < 20; ++i)
			{
				if (PtInRect(&m_tItemRect[i], pt))
				{
					if (m_iIndex == 15 || m_iIndex == 16)
					{
						if (m_vecItem[i] == nullptr)
							break;
					}
					if (m_vecItem[m_iIndex]->Get_Type() == OBJ::SUBITEM)
					{
						if (i == 15 || i == 16 || i == 19)
							break;
					}
					if (i == 17 || i == 18)
					{
						if (m_vecItem[m_iIndex]->Get_Type() != OBJ::SUBITEM)
							break;
					}
					if (m_vecItem[m_iIndex]->Get_Type() == OBJ::RING)
					{
						if (i == 15 || i == 16 || i == 17 || i == 18)
							break;
					}
					if (i == 19)
					{
						if (m_vecItem[m_iIndex]->Get_Type() != OBJ::RING)
							break;
					}
				/*	if (i == 17 || i == 18)
					{
						if (m_vecItem[m_iIndex]->Get_Type() != OBJ::SUBITEM)
							break;
					}
					if (m_iIndex == 17 || m_iIndex == 18)
					{
						if (m_vecItem[i] == nullptr)
							break;
						if (m_vecItem[i]->Get_Type() != OBJ::SUBITEM)
							break;
					}
					if (m_vecItem[m_iIndex]->Get_Type() == OBJ::SUBITEM)
					{
						if (i != 17 && i != 18)
							break;
					}*/
					//Change
					CObj* pTemp = m_vecItem[m_iIndex];
					m_vecItem[m_iIndex] = m_vecItem[i];
					m_vecItem[i] = pTemp;
					m_iIndex = -1;
					bCheck = true;
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyEquip.wav", CSoundMgr::CHANNELID::UI);
					break;
				}
			}
			if (!bCheck)
			{
				m_iIndex = -1;
			}
		}
	}
	
	return NO_EVENT;
}

void CInventory::LateUpdate()
{
}

void CInventory::Render()
{
	

	
}

void CInventory::Render(MATRIX* matParentWorld)
{
	m_tInfo.matWorld = *matParentWorld;
	MATRIX matTrans, matWorld;
	MATTRANSLATION(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matTrans * (*matParentWorld);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Inventory", &matWorld, L"InventoryBase", 0,
		255, RENDER::LEFTTOP);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int iIndex = i * 5 + j;
			MATTRANSLATION(&matTrans, 10.f + j * 22.f, 93.f + i * 22.f, 0.f);
			matWorld = matTrans * (*matParentWorld);
			POINT pt = { CMouse::GetMousePos().x, CMouse::GetMousePos().y - 10.f };
			if (!PtInRect(&m_tItemRect[iIndex], pt))
				CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"InventorySlot", &matWorld, L"ItemOff", 0,
					255, RENDER::LEFTTOP);
			if (m_vecItem[iIndex] == nullptr)
				continue;
			if (m_vecItem.size() > iIndex)
			{
				if (m_iIndex == iIndex)
				{
					MATTRANSLATION(&matTrans, CMouse::GetMousePos().x, 
						CMouse::GetMousePos().y - 10.f, 0.f);
					m_vecItem[iIndex]->Render(&matTrans, RENDER::MID);
				}
				else
				{
					//아이템출력
					MATTRANSLATION(&matTrans, 10.f + j * 22.f + 10.f, 93.f + i * 22.f + 10.f, 0.f);
					matWorld = matTrans * (*matParentWorld);
					m_vecItem[iIndex]->Render(&matWorld, RENDER::MID);
				}
			}
		}
	}
	MATTRANSLATION(&matTrans, 63.f, 74.f, 0.f);
	matWorld = matTrans * (*matParentWorld);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"InventorySlot", &matWorld, L"AtrifactSlot", 0,
		255, RENDER::MID);


	if (m_iIndex == 15)
	{
		MATTRANSLATION(&matTrans, CMouse::GetMousePos().x,
			CMouse::GetMousePos().y - 10.f, 0.f);
		m_vecItem[15]->Render(&matTrans, RENDER::MID);
	}
	else
	{
		MATTRANSLATION(&matTrans, 25.f, 44.f, 0.f);
		matWorld = matTrans * (*matParentWorld);
		if (m_vecItem[15] != nullptr)
			m_vecItem[15]->Render(&matWorld, RENDER::MID);
	}

	if (m_iIndex == 16)
	{
		MATTRANSLATION(&matTrans, CMouse::GetMousePos().x,
			CMouse::GetMousePos().y - 10.f, 0.f);
		m_vecItem[16]->Render(&matTrans, RENDER::MID);
	}
	else
	{
		MATTRANSLATION(&matTrans, 79.f, 44.f, 0.f);
		matWorld = matTrans * (*matParentWorld);
		if (m_vecItem[16] != nullptr)
			m_vecItem[16]->Render(&matWorld, RENDER::MID);
	}
	if (m_iIndex == 17)
	{
		MATTRANSLATION(&matTrans, CMouse::GetMousePos().x,
			CMouse::GetMousePos().y - 10.f, 0.f);
		m_vecItem[17]->Render(&matTrans, RENDER::MID);
	}
	else
	{
		MATTRANSLATION(&matTrans, 47.f, 44.f, 0.f);
		matWorld = matTrans * (*matParentWorld);
		if (m_vecItem[17] != nullptr)
			m_vecItem[17]->Render(&matWorld, RENDER::MID);
	}
	if (m_iIndex == 18)
	{
		MATTRANSLATION(&matTrans, CMouse::GetMousePos().x,
			CMouse::GetMousePos().y - 10.f, 0.f);
		m_vecItem[18]->Render(&matTrans, RENDER::MID);
	}
	else
	{
		MATTRANSLATION(&matTrans, 101.f, 44.f, 0.f);
		matWorld = matTrans * (*matParentWorld);
		if (m_vecItem[18] != nullptr)
			m_vecItem[18]->Render(&matWorld, RENDER::MID);
	}
	if (m_iIndex == 19)
	{
		MATTRANSLATION(&matTrans, CMouse::GetMousePos().x,
			CMouse::GetMousePos().y - 10.f, 0.f);
		m_vecItem[19]->Render(&matTrans, RENDER::MID);
	}
	else
	{
		MATTRANSLATION(&matTrans, 63.f, 74.f, 0.f);
		matWorld = matTrans * (*matParentWorld);
		if (m_vecItem[19] != nullptr)
			m_vecItem[19]->Render(&matWorld, RENDER::MID);
	}

	TCHAR szGold[MAX_STR];
	swprintf_s(szGold, L"%d", m_iGold);

	D3DXMatrixTranslation(&matTrans, 59.f, 170.f, 0.f);

	matWorld = matTrans * (*matParentWorld); 
	matWorld._41 *= 4.f;
	matWorld._42 *= 4.f;
	matWorld._43 *= 4.f;

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);

	// 다이렉트 폰트는 문자열 출력할 때마다 그 때 맞는 텍스처를 만들어서 출력한다. 겁나 느리다.
	CDeviceMgr::GetInstance()->GetFont()->DrawText(
		CDeviceMgr::GetInstance()->GetSprite(),
		szGold, /* 출력할 문자열 */
		lstrlen(szGold), /* 길이 */
		nullptr, /* RECT 포인터 */
		0,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int i = 0; i < 20; ++i)
	{
		CLineMgr::DrawBoxNoScroll(m_tItemRect[i]);
	}
}

CObj * CInventory::GetItem(int iIndex)
{
	return m_vecItem[iIndex];
}

int CInventory::GetIndex()
{
	for (int i = 0; i < 17; ++i)
	{
		if (m_vecItem[i] == nullptr)
			return i;
	}
	return -1;
}

HRESULT CInventory::Initialize()
{
	return S_OK;
}

void CInventory::Release()
{
}

