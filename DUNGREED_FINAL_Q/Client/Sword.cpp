#include "stdafx.h"
#include "Sword.h"
#include "PlayerObserver.h"
#include "HitBox.h"

#include "SoundMgr.h"

CSword::CSword()
{
}


CSword::~CSword()
{
	Release();
}

int CSword::Update()
{
	//위치
	VEC3 vPos = GET_PLAYER_OB->GetPlayerPos();
	bool bRenderRight = GET_PLAYER_OB->GetPlayerRenderRight();

	if (bRenderRight)
		m_tInfo.vPos = vPos + VEC3(5.f, -6.f, 0.f);
	else
		m_tInfo.vPos = vPos - VEC3(5.f, 6.f, 0.f);

	//마우스와 방향
	float fAcos = 0.f;
	if (bRenderRight)
	{
		VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
		VEC3NORMAL(&vDir, &vDir);
		m_tInfo.vDir = vDir;
		float fCos = VEC3DOT(&vDir, &VEC3(1.f, 0.f, 0.f));
		fAcos = acosf(fCos);
		fAcos = TODEGREE(fAcos);
		if (vDir.y < 0)
			fAcos = 360.f - fAcos;
	}
	else
	{
		VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
		VEC3NORMAL(&vDir, &vDir);
		m_tInfo.vDir = vDir;
		float fCos = VEC3DOT(&vDir, &VEC3(-1.f, 0.f, 0.f));
		fAcos = acosf(fCos);
		fAcos = TODEGREE(fAcos);
		if (vDir.y > 0)
			fAcos = 360.f - fAcos;
	}


	m_tAngle.fLookAngle = fAcos;

	IsSwing();

	return 0;
}

void CSword::LateUpdate()
{
	AttackCameraShaking();
	SaveOldInfo();
}

void CSword::Render()
{
	//matWorld
	MATRIX	matScale, matRotateZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	if(GET_PLAYER_OB->GetPlayerRenderRight())
		D3DXMatrixRotationZ(&matRotateZ, TORADIAN(m_tAngle.fLookAngle + m_tAngle.fSwingAngle));
	else
		D3DXMatrixRotationZ(&matRotateZ, TORADIAN(m_tAngle.fLookAngle + (360.f - m_tAngle.fSwingAngle)));
	VEC3 vScroll = CScrollMgr::Get_Scroll();
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matRotateZ * matTrans;

	//텍스쳐 렌더
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart));

}

int CSword::ReverseUpdate()
{
	CObj::ReverseUpdate();
	if (m_lstOldInfoEx.size() > 0)
	{
		m_tAngle = m_lstOldInfoEx.back();
		m_lstOldInfoEx.pop_back();
		if (m_lstOldInfoEx.size() > 0)
			m_lstOldInfoEx.pop_back();
		if (m_lstOldInfoEx.empty())
			m_lstOldInfoEx.emplace_back(m_tAngle);
	}
	return NO_EVENT;
}

void CSword::ReverseLateUpdate()
{
	CObj::ReverseLateUpdate();
}

void CSword::Attack()
{
	if (!m_bAtt)
	{
		m_bAtt = true;
		m_fAttTime = 0.f;
		m_fShakeValue = 0.f;
		VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
		VEC3NORMAL(&vDir, &vDir);
		CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CHitBox::Create(m_tInfo.vPos + VEC3(0.f, 16.f, 0.f) + vDir * 20, VEC3(30.f, 30.f, 0.f), vDir, m_tInfo.iAtt, HITBOX::PLAYER));
		CSoundMgr::Get_Instance()->PlaySoundW(L"MySwing.wav", CSoundMgr::CHANNELID::EFFECT);
	}
}

void CSword::AttackCameraShaking()
{
	if (m_bAtt && m_fAttTime < (1.f / m_fAttSpeed) / 3.f)
	{
		m_fShakeValue += 1.f - 40.f * GET_TIME;
		MATRIX matRotate;
		VEC3 vDir;
		MATROTATIONZ(&matRotate, TORADIAN(m_tAngle.fLookAngle));
		VEC3_T_COORD(&vDir, &VEC3(1.f, 0.f, 0.f), &matRotate);
		VEC3NORMAL(&vDir, &vDir);
		VEC3 ResultDir = vDir;

		if(m_bUpper)
			VEC3CROSS(&ResultDir, &ResultDir, &VEC3(0.f, 0.f, 1.f));
		else
			VEC3CROSS(&ResultDir, &ResultDir, &VEC3(0.f, 0.f, -1.f));

		VEC3NORMAL(&ResultDir, &ResultDir);

		ResultDir = ResultDir + vDir * 2.f;
		VEC3NORMAL(&ResultDir, &ResultDir);


		CScrollMgr::Set_Scroll(-ResultDir * m_fShakeValue);
	}
}

HRESULT CSword::Initialize()
{
	m_eType = OBJ::GREATE_SWORD;
	m_tInfo.iAtt = 20;
	m_tInfo.vSize = { 13.f, 32.f, 0.f };
	m_fAttSpeed = 2.f;
	m_fAttTime = 0.f;
	m_fShakeValue = 0.f;
	m_bAtt = false;
	m_bUpper = false;
	m_tAngle.fSwingAngle = 0.f;
	m_tAngle.fLookAngle = 0.f;
	Set_Frame(L"NormalSword", L"Create", 0.f, 1.f);
	return S_OK;
}

void CSword::Release()
{
}

void CSword::IsSwing()
{
	if (m_bAtt)
	{
		if (GET_PLAYER_OB->GetPlayerType() == PLAYER::RAPIERMAN_PLAYER)
		{
			if (m_bUpper)
			{
				if (m_fAttTime >= 1.f / m_fAttSpeed)
				{
					m_bAtt = false;
				}
				m_tAngle.fSwingAngle -= 180 * m_fAttSpeed * GET_TIME * 5.f;
				if (m_tAngle.fSwingAngle < -0.1f)
					m_tAngle.fSwingAngle = -0.1f;

				m_fAttTime += GET_TIME;
			}
			else
			{
				if (m_fAttTime >= 1.f / m_fAttSpeed)
				{
					m_fAttTime = 0.f;
					m_tAngle.fSwingAngle = 180.1f;
					m_bUpper = true;
					return;
				}
				m_tAngle.fSwingAngle += 180 * m_fAttSpeed * GET_TIME * 5.f;
				if (m_tAngle.fSwingAngle > 180.1f)
				{
					m_tAngle.fSwingAngle = 180.1f;
					m_bUpper = true;
					VEC3 vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
					VEC3NORMAL(&vDir, &vDir);
					CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CHitBox::Create(m_tInfo.vPos + VEC3(0.f, 16.f, 0.f) + vDir * 20, VEC3(30.f, 30.f, 0.f), vDir, m_tInfo.iAtt, HITBOX::PLAYER));
					CSoundMgr::Get_Instance()->PlaySoundW(L"MySwing.wav", CSoundMgr::CHANNELID::EFFECT);
				}
				m_fAttTime += GET_TIME;
			}
		}
		else
		{
			if (m_bUpper)
			{
				if (m_fAttTime >= 1.f / m_fAttSpeed)
				{
					m_fAttTime = 0.f;
					m_tAngle.fSwingAngle = 0.1f;
					m_bAtt = false;
					m_bUpper = false;
				}
				if (m_tAngle.fSwingAngle >= 0.f)
					m_tAngle.fSwingAngle -= 180 * m_fAttSpeed * GET_TIME * 5.f;
				else
					m_tAngle.fSwingAngle = -0.1f;
				m_fAttTime += GET_TIME;
			}
			else
			{
				if (m_fAttTime >= 1.f / m_fAttSpeed && m_tAngle.fSwingAngle == 180.1f)
				{
					m_fAttTime = 0.f;
					m_tAngle.fSwingAngle = 180.1f;
					m_bAtt = false;
					m_bUpper = true;
					return;
				}
				if (m_tAngle.fSwingAngle <= 180.f)
					m_tAngle.fSwingAngle += 180 * m_fAttSpeed * GET_TIME * 5.f;
				else
					m_tAngle.fSwingAngle = 180.1f;
				m_fAttTime += GET_TIME;
			}
		}
		cout << m_tAngle.fSwingAngle << ", " << m_tAngle.fLookAngle << endl;
	}
	if (!m_bAtt)
	{
		if (GET_PLAYER_OB->GetPlayerType() == PLAYER::RAPIERMAN_PLAYER)
		{
			m_bUpper = false;
			m_tAngle.fSwingAngle = -0.1f;
			m_fAttTime = 0.f;
		}
	}
}

void CSword::SaveOldInfo()
{
	CObj::SaveOldInfo();
	m_lstOldInfoEx.emplace_back(m_tAngle);
	if (m_lstOldInfo.size() > 100)
		m_lstOldInfo.pop_front();
}

CSword * CSword::Create()
{
	CSword* pInstance = new CSword;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
