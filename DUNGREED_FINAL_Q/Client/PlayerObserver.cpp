#include "stdafx.h"
#include "PlayerObserver.h"


CPlayerObserver::CPlayerObserver()
	: m_iPlayerHp(0), m_iPlayerMp(0)
{
	D3DXMatrixIdentity(&m_matPlayer);
}


CPlayerObserver::~CPlayerObserver()
{
}

int CPlayerObserver::GetPlayerHp() const
{
	return m_iPlayerHp;
}

int CPlayerObserver::GetPlayerMp() const
{
	return m_iPlayerMp;
}

const D3DXMATRIX& CPlayerObserver::GetPlayerMatrix() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matPlayer;
}

const VEC3 & CPlayerObserver::GetPlayerPos() const
{
	return m_vPlayerPos;
	// TODO: 여기에 반환 구문을 삽입합니다.
}

const VEC3 & CPlayerObserver::GetPlayerDir() const
{
	return m_vPlayerDir;
	// TODO: 여기에 반환 구문을 삽입합니다.
}

const bool CPlayerObserver::GetPlayerRenderRight() const
{
	return m_bPlayerRenderRight;
}

const CPlayer::PLAYER_STATE CPlayerObserver::GetPlayerState() const
{
	return m_eState;
}

const PLAYER::TYPE CPlayerObserver::GetPlayerType() const
{
	return m_ePlayerType;
}

void CPlayerObserver::Update(int iMessage, void* pData)
{
	// 옵저버의 형태
	// 1.푸쉬 옵저버: 서브젝트에서 옵저버들에게 데이터를 푸쉬하는 형태.
	// 2.풀(Pull) 옵저버: 옵저버가 서브젝트에서 데이터를 얻어오는 형태.

	switch (iMessage)
	{
	case DATA_PLAYER_HP:
		m_iPlayerHp = *reinterpret_cast<int*>(pData);
		break;
	case DATA_PLAYER_MP:
		m_iPlayerMp = *reinterpret_cast<int*>(pData);
		break;
	case DATA_PLAYER_MATRIX:
		m_matPlayer = *reinterpret_cast<D3DXMATRIX*>(pData);
		break;
	case DATA_PLAYER_POS:
		m_vPlayerPos = *reinterpret_cast<VEC3*>(pData);
		break;
	case DATA_PLAYER_DIR:
		m_vPlayerDir = *reinterpret_cast<VEC3*>(pData);
		break;
	case DATA_PLAYER_RENDERRIGHT:
		m_bPlayerRenderRight = *reinterpret_cast<bool*>(pData);
		break;
	case DATA_PLAYER_STATE:
		m_eState = *reinterpret_cast<CPlayer::PLAYER_STATE*>(pData);
		break;
	case DATA_PLAYER_TYPE:
		m_ePlayerType = *reinterpret_cast<PLAYER::TYPE*>(pData);
		break;
	default:
		break;
	}
}

CObserver* CPlayerObserver::Create()
{
	return new CPlayerObserver;
}
