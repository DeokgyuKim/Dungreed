#include "stdafx.h"
#include "Dart.h"
#include "PlayerObserver.h"
#include "DartBullet.h"

#include "SoundMgr.h"

CDart::CDart()
{
}


CDart::~CDart()
{
}

int CDart::Update()
{
	m_tInfo.vPos = GET_PLAYER_OB->GetPlayerPos();
	if (GET_PLAYER_OB->GetPlayerRenderRight())
		m_tInfo.vPos += VEC3(5.f, -6.f, 0.f);
	else
		m_tInfo.vPos -= VEC3(5.f, 6.f, 0.f);

	m_tInfo.vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
	VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);

	CObj::UpdateRect();



	if (m_bAtt)
	{
		if (CObjectMgr::GetInstance()->CountHitBox(HITBOX::DART) == 0)
		{
			CSoundMgr::Get_Instance()->PlaySoundW(L"MySwing.wav", CSoundMgr::PLAYER);
			m_bAtt = false;
			m_fAttTime = 0.f;
			m_pDartBullet = nullptr;
		}
	}
	if (m_pDartBullet != nullptr)
	{
		if (m_pDartBullet->Get_Info()->fSpeed < 0.f)
		{
			RECT rc = {};
			if (IntersectRect(&rc, &m_tRect, m_pDartBullet->Get_Rect()))
			{
				m_pDartBullet->Set_Dead(true);
				m_pDartBullet = nullptr;
				m_bAtt = false;
				m_fAttTime = 0.f;
				m_pDartBullet = nullptr;
			}
		}
	}


	return NO_EVENT;
}

void CDart::LateUpdate()
{
	SaveOldInfo();
	AttackCameraShaking();
}

void CDart::Render()
{
	if (!m_bAtt)
	{
		//matWorld
		MATRIX	matRotateZ, matTrans, matWorld;
		float fCos = VEC3DOT(&m_tInfo.vDir, &VEC3(1.f, 0.f, 0.f));
		float fAcos = acosf(fCos);
		fAcos = TODEGREE(fAcos);

		if (m_tInfo.vDir.y < 0)
			fAcos = 360.f - fAcos;

		MATROTATIONZ(&matRotateZ, TORADIAN(fAcos));
		VEC3 vScroll = CScrollMgr::Get_Scroll();
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
		matWorld = matRotateZ * matTrans;

		//텍스쳐 렌더
		CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::MID);
	}
}

void CDart::Attack()
{
	if (!m_bAtt)
	{
		m_bAtt = true;
		m_fAttTime = 0.f;
		//표창생성
		m_pDartBullet = CDartBullet::Create(m_tInfo.vPos, m_tInfo.vDir, m_tInfo.iAtt, HITBOX::DART);
		CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, m_pDartBullet);
	}
}

void CDart::AttackCameraShaking()
{
}

HRESULT CDart::Initialize()
{
	m_eType = OBJ::DART;
	m_tInfo.iAtt = 3;
	m_tInfo.vSize = { 37.f, 37.f, 0.f };
	m_fAttSpeed = 0.2f;
	m_fAttTime = 0.f;
	m_bAtt = false;
	m_tInfo.m_iGold = 1500;
	Set_Frame(L"Bomerang", L"BomerangIdle", 0.f, 1.f);
	m_pDartBullet = nullptr;
	return S_OK;
}

void CDart::Release()
{
}

CDart * CDart::Create()
{
	CDart* pInstance = new CDart;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
