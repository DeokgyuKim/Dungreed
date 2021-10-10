#pragma once
class CObj;
class Command;
class CInputMgr
{
	DECLARE_SINGLETON(CInputMgr)
private:
	CInputMgr();
	~CInputMgr();
public:
	void	Initialize();
	void	GetKeyDown();
	void	Set_Actor(CObj* pObj);
	void	ChangeKey();
public:
	CObj*		Get_Actor() { return m_pActor; }
private:
	CObj*		m_pActor;
private:
	Command*	Jump;
	Command*	MoveLeft;
	Command*	MoveDown;
	Command*	MoveRight;
	Command*	Attack;
	Command*	Dash;


	DWORD		m_dwKeySet[KEY::END];
};

