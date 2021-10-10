#include "stdafx.h"
#include "IcePillar.h"
#include "Niflheim.h"
#include "IceBullet.h"
#include "NiflheimState.h"
#include "PlayerObserver.h"

#include "SoundMgr.h"

CIcePillar::CIcePillar()
	: m_pOrizin(1.f, 0.f, 0.f)
{
	m_pObserver = CPlayerObserver::Create();
	CSubject::GetInstance()->Subscribe(m_pObserver);
}


CIcePillar::~CIcePillar()
{
	CSubject::GetInstance()->UnSubscribe(m_pObserver);
}

HRESULT CIcePillar::Initialize()
{
	m_eType = OBJ::ICE_PILLAR;

	m_tInfo.fSpeed = 200.f;
	m_tInfo.iAtt = 0;
	m_tInfo.iHp = 100;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_tInfo.vRenderDir = { 1.f, 0.f, 0.f };
	m_tInfo.vSize = { 48.f, 16.f,0.f };
	m_fDist = 50.f;
	
	Set_Frame(L"IcePillar", L"IcePillarCreate", 0.f, 20.f);

	switch (m_iNum)
	{
	case 0:
		m_fGongAngle = 45.f;
		break;
	case 1:
		m_fGongAngle = 135.f;
		break;
	case 2:
		m_fGongAngle = 225.f;
		break;
	case 3:
		m_fGongAngle = 315.f;
		break;
	default:
		break;
	}
	m_fZaAngle = 0.f;
	m_tInfo.bDead = false;
	m_bCreate = false;
	m_bDestroy = false;
	m_fTimeCount = 0.f;
	ZeroMemory(m_tCollRect, sizeof(m_tCollRect));

	return S_OK;
}

int CIcePillar::Update()
{
	if (!m_tInfo.bDead && m_tInfo.iHp <= 0)
	{
		Set_Frame(L"IcePillar", L"DestroyPillar", 0.f, 3.f);
		m_tInfo.bDead = true;
	}
	if (m_tInfo.bDead)
	{
		m_fTimeCount += GET_TIME;
		if (m_fTimeCount >= 1.f)
		{
			m_bDestroy = true;
		}
		if (m_bDestroy)
		{
			return DEAD_OBJ;
		}
		return NO_EVENT;
	}

	if (!m_bCreate)
	{
		m_fTimeCount += GET_TIME;
		if (m_fTimeCount >= 1.f)
		{
			Set_Frame(L"IcePillar", L"IcePillarIdle", 0.f, 1.f);
			m_bCreate = true;
			m_fTimeCount = 0.f;
		}
	}
	

	//위치 찾기
	VEC3 vPos = m_pOnwer->Get_Info()->vPos;
	vPos.y -= 16.f;	//사진크기 절반 나눔

	MATRIX matZa, matTrans, matGong, matParent, matWorld;
	MATROTATIONZ(&matZa, TORADIAN(m_fZaAngle));
	MATTRANSLATION(&matTrans, m_fDist, 0.f, 0.f);
	MATROTATIONZ(&matGong, TORADIAN(m_fGongAngle));
	MATTRANSLATION(&matParent, vPos.x, vPos.y, vPos.z);

	m_tInfo.matWorld = matZa * matTrans * matGong * matParent;

	VEC3_T_COORD(&m_tInfo.vPos, &VEC3(0.f, 0.f, 0.f), &m_tInfo.matWorld);

	//충돌박스 제작
	this->UpdateRect();

	return 0;
}

void CIcePillar::LateUpdate()
{
	CObj::FrameUpdate();
	CObj::SaveOldInfo();
}

void CIcePillar::Render()
{
	//matWorld
	//MATRIX	matScale, matRotateZ, matTrans, matWorld;
	//D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	//D3DXMatrixRotationZ(&matRotateZ, m_fZaAngle + m_fGongAngle);
	//VEC3 vScroll = CScrollMgr::Get_Scroll();
	//D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	//matWorld = matScale * matTrans;
	//
	//cout << m_tInfo.vPos.x << ", " << m_tInfo.vPos.y << endl;

	//텍스쳐 렌더
	VEC3 vPos = m_pOnwer->Get_Info()->vPos;
	vPos.y -= 16.f;	//사진크기 절반 나눔
	MATRIX matZa, matTrans, matGong, matParent, matWorld;
	MATROTATIONZ(&matZa, TORADIAN(m_fZaAngle));
	MATTRANSLATION(&matTrans, m_fDist, 0.f, 0.f);
	MATROTATIONZ(&matGong, TORADIAN(m_fGongAngle));
	MATTRANSLATION(&matParent, vPos.x - CScrollMgr::Get_Scroll().x, vPos.y - CScrollMgr::Get_Scroll().y, vPos.z);

	m_tInfo.matWorld = matZa * matTrans * matGong * matParent;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &m_tInfo.matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::MID);

	for (int i = 0; i < 3; ++i)
		CLineMgr::DrawBox(m_tCollRect[i]);
}

void CIcePillar::Release()
{
}

CIcePillar * CIcePillar::Create(CNiflheim * pOwner, int iNum)
{
	CIcePillar* pInstance = new CIcePillar;
	pInstance->m_iNum = iNum;
	pInstance->m_pOnwer = pOwner;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

void CIcePillar::ShotIce()
{
	if (m_tInfo.bDead)
		return;
	MATRIX matRotate;
	MATROTATIONZ(&matRotate, TORADIAN(m_fZaAngle + m_fGongAngle));
	VEC3 vDir = {};
	VEC3_T_COORD(&vDir, &VEC3(1.f, 0.f, 0.f), &matRotate);
	VEC3NORMAL(&vDir, &vDir);
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CIceBullet::Create(m_tInfo.vPos, vDir, 1, HITBOX::MONSTER));
}

void CIcePillar::ShotDirIce(VEC3 vDir, float fSpeed)
{
	if (m_tInfo.bDead)
		return;
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CIceBullet::CreateImmediately(m_tInfo.vPos, fSpeed, vDir, 1, HITBOX::MONSTER));
}

void CIcePillar::ShotTargetIce(VEC3 vPos)
{
	if (m_tInfo.bDead)
		return;
	MATRIX matRotate;
	MATROTATIONZ(&matRotate, TORADIAN(m_fZaAngle + m_fGongAngle));
	VEC3 vDir = vPos - m_tInfo.vPos;
	VEC3NORMAL(&vDir, &vDir);
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CIceBullet::CreateImmediately(m_tInfo.vPos, 200.f, vDir, 1, HITBOX::MONSTER));

}

void CIcePillar::ShotTargetIceImmediately(VEC3 vPos)
{
	if (m_tInfo.bDead)
		return;
	MATRIX matRotate;
	MATROTATIONZ(&matRotate, TORADIAN(m_fZaAngle + m_fGongAngle));
	VEC3 vDir = vPos - m_tInfo.vPos;
	VEC3NORMAL(&vDir, &vDir);
	CObjectMgr::GetInstance()->AddObject(OBJ_MONSTER_HITBOX, CIceBullet::CreateImmediately(m_tInfo.vPos, 200.f, vDir, 1, HITBOX::MONSTER));

}

void CIcePillar::UpdateRect()
{
	VEC3 vPos[3] = {};
	ZeroMemory(vPos, sizeof(vPos));

	VEC3_T_COORD(&vPos[0], &vPos[0], &m_tInfo.matWorld);

	VEC3 vTemp = { 0.f, 0.f, 0.f };
	MATRIX matRotate, matTrans, matWorld;
	MATTRANSLATION(&matTrans, 16.f, 0.f, 0.f);
	MATROTATIONZ(&matRotate, TORADIAN(m_fZaAngle + m_fGongAngle + 90.f));

	matWorld = matTrans * matRotate;

	VEC3_T_COORD(&vTemp, &vTemp, &matWorld);

	vPos[1] = vPos[0] + vTemp;
	vPos[2] = vPos[0] - vTemp;

	for (int i = 0; i < 3; ++i)
		m_tCollRect[i] = { (LONG)vPos[i].x - 8, (LONG)vPos[i].y - 8, (LONG)vPos[i].x + 8, (LONG)vPos[i].y + 8 };
	
}

void CIcePillar::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	VEC3* pVec;
	VEC3  vDir;
	float fCos, fAcos;
	switch (eType)
	{
	case OBJ::HITBOX:
		if (m_pOnwer->Get_FSM()->GetCurrentState() != CNiflheim_PATT6::Instance())
		{
			m_tInfo.iHp -= (int)(*pVecValue)[0];
			CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitMonster.wav", CSoundMgr::CHANNELID::MONSTER);
			pVec = (VEC3*)(*pVecValue)[1];
			float fCos = VEC3DOT(pVec, &VEC3(1.f, 0.f, 0.f));
			float fAcos = acosf(fCos);
			fAcos = TODEGREE(fAcos);
			if (pVec->y < 0)
				fAcos = 360.f - fAcos;
			CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos, 0.4f, L"Monster", L"MonsterMeleeHit", 4.f, fAcos, RENDER::MID);

		}
		break;
	case OBJ::PLAYER:
		if (m_pOnwer->Get_FSM()->GetCurrentState() != CNiflheim_PATT6::Instance())
		{
			vDir = GET_PLAYER_OB->GetPlayerPos() - m_tInfo.vPos;

			VEC3NORMAL(&vDir, &vDir);

			fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
			fAcos = acosf(fCos);
			fAcos = TODEGREE(fAcos);

			if (vDir.y < 0)
				fAcos = 360.f - fAcos;
			CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos, 0.4f, L"Monster", L"MonsterMeleeHit", 4.f, fAcos, RENDER::MID);
			m_tInfo.iHp -= (int)(*pVecValue)[0];
			CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitMonster.wav", CSoundMgr::CHANNELID::MONSTER);
		}
		break;
	default:
		break;
	}
}
