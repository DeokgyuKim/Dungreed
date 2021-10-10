#pragma once
#include "Player.h"
class CGunMan :
	public CPlayer
{
private:
	CGunMan();
	virtual ~CGunMan(); 
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
				Set_Frame(L"GunMan", L"CharIdle", m_tFrame.fStart, 4.f);
				break;
			case CPlayer::RUN:
				Set_Frame(L"GunMan", L"CharRun", m_tFrame.fStart, 8.f);
				break;
			case CPlayer::JUMP:
				Set_Frame(L"GunMan", L"CharJump", 0.f, 1.f);
				break;
			case CPlayer::DASH:
				Set_Frame(L"GunMan", L"CharJump", 0.f, 1.f);
				break;
			default:
				break;
			}
			m_eState = eState;
			m_pSubject->Notify(DATA_PLAYER_STATE, &m_eState);
		}
	}
public:
	static CGunMan* Create(VEC3 vPos, CWeapon* pWeapon);
};

