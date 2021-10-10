#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Weapon.h"
#include "Monster.h"
#include "MeleeMonster.h"
#include "SkullDog.h"
#include "SkullDogState.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Sphere(list<CObj*>* pDst, list<CObj*>* pSrc)
{
	for (auto& Dst : *pDst)
	{
		for (auto& Src : *pSrc)
		{
			if (CheckSphere(Dst->Get_Info(), Src->Get_Info()))
			{
				vector<void*> ValueDst;
				vector<void*> ValueSrc;
				switch (Dst->Get_Type())
				{
				case OBJ::MONSTER:
					switch (Src->Get_Type())
					{
					case OBJ::HITBOX:
						ValueDst.push_back((void*)Src->Get_Info()->iAtt);

						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
				Dst->Collision(Src->Get_Type(), &ValueDst);
				Src->Collision(Dst->Get_Type(), &ValueSrc);
			}
		}
	}
}

void CCollisionMgr::Collision_Rect(list<CObj*>* pDst, list<CObj*>* pSrc)
{
	RECT rc = {};
	if (pDst->empty() || pSrc->empty())
		return;
	for (auto& Dst : *pDst)
	{
		for (auto& Src : *pSrc)
		{
			if (Dst->Get_Type() == OBJ::ICE_PILLAR)
			{
				if (Src->Get_Type() == OBJ::PLAYER)
				{
					if (!SCAST(CPlayer*, Src)->Get_Dash())
						break;
				}
				const RECT* pRect = Dst->Get_Rect();
				for (int i = 0; i < 3; ++i)
				{
					if (IntersectRect(&rc, &pRect[i], Src->Get_Rect()))
					{
						vector<void*> ValueDst;
						vector<void*> ValueSrc;
						int iAtt = 5;
						switch (Dst->Get_Type())
						{
						case OBJ::ICE_PILLAR:
							switch (Src->Get_Type())
							{
							case OBJ::HITBOX:
								ValueDst.push_back((void*)Src->Get_Info()->iAtt);
								ValueDst.push_back((void*)&Src->Get_Info()->vDir);
								break;
							case OBJ::PLAYER:
								if (SCAST(CPlayer*, Src)->GetWeapon()->Get_Type() == OBJ::SPEAR)
									ValueDst.push_back((void*)iAtt);
								else
									return;
								break;
							default:
								break;
							}
							break;
						default:
							break;
						}
						Dst->Collision(Src->Get_Type(), &ValueDst);
						Src->Collision(Dst->Get_Type(), &ValueSrc);
						break;
					}
				}
			}
			else
			{
				if (Dst->Get_Type() == OBJ::MONSTER)
				{
					if (SCAST(CMonster*, Dst)->Get_MonType() == MONSTER::TYPE::MELEE_MONSTER)
					{
						if (SCAST(CMeleeMonster*, Dst)->Get_MeleeMonType() == MELEE_MONSTER::TYPE::SKULLDOG)
						{
							if (SCAST(CSkullDog*, Dst)->Get_FSM()->GetCurrentState() == CSkullDogAttack::Instance())
							{
								if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
								{
									vector<void*> ValueDst;
									vector<void*> ValueSrc;
									switch (Src->Get_Type())
									{
									case OBJ::PLAYER:
										ValueSrc.push_back((void*)Dst->Get_Info()->iAtt);
										Dst->Set_Att(0);
										break;
									default:
										break;
									}
									//Dst->Collision(Src->Get_Type(), &ValueDst);
									Src->Collision(Dst->Get_Type(), &ValueSrc);
								}
							}
						}
					}
				}
				if (Src->Get_Type() == OBJ::PLAYER)
				{
					if (!SCAST(CPlayer*, Src)->Get_Dash())
						break;
				}
				if (IntersectRect(&rc, Dst->Get_Rect(), Src->Get_Rect()))
				{
					vector<void*> ValueDst;
					vector<void*> ValueSrc;
					int iAtt = 5;
					switch (Dst->Get_Type())
					{
					case OBJ::MONSTER:
						switch (Src->Get_Type())
						{
						case OBJ::HITBOX:
							ValueDst.push_back((void*)Src->Get_Info()->iAtt);
							ValueDst.push_back((void*)&Src->Get_Info()->vDir);
							break;
						case OBJ::PLAYER:
							if (SCAST(CPlayer*, Src)->GetWeapon()->Get_Type() == OBJ::SPEAR)
								ValueDst.push_back((void*)iAtt);
							else
								return;
							break;
						default:
							break;
						}
						break;
					case OBJ::PLAYER:
						switch (Src->Get_Type())
						{
						case OBJ::HITBOX:
							ValueDst.push_back((void*)Src->Get_Info()->iAtt);
							break;
						case OBJ::DART:
							rc = {};
							break;
						case OBJ::SHOP:
							if(CKeyMgr::GetInstance()->KeyDown(KEY_E))
									CObjectMgr::GetInstance()->ShowShop();
							return;
							break;
						default:
							break;
						}
					default:
						break;
					}
					Dst->Collision(Src->Get_Type(), &ValueDst);
					Src->Collision(Dst->Get_Type(), &ValueSrc);
				}
			}
		}
	}
}

void CCollisionMgr::Collision_SphereRect(list<CObj*>* pDst, list<CObj*>* pSrc)
{
	for (auto& Dst : *pDst)
	{
		for (auto& Src : *pSrc)
		{
			if (CheckSphereRect(Dst->Get_Info(), Src->Get_Rect()))
			{
				Dst->Collision(Src->Get_Type(), nullptr);
				Src->Collision(Dst->Get_Type(), nullptr);
			}
		}
	}
}

void CCollisionMgr::Collision_Tile(list<CObj*>* pDst)
{
	if (pDst->empty())
		return;
	vector<TILE_INFO*> Tile = CTileMgr::GetInstance()->Get_Tile();
	if (Tile.empty())
		return;
	for (auto& Dst : *pDst)
	{
		int iCullX = (int)(Dst->Get_Info()->vPos.x / TILECX);
		int iCullY = (int)(Dst->Get_Info()->vPos.y / TILECY);

		const int iCullSizeX = 1;
		const int iCullSizeY = 1;


		int iCullXMin = iCullX - iCullSizeX;
		if (iCullXMin < 0)
			iCullXMin = 0;
		int iCullXMax = iCullX + iCullSizeX;
		if (iCullXMax >= TILE_COUNT_X)
			iCullXMax = TILE_COUNT_X - 1;

		int iCullYMin = iCullY - iCullSizeY;
		if (iCullYMin < 0)
			iCullYMin = 0;
		int iCullYMax = iCullY + iCullSizeY;
		if (iCullYMax >= TILE_COUNT_Y)
			iCullYMax = TILE_COUNT_Y - 1;

		for (int i = iCullYMin; i <= iCullYMax; ++i)
		{
			for (int j = iCullXMin; j <= iCullXMax; ++j)
			{
				int Index = i * TILE_COUNT_X + j;
				if (Tile[Index]->byOption == 0)
					continue;
				RECT rc = {};
				if (Tile[Index]->byOption == 1)
				{
					if (Dst->Get_Type() == OBJ::PLAYER)
						if (SCAST(CPlayer*, Dst)->GetState() == CPlayer::PLAYER_STATE::DASH)
							break;
					if (SCAST(CPlayer*, Dst)->Get_NotCollision() == false && IntersectRect(&rc, Dst->Get_Rect(), &Tile[Index]->m_tRect))
					{
						vector<void*> Value;
						Value.push_back((void*)&rc);
						Value.push_back((void*)Tile[Index]->vPos);
						Value.push_back((void*)Tile[Index]->byOption);
						Dst->Collision(OBJ::TERRAIN, &Value);
					}
				}
				else if (Tile[Index]->byOption == 2)
				{
					if (IntersectRect(&rc, Dst->Get_Rect(), &Tile[Index]->m_tRect))
					{
						vector<void*> Value;
						Value.push_back((void*)&rc);
						Value.push_back((void*)Tile[Index]->vPos);
						Value.push_back((void*)Tile[Index]->byOption);
						Dst->Collision(OBJ::TERRAIN, &Value);
					}
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Player(list<CObj*>* pDst)
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_E))
	{
		for (auto& Dst : *pDst)
		{
			if (Dst != CInputMgr::GetInstance()->Get_Actor())
			{
				RECT rc = {};
				if (IntersectRect(&rc, Dst->Get_Rect(), CInputMgr::GetInstance()->Get_Actor()->Get_Rect()))
				{
					SCAST(CPlayer*, CInputMgr::GetInstance()->Get_Actor())->Set_Idle();
					CInputMgr::GetInstance()->Set_Actor(Dst);
					CUiMgr::GetInstance()->SetPlayer(SCAST(CPlayer*, Dst));
					SCAST(CPlayer*, CObjectMgr::GetInstance()->GetPlayer())->Observer_PlayerType();
					break;
				}
			}
		}
	}
}

bool CCollisionMgr::CheckSphere(const INFO * pDst, const INFO * pSrc)
{
	float fx = pDst->vPos.x - pSrc->vPos.x;
	float fy = pDst->vPos.y - pSrc->vPos.y;
	float fDist = sqrtf(fx * fx + fy * fy);

	float fRadiusSum = float(((int)pDst->vSize.x >> 1) + ((int)pDst->vSize.x >> 1));

	if (fRadiusSum > fDist)
		return true;

	return false;
}
bool CCollisionMgr::CheckSphereRect(const INFO * pSphere, const RECT* pRc)
{
	if ((pRc->left <= pSphere->vPos.x && pSphere->vPos.x <= pRc->right) ||
		(pRc->top <= pSphere->vPos.y && pSphere->vPos.y <= pRc->bottom))
	{
		RECT rc = {
			pRc->left - (LONG)pSphere->vSize.x / 2,
			pRc->top - (LONG)pSphere->vSize.y / 2,
			pRc->right + (LONG)pSphere->vSize.x / 2,
			pRc->bottom + (LONG)pSphere->vSize.y / 2
		};

		if ((rc.left <= pSphere->vPos.x && pSphere->vPos.x <= rc.right) &&
			(rc.top <= pSphere->vPos.y && pSphere->vPos.y <= rc.bottom))
			return true;
	}
	else
	{
		float fX = pSphere->vPos.x - pRc->left;
		float fY = pSphere->vPos.y - pRc->bottom;
		float fDist = sqrtf(fX * fX + fY * fY);
		if (fDist < pSphere->vSize.x / 2)
			return true;

		fX = pSphere->vPos.x - pRc->right;
		fY = pSphere->vPos.y - pRc->bottom;
		fDist = sqrtf(fX * fX + fY * fY);
		if (fDist < pSphere->vSize.x / 2)
			return true;

		fX = pSphere->vPos.x - pRc->right;
		fY = pSphere->vPos.y - pRc->top;
		fDist = sqrtf(fX * fX + fY * fY);
		if (fDist < pSphere->vSize.x / 2)
			return true;

		fX = pSphere->vPos.x - pRc->left;
		fY = pSphere->vPos.y - pRc->top;
		fDist = sqrtf(fX * fX + fY * fY);
		if (fDist < pSphere->vSize.x / 2)
			return true;
	}
	return false;
}
