#include "stdafx.h"
#include "Gun.h"
#include "PlayerObserver.h"
#include "RevolverBullet.h"

#include "SoundMgr.h"

CGun::CGun()
{
}


CGun::~CGun()
{
	Release();
}

int CGun::Update()
{
	m_tInfo.vPos = GET_PLAYER_OB->GetPlayerPos();
	if (GET_PLAYER_OB->GetPlayerRenderRight())
		m_tInfo.vPos += VEC3(5.f, -6.f, 0.f);
	else
		m_tInfo.vPos -= VEC3(5.f, 6.f, 0.f);

	VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
	VEC3NORMAL(&vDir, &vDir);
	m_tInfo.vDir = vDir;
	float fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
	float fAcos = acosf(fCos);
	fAcos = TODEGREE(fAcos);
	if (vDir.y < 0)
		fAcos = -fAcos;
	m_tGunInfo.m_tAngle.fLookAngle = fAcos;

	if (m_bAtt)
	{
		m_fAttTime += GET_TIME;
		if (GET_PLAYER_OB->GetPlayerType() == PLAYER::GUNMAN_PLAYER)
		{
			if (m_tGunInfo.m_iBulletCount > 0)
			{
				if (!m_bDoubleShot && m_fAttTime >= 1.f / (m_fAttSpeed * 2.f))
				{
					CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CRevolverBullet::Create(m_tInfo.vPos, m_tInfo.vDir, m_tInfo.iAtt, HITBOX::PLAYER));

					CSoundMgr::Get_Instance()->PlaySoundW(L"MyRevolverAtt.wav", CSoundMgr::CHANNELID::UI);
					--m_tGunInfo.m_iBulletCount;
					m_bDoubleShot = true;
				}
			}
		}
		if (m_fAttTime >= 1.f / m_fAttSpeed)
		{
			m_bAtt = false;
			m_bDoubleShot = false;
			m_fAttTime = 0.f;
			m_fShakeValue = 0.f;
		}
	}

	if (m_bReload)
	{
		m_fReloadTime += GET_TIME;
		if (m_fReloadTime >= RELOAD_TIME)
		{
			m_tGunInfo.m_iBulletCount = 10;
			m_bReload = false;
			m_bDoubleShot = false;
			m_fReloadTime = 0.f;
		}
	}

	return NO_EVENT;
}

void CGun::LateUpdate()
{
	AttackCameraShaking();
	SaveOldInfo();
}

void CGun::Render()
{
	//matWorld
	MATRIX	matScale, matRotateZ, matTrans, matWorld;
	if(fabs(m_tGunInfo.m_tAngle.fLookAngle) < 90.f)
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, -1.f, 0.f);

	//if(fabs(m_tGunInfo.m_tAngle.fLookAngle) < 90.f)
	MATROTATIONZ(&matRotateZ, TORADIAN(m_tGunInfo.m_tAngle.fLookAngle));
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matRotateZ * matTrans;

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::LEFT);

}

int CGun::ReverseUpdate()
{
	CObj::ReverseUpdate();
	if (m_lstOldInfoEx.size() > 0)
	{
		m_tGunInfo = m_lstOldInfoEx.back();
		m_lstOldInfoEx.pop_back();
		if (m_lstOldInfoEx.size() > 0)
			m_lstOldInfoEx.pop_back();
		if (m_lstOldInfoEx.empty())
			m_lstOldInfoEx.emplace_back(m_tGunInfo);
	}
	return NO_EVENT;
}

void CGun::ReverseLateUpdate()
{
	CObj::ReverseLateUpdate();
}

void CGun::Attack()
{
	if (m_tGunInfo.m_iBulletCount <= 0 && !m_bReload)
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"Reload-resources.assets-1765.wav", CSoundMgr::CHANNELID::PLAYER);
		m_bReload = true;
		m_fReloadTime = 0.f;
		return;
	}
	if (!m_bAtt && m_tGunInfo.m_iBulletCount > 0)
	{
		--m_tGunInfo.m_iBulletCount;
		m_bDoubleShot = false;
		m_bAtt = true;
		m_fAttTime = 0.f;
		CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CRevolverBullet::Create(m_tInfo.vPos, m_tInfo.vDir, m_tInfo.iAtt, HITBOX::PLAYER));

		CSoundMgr::Get_Instance()->PlaySoundW(L"MyRevolverAtt.wav", CSoundMgr::CHANNELID::UI);
	}
}

void CGun::AttackCameraShaking()
{
	if (m_bAtt && m_fAttTime < 0.1f)
	{
		m_fShakeValue += 1.f - 40.f * GET_TIME;
		
		if (fabs(m_tGunInfo.m_tAngle.fLookAngle) < 90.f)
			CScrollMgr::Set_Scroll(m_tInfo.vDir * m_fShakeValue);
		else
			CScrollMgr::Set_Scroll(-m_tInfo.vDir * m_fShakeValue);
	}
}

HRESULT CGun::Initialize()
{
	m_eType = OBJ::GUN;
	m_tInfo.iAtt = 10;
	m_tInfo.vSize = { 16.f, 8.f, 0.f };
	m_fAttSpeed = 5.f;
	m_fAttTime = 0.f;
	m_bAtt = false;
	m_tGunInfo.m_tAngle.fSwingAngle = 0.f;
	m_tGunInfo.m_tAngle.fLookAngle = 0.f;
	m_tGunInfo.m_iBulletCount = 10;
	Set_Frame(L"Revolver", L"Rare", 0.f, 1.f);
	m_bReload = true;
	m_bDoubleShot = false;
	m_fReloadTime = 0.f;
	m_fShakeValue = 0.f;
	return S_OK;
}

void CGun::Release()
{
}

void CGun::SaveOldInfo()
{
	CObj::SaveOldInfo();
	m_lstOldInfoEx.emplace_back(m_tGunInfo);
	if (m_lstOldInfo.size() > 100)
		m_lstOldInfo.pop_front();
}

CGun * CGun::Create()
{
	CGun* pInstance = new CGun;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
