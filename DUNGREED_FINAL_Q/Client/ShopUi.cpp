#include "stdafx.h"
#include "ShopUi.h"
#include "Sword.h"
#include "Gun.h"
#include "Spear.h"
#include "Stamp.h"
#include "Dart.h"
#include "FireSword.h"

#include "BatItem.h"
#include "Wing.h"

#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CShopUi)

CShopUi::CShopUi()
{
	this->Initialize();
	m_iIndex = -1;
	m_vecItem.resize(6);
	m_vecItem[0] = CStamp::Create();
	m_vecItem[1] = CSpear::Create();
	m_vecItem[2] = CFireSword::Create();
	m_vecItem[3] = CBatItem::Create();
	m_vecItem[4] = CWing::Create();
	m_vecItem[5] = CGun::Create();
}


CShopUi::~CShopUi()
{
	Release();
}

int CShopUi::Update()
{
	for (int i = 0; i < 6; ++i)
	{
		m_tItemRect[i] = { 6, 23 * i + 30, 113, 23 * (i + 1) + 30 };
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_RBUTTON))
	{
		VEC3 vPos = CMouse::GetMousePos();
		POINT pt = { (LONG)vPos.x, (LONG)vPos.y - 10.f };
		for (int i = 0; i < 6; ++i)
		{
			if (m_vecItem[i] == nullptr)
				continue;
			if (PtInRect(&m_tItemRect[i], pt))
			{
				CObj* pObj = nullptr;
				switch (i)
				{
				case 0:
					pObj = CStamp::Create();
					break;
				case 1:
					pObj = CSpear::Create();
					break;
				case 2:
					pObj = CFireSword::Create();
					break;
				case 3:
					pObj = CBatItem::Create();
					break;
				case 4:
					pObj = CWing::Create();
					break;
				case 5:
					pObj = CGun::Create();
					break;
				default:
					break;
				}
				if (CInventory::GetInstance()->GetIndex() != -1 && CInventory::GetInstance()->GetGold() >= pObj->Get_Info()->m_iGold)
				{
					CInventory::GetInstance()->PlusGold(-pObj->Get_Info()->m_iGold);
					CInventory::GetInstance()->PushItem(CInventory::GetInstance()->GetIndex(), pObj);
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyEquip.wav", CSoundMgr::CHANNELID::UI);
				}
				else
					delete pObj;
				break;
			}
		}
	}
	CInventory::GetInstance()->Update();
	
	return NO_EVENT;
}

void CShopUi::LateUpdate()
{
}

void CShopUi::Render()
{
	MATRIX matTrans;
	MATTRANSLATION(&matTrans, 0.f, 0.f, 0.f);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Shop", &matTrans, L"ShopUI", 0,
		255, RENDER::LEFTTOP);

	for (int i = 0; i < 6; ++i)
	{
		MATTRANSLATION(&matTrans, 6.f, 23 * i + 30.f, 0.f);
		POINT pt = { CMouse::GetMousePos().x, CMouse::GetMousePos().y - 10.f };
		if (PtInRect(&m_tItemRect[i], pt))
			CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"ShopSelectItem", &matTrans, L"ShopItem_Selected", 0,
				255, RENDER::LEFTTOP);
		else
			CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"ShopSelectItem", &matTrans, L"ShopItem_UnSelected", 0,
				255, RENDER::LEFTTOP);

		//아이템 출력
		if (m_vecItem[i] != nullptr)
		{
			MATTRANSLATION(&matTrans, 16.f, 23 * i + 42.f, 0.f);
			m_vecItem[i]->Render(&matTrans, RENDER::MID);
			//CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_vecItem[i], &matTrans, L"ShopItem_Selected", 0,
			//	255, RENDER::LEFTTOP);
		}

		TCHAR szGold[MAX_STR];
		swprintf_s(szGold, L"%d", m_vecItem[i]->Get_Info()->m_iGold);

		MATTRANSLATION(&matTrans, 62.f, 23 * i + 42.f, 0.f);

		matTrans._41 *= 4.f;
		matTrans._42 *= 4.f;
		matTrans._43 *= 4.f;

		CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);

		// 다이렉트 폰트는 문자열 출력할 때마다 그 때 맞는 텍스처를 만들어서 출력한다. 겁나 느리다.
		CDeviceMgr::GetInstance()->GetFont()->DrawText(
			CDeviceMgr::GetInstance()->GetSprite(),
			szGold, /* 출력할 문자열 */
			lstrlen(szGold), /* 길이 */
			nullptr, /* RECT 포인터 */
			0,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	for (int i = 0; i < 6; ++i)
	{
		CLineMgr::DrawBoxNoScroll(m_tItemRect[i]);
	}
}

CObj * CShopUi::GetItem(int iIndex)
{
	return m_vecItem[iIndex];
}

HRESULT CShopUi::Initialize()
{
	return S_OK;
}

void CShopUi::Release()
{
	for(auto& pObj : m_vecItem)
	{
		if (pObj)
			delete pObj;
		pObj = nullptr;
	}
	m_vecItem.clear();
}

