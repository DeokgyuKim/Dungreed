#include "stdafx.h"
#include "Stamp.h"
#include "StarBullet.h"
#include "PlayerObserver.h"

#include "SoundMgr.h"

CStamp::CStamp()
{
}


CStamp::~CStamp()
{
	Release();
}

int CStamp::Update()
{
	m_tInfo.vPos = GET_PLAYER_OB->GetPlayerPos();
	if (GET_PLAYER_OB->GetPlayerRenderRight())
		m_tInfo.vPos += VEC3(5.f, -6.f, 0.f);
	else
		m_tInfo.vPos -= VEC3(5.f, 6.f, 0.f);

	m_tInfo.vDir = (CMouse::GetMousePos() + CScrollMgr::Get_Scroll()) - m_tInfo.vPos;
	VEC3NORMAL(&m_tInfo.vDir, &m_tInfo.vDir);

	if (m_bAtt)
	{
		m_fAttTime += GET_TIME;
		if (m_fAttTime >= 1.f / m_fAttSpeed)
		{
			m_fAttTime = 0.f;
			m_bAtt = false;
		}
	}

	return NO_EVENT;
}

void CStamp::LateUpdate()
{
	SaveOldInfo();
	AttackCameraShaking();
}

void CStamp::Render()
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

	//ÅØ½ºÃÄ ·»´õ
	CTextureMgr::GetInstance()->Render(MULTI_TEXTURE, m_tFrame.pObjectKey, &matWorld, m_tFrame.pStateKey, int(m_tFrame.fStart), 255, RENDER::LEFT);
}

void CStamp::Attack()
{
	if (!m_bAtt)
	{
		m_bAtt = true;
		m_fAttTime = 0.f;
		//ÃÑ¾Ë»ý¼º
		CObjectMgr::GetInstance()->AddObject(OBJ_PLAYER_HITBOX, CStarBullet::Create(m_tInfo.vPos, m_tInfo.vDir, m_tInfo.iAtt, HITBOX::PLAYER));

		CSoundMgr::Get_Instance()->PlaySoundW(L"MySpawnIceBullet.wav", CSoundMgr::PLAYER);
	}
}

void CStamp::AttackCameraShaking()
{
}

HRESULT CStamp::Initialize()
{
	m_eType = OBJ::STAMP;
	m_tInfo.iAtt = 40;
	m_tInfo.vSize = { 13.f, 13.f, 13.f };
	m_fAttSpeed = 3.f;
	m_fAttTime = 0.f;
	m_bAtt = false;
	m_tInfo.m_iGold = 1000;
	Set_Frame(L"Stamp", L"Stamp", 0.f, 1.f);
	return S_OK;
}

void CStamp::Release()
{
}

CStamp * CStamp::Create()
{
	CStamp* pInstance = new CStamp;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}
