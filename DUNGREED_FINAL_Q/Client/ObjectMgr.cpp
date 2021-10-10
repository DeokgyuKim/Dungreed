#include "stdafx.h"
#include "ObjectMgr.h"
#include "Obj.h"
#include "Niflheim.h"
#include "NiflheimState.h"
#include "MeleeMonster.h"
#include "RangedMonster.h"
#include "HitBox.h"

#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
	: m_bReverse(false)
	, m_bInven(false)
	, m_bShop(false)
	, m_bClear(false)
{
}


CObjectMgr::~CObjectMgr()
{
	Release();
}

CObj* CObjectMgr::GetPlayer()
{
	if (m_ObjectList[OBJ_PLAYER].empty())
		return nullptr;

	return CInputMgr::GetInstance()->Get_Actor();
}

CObj * CObjectMgr::GetBoss()
{
	if (m_ObjectList[OBJ_BOSS].empty())
		return nullptr;

	return m_ObjectList[OBJ_BOSS].front();
}

CObj * CObjectMgr::GetTarget(VEC3 vPos)
{
	float	fLength = 99999999;
	CObj*	pObj = nullptr;

	if (m_ObjectList[OBJ_MONSTER].empty() && m_ObjectList[OBJ_ICEPILLAR].empty() && m_ObjectList[OBJ_BOSS].empty())
		return nullptr;

	for (auto& Monster : m_ObjectList[OBJ_MONSTER])
	{
		if (fLength > VEC3LENGTH(&(vPos - Monster->Get_Info()->vPos)))
		{
			fLength = VEC3LENGTH(&(vPos - Monster->Get_Info()->vPos));
			pObj = Monster;
		}
	}
	for (auto& IcePillar : m_ObjectList[OBJ_ICEPILLAR])
	{
		if (fLength > VEC3LENGTH(&(vPos - IcePillar->Get_Info()->vPos)))
		{
			fLength = VEC3LENGTH(&(vPos - IcePillar->Get_Info()->vPos));
			pObj = IcePillar;
		}
	}
	for (auto& Boss : m_ObjectList[OBJ_BOSS])
	{
		if (fLength > VEC3LENGTH(&(vPos - Boss->Get_Info()->vPos)))
		{
			fLength = VEC3LENGTH(&(vPos - Boss->Get_Info()->vPos));
			pObj = Boss;
		}
	}

	return pObj;
}

int CObjectMgr::CountMonster(MONSTER::TYPE eType, MELEE_MONSTER::TYPE eMelType, RANGED_MONSTER::TYPE eRanType)
{
	int iCount = 0;
	for (auto& pObj : m_ObjectList[OBJ_MONSTER])
	{
		if (SCAST(CMonster*, pObj)->Get_MonType() == eType)
		{
			if (eType == MONSTER::MELEE_MONSTER)
			{
				if (SCAST(CMeleeMonster*, pObj)->Get_MeleeMonType() == eMelType)
					++iCount;
			}
			else if (eType == MONSTER::RANGED_MONSTER)
			{
				if (SCAST(CRangedMonster*, pObj)->Get_RangedType() == eRanType)
					++iCount;
			}
		}
	}
	return iCount;
}

int CObjectMgr::CountHitBox(HITBOX::TYPE eType)
{
	int iCount = 0;

	for (auto& pObj : m_ObjectList[OBJ_PLAYER_HITBOX])
	{
		if (SCAST(CHitBox*, pObj)->Get_HitBoxType() == eType)
			++iCount;
	}

	return iCount;
}

void CObjectMgr::DeleteLst(OBJECT_TYPE eType)
{
	for (auto pObj : m_ObjectList[eType])
	{
		if (pObj)
			delete pObj;
		pObj = nullptr;
	}
	m_ObjectList[eType].clear();
}

void CObjectMgr::ClearLst(OBJECT_TYPE eType)
{
	m_ObjectList[eType].clear();
}

HRESULT CObjectMgr::AddObject(OBJECT_TYPE eType, CObj* pObject)
{
	NULL_CHECK_RETURN(pObject, E_FAIL);
	m_ObjectList[eType].push_back(pObject);

	return S_OK;
}

void CObjectMgr::Update()
{
	iCount = 0;
	CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_PLAYER], &m_ObjectList[OBJ_SHOP]);
	if (CKeyMgr::GetInstance()->KeyDown(KEY_I))
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"MyOpenInventory.wav", CSoundMgr::CHANNELID::UI);
		m_bInven = !m_bInven;
	}
	if (m_bShop)
		CShopUi::GetInstance()->Update();
	else if (m_bInven)
		CInventory::GetInstance()->Update();
	else
	{
		for (int i = 0; i < OBJECT_END; ++i)
		{
			if (i == OBJ_ICEPILLAR)
				continue;
			OBJECT_LIST::iterator iter_begin = m_ObjectList[i].begin();
			OBJECT_LIST::iterator iter_end = m_ObjectList[i].end();

			for (; iter_begin != iter_end;)
			{
				//if ((*iter_begin)->Get_Info()->bDead)
				//{
				//	++iter_begin;
				//	continue;
				//}
				++iCount;
				int iEvent = 0;
				if (!m_bReverse || i == OBJ_EFFECT)
					iEvent = (*iter_begin)->Update();
				else
				{
					if (i == OBJ_PLAYER && (*iter_begin) != GetPlayer())
					{

					}
					else
						iEvent = (*iter_begin)->ReverseUpdate();
				}

				if (DEAD_OBJ == iEvent)
				{
					SafeDelete(*iter_begin);
					iter_begin = m_ObjectList[i].erase(iter_begin);
				}
				else if (REVERSE_END == iEvent)
				{
					m_bReverse = false;
				}
				else if (DEAD_BOSS == iEvent)
				{
					DestroyWindow(g_hWnd);
				}
				else
					++iter_begin;
			}
		}
	}
}

void CObjectMgr::LateUpdate()
{
	if (!m_bInven)
	{
		CCollisionMgr::Collision_Tile(&m_ObjectList[OBJ_PLAYER]);
		for (int i = 0; i < OBJECT_END; ++i)
		{
			if (i == OBJ_ICEPILLAR)
				continue;
			OBJECT_LIST::iterator iter_begin = m_ObjectList[i].begin();
			OBJECT_LIST::iterator iter_end = m_ObjectList[i].end();

			for (; iter_begin != iter_end; ++iter_begin)
			{
				if (!m_bReverse)
					(*iter_begin)->LateUpdate();
				else
					(*iter_begin)->ReverseLateUpdate();
			}
		}
		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_MONSTER], &m_ObjectList[OBJ_PLAYER_HITBOX]);
		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_ICEPILLAR], &m_ObjectList[OBJ_PLAYER_HITBOX]);
		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_BOSS], &m_ObjectList[OBJ_PLAYER_HITBOX]);
		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_MONSTER], &m_ObjectList[OBJ_PLAYER]);
		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_ICEPILLAR], &m_ObjectList[OBJ_PLAYER]);
		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_BOSS], &m_ObjectList[OBJ_PLAYER]);

		CCollisionMgr::Collision_Rect(&m_ObjectList[OBJ_PLAYER], &m_ObjectList[OBJ_MONSTER_HITBOX]);

		CCollisionMgr::Collision_Player(&m_ObjectList[OBJ_PLAYER]);
	}
}

void CObjectMgr::Render(bool bPlayerRender)
{
	MATRIX matTrans;
	MATTRANSLATION(&matTrans, 0.f, -CScrollMgr::Get_Scroll().y / (TILECY * TILE_COUNT_Y) * 80.f, 0.f);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Terrain", &matTrans, L"Map", 1, 255, RENDER::LEFTTOP);

	MATTRANSLATION(&matTrans, 0.f, -CScrollMgr::Get_Scroll().y / (TILECY * TILE_COUNT_Y) * 170.f, 0.f);
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"Terrain", &matTrans, L"Map", 2, 255, RENDER::LEFTTOP);
	CTileMgr::GetInstance()->Render();
	for (int i = 0; i < OBJECT_END; ++i)
	{
		
		if (i == OBJ_ICEPILLAR)
			continue;
		OBJECT_LIST::iterator iter_begin = m_ObjectList[i].begin();
		OBJECT_LIST::iterator iter_end = m_ObjectList[i].end();

		for (; iter_begin != iter_end; ++iter_begin)
		{
			if (i == OBJ_PLAYER && bPlayerRender == false)
			{
				if ((*iter_begin) == GetPlayer())
					continue;
			}
			(*iter_begin)->Render();
		}
	}
	CUiMgr::GetInstance()->RenderUi();
	if (m_bShop)
		CShopUi::GetInstance()->Render();
	if (m_bInven)
		CUiMgr::GetInstance()->RenderInventroy();
}

void CObjectMgr::Release()
{
	for (int i = 0; i < OBJECT_END; ++i)
	{
		for_each(m_ObjectList[i].begin(), m_ObjectList[i].end(), SafeDelete<CObj*>);
		m_ObjectList[i].clear();
	}
}
