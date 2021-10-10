#pragma once
#include "Obj.h"
#include "StateMachine.h"
#include "SubItem.h"

class CWeapon;
class CPlayer :
	public CObj
{
public:
	enum PLAYER_STATE { IDLE, RUN, JUMP, DASH, END };
protected:
	CPlayer();
public:
	virtual ~CPlayer();
public:
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render() override;
public:
	virtual int		ReverseUpdate() override;
public:
	virtual void	MoveLeft();
	virtual void	MoveRight();
	virtual void	MoveDown();
	virtual void	Jump();
	virtual void	Attack();
	virtual void	Dash();
public:
	void			IsJumping();
	void			IsDashing();
	void			IsDamaged();
	void			NotCollision();
	void			MakeAfterImage();
public:
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
protected:
	virtual HRESULT Initialize() override;
	virtual void	Release() override;
public:
	static	CPlayer*	Create(VEC3 vPos, CWeapon* pWeapon);
public:
	inline bool		Get_RenderRight() { return m_bRenderRight; }
	inline bool		Get_NotCollision() { return m_bNotCollisionTile; }
	inline bool		Get_Dash() { return m_bDash; }
	inline bool		Get_DamagedRenderUi() { return m_bDamagedRenderUi; }
	inline int		Get_DashCount() { return m_iDashCount; }
	inline PLAYER::TYPE	Get_PlayerType() { return m_ePlayerType; }
	inline virtual void	Set_Pos(VEC3 vPos) { m_tInfo.vPos = vPos; 
	if (CInventory::GetInstance()->GetItem(m_iInvenIndex + 2) != nullptr)
		if (SCAST(CSubItem*, CInventory::GetInstance()->GetItem(m_iInvenIndex + 2))->GetSubItemType() == SUBITEM::BAT)
			CInventory::GetInstance()->GetItem(m_iInvenIndex + 2)->Initialize();
	m_lstOldInfo.clear();
	m_lstOldInfo.push_back(make_pair(m_tInfo, m_tFrame));
	//CInventory::GetInstance()->GetItem(m_iInvenIndex)->DeleteOldInfo();
	}
public:
	inline void		Observer_PlayerType(){ m_pSubject->Notify(DATA_PLAYER_TYPE, &m_ePlayerType); }
public:
	VEC3			Get_PlayerMovePos();
protected:
	bool			m_bRenderRight;
	bool			m_bJump;
	bool			m_bDash;
	bool			m_bNotCollisionTile;
	bool			m_bDamaged;
	bool			m_bDamagedRenderUi;
	CWeapon*		m_pWeapon;
	PLAYER_STATE	m_eState;
	PLAYER::TYPE	m_ePlayerType;
	float			m_fJumpTime;
	float			m_fDashTime;
	float			m_fNotCollisionTileTime;
	float			m_fDamagedTime;
	float			m_fAfterImageTime;
	float			m_fDashChargingTime;
	float			m_fMoveDustCreateTime;
	int				m_iDashCount;
	CSubject*		m_pSubject;
	CInventory*		m_pInven;
	static int		m_iInvenIndex;
public:
	void			Set_Idle() { ChangeState(IDLE); }
public:
	virtual void	ChangeState(PLAYER_STATE eState) {
		if (m_eState != eState)
		{
			switch (eState)
			{
			case CPlayer::IDLE:
				Set_Frame(L"Normal", L"CharIdle", m_tFrame.fStart, 4.f);
				break;
			case CPlayer::RUN:
				Set_Frame(L"Normal", L"CharRun", m_tFrame.fStart, 8.f);
				break;
			case CPlayer::JUMP:
				Set_Frame(L"Normal", L"CharJump", 0.f, 1.f);
				break;
			case CPlayer::DASH:
				Set_Frame(L"Normal", L"CharJump", 0.f, 1.f);
				break;
			default:
				break;
			}
			m_eState = eState;
			m_pSubject->Notify(DATA_PLAYER_STATE, &m_eState);
		}
	}
	PLAYER_STATE	GetState() { return m_eState; }
	const CWeapon*	GetWeapon();
};

