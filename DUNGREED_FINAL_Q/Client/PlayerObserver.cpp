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
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_matPlayer;
}

const VEC3 & CPlayerObserver::GetPlayerPos() const
{
	return m_vPlayerPos;
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
}

const VEC3 & CPlayerObserver::GetPlayerDir() const
{
	return m_vPlayerDir;
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
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
	// �������� ����
	// 1.Ǫ�� ������: ������Ʈ���� �������鿡�� �����͸� Ǫ���ϴ� ����.
	// 2.Ǯ(Pull) ������: �������� ������Ʈ���� �����͸� ������ ����.

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
