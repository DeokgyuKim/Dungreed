#include "stdafx.h"
#include "Player.h"
#include "Weapon.h"
#include "Dart.h"
#include "Ring.h"

#include "SoundMgr.h"

int		CPlayer::m_iInvenIndex = 15;

CPlayer::CPlayer()
	: m_pInven(CInventory::GetInstance())
{
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize()
{
	m_eType = OBJ::PLAYER;

	m_tInfo.vSize = { 16.f, 16.f, 0.f };
	m_tInfo.fSpeed = 150.f;
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 1.f, 0.f, 0.f };

	m_tInfo.iAtt = 20;
	m_tInfo.iHp = PLAYER_HP;

	m_tInfo.bDead = false;
	m_bRenderRight = true;
	m_bJump = false;
	m_bDash = false;
	m_bDamaged = false;
	m_bDamagedRenderUi = false;
	m_bNotCollisionTile = false;
	m_fJumpTime = 0.f;
	m_fDashTime = 0.f;
	m_fAfterImageTime = 0.f;
	m_iDashCount = 6.f;
	m_fDashChargingTime = 0.f;
	m_fMoveDustCreateTime = 0.f;
	m_ePlayerType = PLAYER::NORMAL_PLAYER;
	m_eState = PLAYER_STATE::END;
	CObj::SaveOldInfo();
	m_pSubject = CSubject::GetInstance();
	CInputMgr::GetInstance()->Initialize();
	ChangeState(PLAYER_STATE::IDLE);
	m_iInvenIndex = 15;
	return S_OK;
}

int CPlayer::Update()
{
	if (CInputMgr::GetInstance()->Get_Actor() != this)
	{
		CObj::FrameUpdate();
		CObj::UpdateRect();
		return NO_EVENT;
	}
	//플레이어 방향 탐색
	VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
	VEC3NORMAL(&vDir, &vDir);
	VEC3CROSS(&vDir, &vDir, &VEC3(0.f, -1.f, 0.f));
	if (vDir.z <= 0)	//오른쪽
		m_bRenderRight = true;
	else				//왼쪽
		m_bRenderRight = false;
	
	//키 입력
	CInputMgr::GetInstance()->GetKeyDown();

	if (CKeyMgr::GetInstance()->KeyDown(KEY_Q))
	{
		if (m_iInvenIndex == 16)
		{
			m_iInvenIndex = 15;
			if (m_pInven->GetItem(m_iInvenIndex) == nullptr)
				m_iInvenIndex = 16;
			else
			{
				m_pWeapon = SCAST(CWeapon*, m_pInven->GetItem(m_iInvenIndex));
				if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
					if (SCAST(CSubItem*, m_pInven->GetItem(m_iInvenIndex + 2))->GetSubItemType() == SUBITEM::BAT)
						m_pInven->GetItem(m_iInvenIndex + 2)->Initialize();
			}
		}
		else if (m_iInvenIndex == 15)
		{
			m_iInvenIndex = 16;
			if (m_pInven->GetItem(m_iInvenIndex) == nullptr)
				m_iInvenIndex = 15;
			else
			{
				m_pWeapon = SCAST(CWeapon*, m_pInven->GetItem(m_iInvenIndex));
				if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
					if (SCAST(CSubItem*, m_pInven->GetItem(m_iInvenIndex + 2))->GetSubItemType() == SUBITEM::BAT)
						m_pInven->GetItem(m_iInvenIndex + 2)->Initialize();
			}
		}

	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_R))
	{
		if (CInventory::GetInstance()->GetItem(19) != nullptr)
		{
			SCAST(CRing*, CInventory::GetInstance()->GetItem(19))->EffectRing();
		}
	}


	//점프, 대쉬, 대쉬중 논 콜리젼
	IsJumping();
	IsDashing();
	NotCollision();

	//데미지 입었는지 체크
	IsDamaged();

	//프레임 방향 설정
	if (m_bRenderRight)
	{
		if (m_tInfo.vDir.x < 0)
			Set_Frame_Reverse(true);
		else
			Set_Frame_Reverse(false);
	}
	else
	{
		if (m_tInfo.vDir.x > 0)
			Set_Frame_Reverse(true);
		else
			Set_Frame_Reverse(false);
	}

	//프레임 업데이트
	CObj::FrameUpdate();


	//플레이어 이동
	if (m_bDash)
	{
		m_tInfo.vPos += m_tInfo.vDir * SPEED((m_tInfo.fSpeed + 400 * cosf(m_fDashTime)), GET_TIME);
		int index = CTileMgr::GetInstance()->Get_TileIndex(m_tInfo.vPos);

		int iCount = 1;
		while(CTileMgr::GetInstance()->Get_Tile()[index]->byOption == 2)
		{
			m_tInfo.vPos -= m_tInfo.vDir * SPEED((m_tInfo.fSpeed + 400 * cosf(m_fDashTime)), GET_TIME) / iCount++;
			if (iCount >= 20)
				break;
			m_tInfo.vPos += m_tInfo.vDir * SPEED((m_tInfo.fSpeed + 400 * cosf(m_fDashTime)), GET_TIME) / iCount;
		}

		if (m_tInfo.vPos.y > CTileMgr::GetInstance()->Get_Tile()[1081]->vPos.y)
			m_tInfo.vPos.y = CTileMgr::GetInstance()->Get_Tile()[1081]->vPos.y + 1;
	}
	else
	{
		m_tInfo.vPos += m_tInfo.vDir * SPEED(m_tInfo.fSpeed, GET_TIME);
		int index = CTileMgr::GetInstance()->Get_TileIndex(m_tInfo.vPos);
		int iCount = 1;
		while (CTileMgr::GetInstance()->Get_Tile()[index]->byOption == 2)
		{
			m_tInfo.vPos -= m_tInfo.vDir * SPEED((m_tInfo.fSpeed + 400 * cosf(m_fDashTime)), GET_TIME) / iCount++;
			if (iCount >= 10)
				break;
			m_tInfo.vPos += m_tInfo.vDir * SPEED((m_tInfo.fSpeed + 400 * cosf(m_fDashTime)), GET_TIME) / iCount;
		}
		m_tInfo.vDir = { 0.f, 0.f, 0.f };
	}

	//옵저버에 노티파이
	m_pSubject->Notify(DATA_PLAYER_POS, &m_tInfo.vPos);
	m_pSubject->Notify(DATA_PLAYER_RENDERRIGHT, &m_bRenderRight);
	m_pSubject->Notify(DATA_PLAYER_DIR, &Get_PlayerMovePos());

	//무기 업데이트
	if(m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->Update();
	if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
		m_pInven->GetItem(m_iInvenIndex + 2)->Update();
	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	if (CInputMgr::GetInstance()->Get_Actor() != this)
		return;
	//충돌상자 업데이트
	CObj::UpdateRect();


	//플레이어 상태 보정
	if (fabs(m_tInfo.vPos.x - m_lstOldInfo.back().first.vPos.x) <= 1.f)
		ChangeState(PLAYER_STATE::IDLE);

	if (m_tInfo.vPos.x < 0)
		m_tInfo.vPos.x = 0;
	if (m_tInfo.vPos.y < 0)
		m_tInfo.vPos.y = 0;

	if (m_tInfo.vPos.x > TILECX * TILE_COUNT_X)
		m_tInfo.vPos.x = TILECX * TILE_COUNT_X;
	if (m_tInfo.vPos.y > TILECY * TILE_COUNT_Y)
		m_tInfo.vPos.y = TILECY * TILE_COUNT_Y;

	//스크롤 보정
	CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });

	//무기 업데이트
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->LateUpdate();
	if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
		m_pInven->GetItem(m_iInvenIndex + 2)->LateUpdate();

	//잔상 추가
	MakeAfterImage();

	//이전 인포 저장
	CObj::SaveOldInfo();
	Observer_PlayerType();
}

void CPlayer::Render()
{
	if (CInputMgr::GetInstance()->Get_Actor() != this)
	{
		//matWorld
		MATRIX	matScale, matRotateZ, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		D3DXMatrixRotationZ(&matRotateZ, 0.f);
		VEC3 vScroll = CScrollMgr::Get_Scroll();
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
		matWorld = matScale * matRotateZ * matTrans;

		//텍스쳐 렌더
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
		CLineMgr::DrawBox(m_tRect);
		return;
	}
	//matWorld
	MATRIX	matScale, matRotateZ, matTrans, matWorld;
	if(m_bRenderRight)
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotateZ, 0.f);
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matRotateZ * matTrans;

	//텍스쳐 렌더
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));
	CLineMgr::DrawBox(m_tRect);

	//무기 렌더
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->Render();
	if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
		m_pInven->GetItem(m_iInvenIndex + 2)->Render();
}

int CPlayer::ReverseUpdate()
{
	CObj::ReverseUpdate();
	if (CInputMgr::GetInstance()->Get_Actor() != this)
		return NO_EVENT;
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->ReverseUpdate();
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		m_pInven->GetItem(m_iInvenIndex)->ReverseLateUpdate();

	if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
		m_pInven->GetItem(m_iInvenIndex + 2)->ReverseUpdate();
	if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
		m_pInven->GetItem(m_iInvenIndex + 2)->ReverseLateUpdate();
	//스크롤 보정
	CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
	if (m_lstOldInfo.size() == 1)
	{
		ChangeState(PLAYER_STATE::IDLE);
		Set_Frame(L"Normal", L"CharIdle", m_tFrame.fStart, 4.f);
		return REVERSE_END;
	}
	return NO_EVENT;
}

void CPlayer::MoveLeft()
{
	m_tInfo.vDir.x = -1.f;
	VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);
	m_fMoveDustCreateTime += GET_TIME;
}

void CPlayer::MoveRight()
{
	m_tInfo.vDir.x = 1.f;
	VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);
	m_fMoveDustCreateTime += GET_TIME;

}

void CPlayer::MoveDown()
{
	m_bNotCollisionTile = true;//m_tInfo.vSize.y;
	m_fNotCollisionTileTime = 0.f;
	m_fMoveDustCreateTime += GET_TIME;
}

void CPlayer::Jump()
{
	if (!m_bJump)
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"MyJump.wav", CSoundMgr::CHANNELID::PLAYER);
		m_bJump = true;
		m_fJumpTime = 0.f;
	}
	m_fMoveDustCreateTime += GET_TIME;
}

void CPlayer::Attack()
{
	if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
		SCAST(CWeapon*, m_pInven->GetItem(m_iInvenIndex))->Attack();
}

void CPlayer::Dash()
{
	if (!m_bDash && m_iDashCount > 0)
	{
		m_bJump = false;
		if (m_pInven->GetItem(m_iInvenIndex + 2) != nullptr)
		{
			if (SCAST(CSubItem*, m_pInven->GetItem(m_iInvenIndex + 2))->GetSubItemType() != SUBITEM::WING)
				--m_iDashCount;
		}
		else
			--m_iDashCount;
		m_fJumpTime = 0.f;
		m_tInfo.vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
		VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);
		m_bDash = true;
		CSoundMgr::Get_Instance()->PlaySoundW(L"MyDash.wav", CSoundMgr::CHANNELID::PLAYER);
		m_fDashTime = 0.f;
	}
}

void CPlayer::IsJumping()
{
	if (m_bJump)
	{
		m_fJumpTime += GET_TIME;
		m_fMoveDustCreateTime += GET_TIME;
		float fValue = 66 * m_fJumpTime - 190.8f * m_fJumpTime * m_fJumpTime * 0.5f;
		if (fValue < 0)
			fValue = 0.f;
		m_tInfo.vPos.y -= fValue;
	}
	if (m_eState != PLAYER_STATE::DASH)
	{
		if (!CTileMgr::GetInstance()->IsCollision(m_tInfo.vPos, m_tRect))
		{
			m_fJumpTime += GET_TIME;
			float fValue = 190.8f * m_fJumpTime * m_fJumpTime * 0.5f;
			if (fValue > m_tInfo.vSize.y * 0.5f)
				fValue = m_tInfo.vSize.y * 0.5f;
			m_tInfo.vPos.y += fValue;
			if (m_tInfo.vPos.y > CTileMgr::GetInstance()->Get_Tile()[1081]->vPos.y)
				m_tInfo.vPos.y = CTileMgr::GetInstance()->Get_Tile()[1081]->vPos.y + 1;
		}
	}
}

void CPlayer::IsDashing()
{
	if (m_bDash)
	{
		m_fMoveDustCreateTime += GET_TIME;
		m_fDashTime += (PI / 2) * (GET_TIME * 5.f);
		if (m_fDashTime > PI / 2)
		{
			m_bDash = false;
			ChangeState(PLAYER_STATE::IDLE);
		}
	}
	if (m_iDashCount < 6)
	{
		m_fDashChargingTime += GET_TIME;
		if (m_fDashChargingTime >= 1.f)
		{
			++m_iDashCount;
			m_fDashChargingTime = 0.f;
		}
	}
}

void CPlayer::IsDamaged()
{
	if (m_fDamagedTime > 0.f)
	{
		m_fDamagedTime += GET_TIME;
		if(m_fDamagedTime >= 0.2f)
			m_bDamaged = false;
		if (m_fDamagedTime >= 0.5f)
		{
			m_bDamagedRenderUi = false;
			m_fDamagedTime = 0.f;
		}
	}
}

void CPlayer::NotCollision()
{
	if (CTileMgr::GetInstance()->Get_Tile()[CTileMgr::GetInstance()->Get_TileIndex(m_tInfo.vPos + VEC3(0.f, 8.f, 0.f))]->byOption == 2)
		return;
	if (m_bNotCollisionTile)
	{
		m_fNotCollisionTileTime += GET_TIME;
		if (m_fNotCollisionTileTime > 0.25f)
			m_bNotCollisionTile = false;
	}
}

void CPlayer::MakeAfterImage()
{
	m_fAfterImageTime += GET_TIME;
	if (m_fAfterImageTime > 0.05f)
	{
		if (VEC3LENGTH(&((m_lstOldInfo.back().first.vPos) - m_tInfo.vPos)) > 3.f)
		{
			CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::ALPHA_EFFECT, m_tInfo.vPos, 1.0f, m_tFrame.pObjectKey, m_tFrame.pStateKey, m_tFrame.fStart, 0.f, RENDER::BOTTOM,  m_bRenderRight);
			m_fAfterImageTime = 0.f;
		}
	}
}

void CPlayer::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	switch (eType)
	{
	case OBJ::TERRAIN:
		if ((int)(*pVecValue)[2] == 1)
		{
			//if (m_tInfo.vPos.y <= ((RECT*)((*pVecValue)[0]))->bottom)
			if (m_lstOldInfo.back().first.vPos.y <= m_tInfo.vPos.y)
			{
				m_tInfo.vPos.y = (float)(((RECT*)((*pVecValue)[0]))->bottom);// -m_tInfo.vSize.y / 2.f;
				CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
				if (m_fMoveDustCreateTime > 0.4f && !m_bJump && !m_bDash)
				{
					CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos, 0.4f, L"Player", L"MoveDust", 5.f);
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyStep", CSoundMgr::CHANNELID::EFFECT);
					m_fMoveDustCreateTime = 0.f;
				}
				m_bJump = false;
				m_fJumpTime = 0.f;
			}
		}
		else if ((int)(*pVecValue)[2] == 2)
		{
			RECT* pRect = ((RECT*)((*pVecValue)[0]));
			int Wide = pRect->right - pRect->left;
			int Height = pRect->bottom - pRect->top;

			if (Wide > Height)	//세로이동
			{
				if (m_tInfo.vPos.y < ((VEC3*)((*pVecValue)[1]))->y)	//위로
				{
					m_tInfo.vPos.y -= Height;
				}
				else	//아래로
				{
					m_tInfo.vPos.y += Height;
				}
			}
			else				//가로이동
			{
				if (m_tInfo.vPos.x > ((VEC3*)((*pVecValue)[1]))->x)	//위로
				{
					m_tInfo.vPos.x += Wide;
				}
				else	//아래로
				{
					m_tInfo.vPos.x -= Wide;
				}
			}
		}
		break;
	case OBJ::HITBOX:
		if (!m_bDamaged && !m_bDash)
		{
			if (CInventory::GetInstance()->GetItem(m_iInvenIndex + 2) != nullptr)
			{
				if (SCAST(CSubItem*, CInventory::GetInstance()->GetItem(m_iInvenIndex + 2))->GetSubItemType() == SUBITEM::SHIELD)
				{
					m_tInfo.iHp -= (int)(*pVecValue)[0] / 2;
					if (m_tInfo.iHp <= 0)
						m_tInfo.iHp = 0;
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitPlayer.wav", CSoundMgr::CHANNELID::PLAYER);
					m_bDamaged = true;
					m_bDamagedRenderUi = true;
					m_fDamagedTime = 0.0001f;
				}
				else
				{
					m_tInfo.iHp -= (int)(*pVecValue)[0];
					if (m_tInfo.iHp <= 0)
						m_tInfo.iHp = 0;
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitPlayer.wav", CSoundMgr::CHANNELID::PLAYER);
					m_bDamaged = true;
					m_bDamagedRenderUi = true;
					m_fDamagedTime = 0.0001f;
				}
			}
			else
			{
				m_tInfo.iHp -= (int)(*pVecValue)[0];
				if (m_tInfo.iHp <= 0)
					m_tInfo.iHp = 0;
				CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitPlayer.wav", CSoundMgr::CHANNELID::PLAYER);
				m_bDamaged = true;
				m_bDamagedRenderUi = true;
				m_fDamagedTime = 0.0001f;
			}
		}
		break;
	case OBJ::MONSTER:
		if (!m_bDamaged && !m_bDash)
		{
			if (CInventory::GetInstance()->GetItem(m_iInvenIndex + 2) != nullptr)
			{
				if (SCAST(CSubItem*, CInventory::GetInstance()->GetItem(m_iInvenIndex + 2))->GetSubItemType() == SUBITEM::SHIELD)
				{
					m_tInfo.iHp -= (int)(*pVecValue)[0] / 2;
					if (m_tInfo.iHp <= 0)
						m_tInfo.iHp = 0;
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitPlayer.wav", CSoundMgr::CHANNELID::PLAYER);
					m_bDamaged = true;
					m_bDamagedRenderUi = true;
					m_fDamagedTime = 0.0001f;
				}
				else
				{
					m_tInfo.iHp -= (int)(*pVecValue)[0];
					if (m_tInfo.iHp <= 0)
						m_tInfo.iHp = 0;
					CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitPlayer.wav", CSoundMgr::CHANNELID::PLAYER);
					m_bDamaged = true;
					m_bDamagedRenderUi = true;
					m_fDamagedTime = 0.0001f;
				}
			}
			else
			{
				m_tInfo.iHp -= (int)(*pVecValue)[0];
				if (m_tInfo.iHp <= 0)
					m_tInfo.iHp = 0;
				CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitPlayer.wav", CSoundMgr::CHANNELID::PLAYER);
				m_bDamaged = true;
				m_bDamagedRenderUi = true;
				m_fDamagedTime = 0.0001f;
			}
		}
	case OBJ::DART:
		if (m_pInven->GetItem(m_iInvenIndex) != nullptr)
			SCAST(CWeapon*, m_pInven->GetItem(m_iInvenIndex))->Set_boolAtt(false);
	default:
		break;
	}
}

void CPlayer::Release()
{
}

CPlayer * CPlayer::Create(VEC3 vPos, CWeapon* pWeapon)
{
	CPlayer* pInstance = new CPlayer;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_pWeapon = pWeapon;
	if (!CObjectMgr::GetInstance()->GetClear())
		pInstance->m_pInven->PushItem(15, pWeapon);
	return pInstance;
}

VEC3 CPlayer::Get_PlayerMovePos()
{
	VEC3 vDir = m_lstOldInfo.back().first.vPos - m_tInfo.vPos;
	VEC3NORMAL(&vDir, &vDir);
	return vDir;
}

const CWeapon * CPlayer::GetWeapon()
{
	return SCAST(CWeapon*, m_pInven->GetItem(m_iInvenIndex));
}
