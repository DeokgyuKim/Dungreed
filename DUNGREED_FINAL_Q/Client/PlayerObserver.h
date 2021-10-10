#pragma once
#include "Observer.h"
#include "Player.h"

class CPlayerObserver :
	public CObserver
{
private:
	CPlayerObserver();

public:
	virtual ~CPlayerObserver();

public:
	int							GetPlayerHp() const;
	int							GetPlayerMp() const;
	const D3DXMATRIX&			GetPlayerMatrix() const;
	const VEC3&					GetPlayerPos() const;
	const VEC3&					GetPlayerDir() const;
	const bool					GetPlayerRenderRight() const;
	const CPlayer::PLAYER_STATE GetPlayerState() const;
	const PLAYER::TYPE			GetPlayerType() const;

public:
	virtual void Update(int iMessage, void* pData) override;

public:
	static CObserver* Create();

private:
	int						m_iPlayerHp;
	int						m_iPlayerMp;
	D3DXMATRIX				m_matPlayer;
	VEC3					m_vPlayerPos;
	VEC3					m_vPlayerDir;
	bool					m_bPlayerRenderRight;
	CPlayer::PLAYER_STATE	m_eState;
	PLAYER::TYPE			m_ePlayerType;
};

