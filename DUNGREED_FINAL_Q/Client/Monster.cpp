#include "stdafx.h"
#include "Monster.h"
#include "PlayerObserver.h"
#include "SoundMgr.h"

CMonster::CMonster()
	: m_bLook(false)
	, m_fLookTime(0.f)
{
	m_pObserver = CPlayerObserver::Create();
	CSubject::GetInstance()->Subscribe(m_pObserver);
}


CMonster::~CMonster()
{
	CSubject::GetInstance()->UnSubscribe(m_pObserver);
}

void CMonster::Render()
{
	if (m_tInfo.bDead)
		return;
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
	RenderHp();
	CLineMgr::DrawBox(m_tRect);
}

void CMonster::RenderHp()
{
	if (m_iFullHp <= m_tInfo.iHp)
		return;
	MATRIX matTrans, matScale, matWorld;
	MATSCAILING(&matScale, 0.25f, 0.25f, 1.f);
	MATTRANSLATION(&matTrans, m_tInfo.vPos.x - CScrollMgr::Get_Scroll().x - 9.f,
		m_tInfo.vPos.y - CScrollMgr::Get_Scroll().y + 2.f,
		m_tInfo.vPos.z);

	matWorld = matScale * matTrans;
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"HpBar", &matWorld, L"PlayerLifeBase", 0, 255, RENDER::LEFTTOP);

	MATSCAILING(&matScale, 0.25f * ((float)m_tInfo.iHp / (float)m_iFullHp), 0.25f, 1.f);
	MATTRANSLATION(&matTrans, m_tInfo.vPos.x - CScrollMgr::Get_Scroll().x - 9.f,
		m_tInfo.vPos.y - CScrollMgr::Get_Scroll().y + 2.f,
		m_tInfo.vPos.z);
	matWorld = matScale * matTrans;

	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, L"HpBar", &matWorld, L"LifeBar", 0, 255, RENDER::LEFTTOP);


}

void CMonster::Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue)
{
	VEC3* pVec;
	VEC3  vDir;
	float fCos;
	float fAcos;
	switch (eType)
	{
	case OBJ::HITBOX:
		m_tInfo.iHp -= (int)(*pVecValue)[0];
		if (m_tInfo.iHp <= 0)
			CSoundMgr::Get_Instance()->PlaySoundW(L"MyMonsterDie.wav", CSoundMgr::CHANNELID::MONSTER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitMonster.wav", CSoundMgr::CHANNELID::MONSTER);
		pVec = (VEC3*)(*pVecValue)[1];
		CreateMeleeHit(*pVec);
		break;
	case OBJ::PLAYER:
		m_tInfo.iHp -= (int)(*pVecValue)[0];
		if (m_tInfo.iHp <= 0)
			CSoundMgr::Get_Instance()->PlaySoundW(L"MyMonsterDie.wav", CSoundMgr::CHANNELID::MONSTER);
		CSoundMgr::Get_Instance()->PlaySoundW(L"MyHitMonster.wav", CSoundMgr::CHANNELID::MONSTER);
		vDir = GET_PLAYER_OB->GetPlayerPos() - m_tInfo.vPos;

		VEC3NORMAL(&vDir, &vDir);

		fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
		fAcos = acosf(fCos);
		fAcos = TODEGREE(fAcos);

		if (vDir.y < 0)
			fAcos = 360.f - fAcos;
		CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos, 0.4f, L"Monster", L"MonsterMeleeHit", 4.f, fAcos, RENDER::MID);
		break;
	default:
		break;
	}
}

void CMonster::CreateMeleeHit(VEC3 vDir)
{
	float fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
	float fAcos = acosf(fCos);
	fAcos = TODEGREE(fAcos);
	if (vDir.y < 0)
		fAcos = 360.f - fAcos;
	CEffectMgr::GetInstance()->CreateEffect(EFFECTMGR::RESOURCE_EFFECT, m_tInfo.vPos, 0.4f, L"Monster", L"MonsterMeleeHit", 4.f, fAcos, RENDER::MID);

}
