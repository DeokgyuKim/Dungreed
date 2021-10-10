#include "stdafx.h"
#include "Spear.h"
#include "PlayerObserver.h"
#include "HitBox.h"
#include "SoundMgr.h"

CSpear::CSpear()
{
}


CSpear::~CSpear()
{
	Release();
}

int CSpear::Update()
{
	m_tInfo.vPos = GET_PLAYER_OB->GetPlayerPos();
	m_tInfo.vPos.y -= 8.f;

	m_tInfo.vPos += m_tInfo.vDir * m_fValue;

	if (m_fValue <= 0.f)
	{
		m_tInfo.vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
		VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);
	}

	IsAttack();
	return NO_EVENT;
}

void CSpear::LateUpdate()
{
	SaveOldInfo();
	AttackCameraShaking();
}

void CSpear::Render()
{
	//matWorld
	MATRIX	matScale, matRotateZ, matTrans, matWorld;
	MATSCAILING(&matScale, 1.f, 1.f, 0.f);

	float fCos = VEC3DOT(&m_tInfo.vDir, &VEC3(1.f, 0.f, 0.f));;
	float fAcos = acosf(fCos);
	fAcos = TODEGREE(fAcos);
	if (m_tInfo.vDir.y < 0)
		fAcos = 360.f - fAcos;

	MATROTATIONZ(&matRotateZ, TORADIAN(fAcos));
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matRotateZ * matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::MID);
}

int CSpear::ReverseUpdate()
{
	CObj::ReverseUpdate();
	if (m_lstOldInfoEx.size() > 0)
	{
		m_fValue = m_lstOldInfoEx.back();
		m_lstOldInfoEx.pop_back();
		if (m_lstOldInfoEx.size() > 0)
			m_lstOldInfoEx.pop_back();
		if (m_lstOldInfoEx.empty())
			m_lstOldInfoEx.emplace_back(m_fValue);
	}
	return NO_EVENT;
}

void CSpear::ReverseLateUpdate()
{
	CObj::ReverseLateUpdate();
}

void CSpear::Attack()
{
	if (!m_bAtt)
	{
		CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CHitBox::Create(m_tInfo.vPos + m_tInfo.vDir * 30.f + VEC3(0.f, 15.f, 0.f), VEC3(30.f, 30.f, 0.f), m_tInfo.vDir, m_tInfo.iAtt, HITBOX::PLAYER));
		m_bAtt = true;
		m_fAttTime = 0.f;
		m_fValue = 10.f;
		m_fShakeValue = 0.f;
		CSoundMgr::Get_Instance()->PlaySoundW(L"MySwing.wav", CSoundMgr::CHANNELID::PLAYER);
	}
}

void CSpear::AttackCameraShaking()
{
	if (m_bAtt && m_fAttTime < 0.1f)
	{
		m_fShakeValue += 1 - 40.f * GET_TIME;
		CScrollMgr::Set_Scroll(-m_tInfo.vDir * m_fShakeValue);
	}
}

HRESULT CSpear::Initialize()
{
	m_eType = OBJ::SPEAR;
	m_tInfo.iAtt = 40;
	m_tInfo.vSize = { 40.f, 6.f, 0.f };
	m_fAttSpeed = 1.f;
	m_fAttTime = 0.f;
	m_bAtt = false;
	m_bStab = false;
	m_fValue = 0.f;
	m_fShakeValue = 0.f;
	m_tInfo.m_iGold = 500;
	Set_Frame(L"Spear", L"SpearIdle", 0.f, 1.f);
	return S_OK;
}

void CSpear::Release()
{
}

void CSpear::IsAttack()
{
	if (m_bAtt)
	{
		m_fAttTime += GET_TIME;
		if (m_fAttTime <= 1.f / (m_fAttSpeed * 5.f))
			m_fValue -= 50 * GET_TIME;
		else
			m_fValue = 0.f;
		if (m_fAttTime >= 1.f / m_fAttSpeed)
		{
			m_bAtt = false;
			m_fAttTime = 0.f;
		}
	}
}

void CSpear::SaveOldInfo()
{
	CObj::SaveOldInfo();
	m_lstOldInfoEx.emplace_back(m_fValue);
	if (m_lstOldInfo.size() > 100)
		m_lstOldInfo.pop_front();
}

CSpear * CSpear::Create()
{
	CSpear* pInstance = new CSpear;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
