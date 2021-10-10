#pragma once
#include "Player.h"
class CRapierMan :
	public CPlayer
{
private:
	CRapierMan();
	virtual ~CRapierMan();
protected:
	virtual HRESULT Initialize() override;
public:
	virtual int		ReverseUpdate() override;
	virtual void	ChangeState(PLAYER_STATE eState) {
		if (m_eState != eState)
		{
			switch (eState)
			{
			case CPlayer::IDLE:
				Set_Frame(L"RapierMan", L"CharIdle", m_tFrame.fStart, 4.f);
				break;
			case CPlayer::RUN:
				Set_Frame(L"RapierMan", L"CharRun", m_tFrame.fStart, 8.f);
				break;
			case CPlayer::JUMP:
				Set_Frame(L"RapierMan", L"CharJump", 0.f, 1.f);
				break;
			case CPlayer::DASH:
				Set_Frame(L"RapierMan", L"CharJump", 0.f, 1.f);
				break;
			default:
				break;
			}
			m_eState = eState;
			m_pSubject->Notify(DATA_PLAYER_STATE, &m_eState);
		}
	}
public:
	static CRapierMan* Create(VEC3 vPos, CWeapon* pWeapon);
};