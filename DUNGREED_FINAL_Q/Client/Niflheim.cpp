#include "stdafx.h"
#include "Niflheim.h"
#include "IcePillar.h"
#include "NiflheimState.h"
#include "PlayerObserver.h"
#include "IceSpear.h"
#include "Icicle.h"

#include "SoundMgr.h"

CNiflheim::CNiflheim()
{
}


CNiflheim::~CNiflheim()
{
	Release();
}

HRESULT CNiflheim::Initialize()
{
	m_eType = OBJ::MONSTER;
	m_eMonType = MONSTER::BOSS_MONSTER;

	m_tInfo.fSpeed = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 20.f, 18.f, 0.f };

	m_tInfo.iAtt = 1;
	m_tInfo.iHp = BOSS_HP;

	Set_Frame(L"Boss", L"BossCreate", 0.f, 16.f);

	m_tInfo.bDead = false;
	m_bStopFrame = false;

	for(int i = 0; i < 4; ++i)
	{
		CIcePillar* pTemp = CIcePillar::Create(this, i);
		m_lstIcePillar.emplace_back(pTemp);
		CObjectMgr::GetInstance()->AddObject(OBJ_ICEPILLAR, pTemp);
	}

	Set_Look(true);

	m_pState = new StateMachine<CNiflheim>(this);
	m_pState->SetCurrentState(CNiflheim_IDLE::Instance());

	m_fMakeCount = 0.f;
	m_bMake = false;

	m_fDeadCount = 0.f;

	return S_OK;
}

int CNiflheim::Update()
{
	if (m_tInfo.bDead)
		return DEAD_OBJ;
	if (!m_bMake)
	{
		m_fMakeCount += GET_TIME;
		if (m_fMakeCount >= 3.f)
		{
			m_bMake = true;
			Set_Frame(L"Boss", L"BossIdle", 0.f, 6.f);
		}
		return NO_EVENT;
	}

	m_pState->Update();
	for (auto& Ice = m_lstIcePillar.begin(); Ice != m_lstIcePillar.end();)
	{
		if ((*Ice)->Update() == DEAD_OBJ)
		{
			auto iter_find = find(CObjectMgr::GetInstance()->GetLst(OBJ_ICEPILLAR)->begin(), CObjectMgr::GetInstance()->GetLst(OBJ_ICEPILLAR)->end(), *Ice);
			CObjectMgr::GetInstance()->GetLst(OBJ_ICEPILLAR)->erase(iter_find);
			SafeDelete<CIcePillar*>(*Ice);
			Ice = m_lstIcePillar.erase(Ice);
		}
		else
			++Ice;
	}
	CObj::UpdateRect();
	return 0;
}

void CNiflheim::LateUpdate()
{
	if (m_tInfo.bDead)
	{
		CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
		
		return;
	}
	if (!m_bMake)
	{
		CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
		if (m_tFrame.bReverse)
			m_tFrame.fStart -= m_tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime() * 0.33f;
		else
			m_tFrame.fStart += m_tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime() * 0.33f;

		if (m_tFrame.fStart > m_tFrame.fCount)
			m_tFrame.fStart = 0.f;
		if (m_tFrame.fStart < 0)
			m_tFrame.fStart = m_tFrame.fCount - 0.00001f;
		return;
	}
	if (m_fMakeCount <= 4.f)
	{
		m_fMakeCount += GET_TIME;
		CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
	}
	for (auto& Ice : m_lstIcePillar)
		Ice->LateUpdate();
	if(!m_bStopFrame)
		CObj::FrameUpdate();

	if (m_bShake)
	{
		CScrollMgr::Set_Scroll(VEC3(rand() % m_iShakeValue, rand() % m_iShakeValue, 0.f));
	}
	CObj::SaveOldInfo();
}

void CNiflheim::Render()
{
	MATRIX matScale, matTrans, matWorld;

	if (m_bLook)
	{
		VEC3 vDir = DCAST(CPlayerObserver*, m_pObserver)->GetPlayerPos() - m_tInfo.vPos;
		VEC3NORMAL(&vDir, &vDir);
		VEC3CROSS(&vDir, &vDir, &VEC3(0.f, -1.f, 0.f));
		if (vDir.z <= 0)	//¿À¸¥ÂÊ
			MATSCAILING(&matScale, 1.f, 1.f, 1.f);
		else				//¿ÞÂÊ
			MATSCAILING(&matScale, -1.f, 1.f, 1.f);
	}
	else
	{
		m_fLookTime += GET_TIME;
		if (m_fLookTime < 2.0f)
			MATSCAILING(&matScale, 1.f, 1.f, 1.f);
		else
		{
			MATSCAILING(&matScale, -1.f, 1.f, 1.f);
			if (m_fLookTime >= 4.f)
				m_fLookTime = 0.f;
		}
	}
	MATTRANSLATION(&matTrans, m_tInfo.vPos.x - CScrollMgr::Get_Scroll().x,
		m_tInfo.vPos.y - CScrollMgr::Get_Scroll().y,
		m_tInfo.vPos.z);

	matWorld = matScale * matTrans;

	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, (int)m_tFrame.fStart);
	for (auto& Ice : m_lstIcePillar)
		Ice->Render();
}

void CNiflheim::Release()
{
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::NIFLDEAD_EFFECT, m_tInfo.vPos, 3.f, L"Boss", L"BossDie", 30.f);
}

bool CNiflheim::Attack()
{
	return false;
}

bool CNiflheim::Searching()
{
	return false;
}

void CNiflheim::Following()
{
}

void CNiflheim::MakeIcePillar()
{
	for (int i = 0; i < 4; ++i)
	{
		CIcePillar* pTemp = CIcePillar::Create(this, i);
		m_lstIcePillar.emplace_back(pTemp);
		CObjectMgr::GetInstance()->AddObject(OBJ_ICEPILLAR, pTemp);
	}
}

void CNiflheim::ShotIce()
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->ShotIce();
}

void CNiflheim::ShotSpear(VEC3 vPos, bool bRight)
{
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CIceSpear::Create(vPos, 20.f, bRight, HITBOX::MONSTER));
}

void CNiflheim::ShotIcicle(VEC3 vPos)
{
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CIcicle::Create(vPos, 20.f, HITBOX::MONSTER));
}

void CNiflheim::ShotDirIce(VEC3 vDir, float fSpeed)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->ShotDirIce(vDir, fSpeed);
}

void CNiflheim::ShotTargetIce(VEC3 vDir)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->ShotTargetIce(vDir);
}

void CNiflheim::ShotTargetIceImmediately(VEC3 vPos)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->ShotTargetIceImmediately(vPos);
}

void CNiflheim::ZaIce(float fAngle)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->RotateZa(fAngle);
}

void CNiflheim::GongIce(float fAngle)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->RotateGong(fAngle);
}

void CNiflheim::SetIceZaAngle(float fAngle)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->SetZaAngle(fAngle);
}

void CNiflheim::SetIceGongAngle(float fAngle)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->SetGongAngle(fAngle);
}

void CNiflheim::SetIceDist(float fDist)
{
	for (auto& Pillar : m_lstIcePillar)
		Pillar->PlusDist(fDist);
}

void CNiflheim::SetPatt3IceAngle()
{
	for (auto& Pillar : m_lstIcePillar)
	{
		switch (Pillar->GetNum())
		{
		case 0:
			Pillar->SetGongAngle(30.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 1:
			Pillar->SetGongAngle(150.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 2:
			Pillar->SetGongAngle(210.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 3:
			Pillar->SetGongAngle(330.f);
			Pillar->SetZaAngle(0.f);
			break;
		default:
			break;
		}
	}
}

void CNiflheim::SetPatt3IceAngleEnd()
{
	for (auto& Pillar : m_lstIcePillar)
	{
		switch (Pillar->GetNum())
		{
		case 0:
			Pillar->SetGongAngle(45.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 1:
			Pillar->SetGongAngle(135.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 2:
			Pillar->SetGongAngle(225.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 3:
			Pillar->SetGongAngle(315.f);
			Pillar->SetZaAngle(0.f);
			break;
		default:
			break;
		}
	}
}

void CNiflheim::SetPatt6IceAngle()
{
	for (auto& Pillar : m_lstIcePillar)
	{
		switch (Pillar->GetNum())
		{
		case 0:
			Pillar->SetGongAngle(90.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 1:
			Pillar->SetGongAngle(120.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 2:
			Pillar->SetGongAngle(150.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 3:
			Pillar->SetGongAngle(180.f);
			Pillar->SetZaAngle(0.f);
			break;
		default:
			break;
		}
	}
}

void CNiflheim::SetPatt6IceCharginAngle()
{
	for (auto& Pillar : m_lstIcePillar)
	{
		switch (Pillar->GetNum())
		{
		case 0:
			Pillar->RotateGong(-0.2f);
			Pillar->SetZaAngle(0.f);
			break;
		case 1:
			Pillar->RotateGong(-0.07f);
			Pillar->SetZaAngle(0.f);
			break;
		case 2:
			Pillar->RotateGong(0.07f);
			Pillar->SetZaAngle(0.f);
			break;
		case 3:
			Pillar->RotateGong(0.2f);
			Pillar->SetZaAngle(0.f);
			break;
		default:
			break;
		}
	}
}

void CNiflheim::SetPatt6IceAngleEnd()
{
	for (auto& Pillar : m_lstIcePillar)
	{
		switch (Pillar->GetNum())
		{
		case 0:
			Pillar->SetGongAngle(45.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 1:
			Pillar->SetGongAngle(135.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 2:
			Pillar->SetGongAngle(225.f);
			Pillar->SetZaAngle(0.f);
			break;
		case 3:
			Pillar->SetGongAngle(315.f);
			Pillar->SetZaAngle(0.f);
			break;
		default:
			break;
		}
	}
}

VEC3 CNiflheim::Get_PlayerPos()
{
	return GET_PLAYER_OB->GetPlayerPos();
}

void CNiflheim::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	VEC3 vDir;
	float fCos, fAcos;
	switch (eType)
	{
	case OBJ::HITBOX:
		if (IsAttackPossible())
		{
			m_tInfo.iHp -= (int)(*pVecValue)[0];
			CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitMonster.wav", CSoundMgr::CHANNELID::MONSTER);
			if (m_tInfo.iHp <= 0)
			{
				m_tInfo.bDead = true;
				Set_Frame(L"Boss", L"BossDie", 0.f, 29.f);
			}
			VEC3* pVec = (VEC3*)(*pVecValue)[1];
			CreateMeleeHit(*pVec);
		}
		break;
	case OBJ::PLAYER:
		if (IsAttackPossible())
		{
			m_tInfo.iHp -= (int)(*pVecValue)[0];
			CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitMonster.wav", CSoundMgr::CHANNELID::MONSTER);
			if (m_tInfo.iHp <= 0)
			{
				m_tInfo.bDead = true;
				Set_Frame(L"Boss", L"BossDie", 0.f, 29.f);
			}
			vDir = GET_PLAYER_OB->GetPlayerPos() - m_tInfo.vPos;

			VEC3NORMAL(&vDir, &vDir);

			fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
			fAcos = acosf(fCos);
			fAcos = TODEGREE(fAcos);

			if (vDir.y < 0)
				fAcos = 360.f - fAcos;
			CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos, 0.4f, L"Monster", L"MonsterMeleeHit", 4.f, fAcos, RENDER::MID);
		}
		break;
	default:
		break;
	}
}

CNiflheim * CNiflheim::Create(VEC3 vPos)
{
	CNiflheim* pInstance = new CNiflheim;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	return pInstance;
}
