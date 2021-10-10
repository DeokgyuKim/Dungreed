#pragma once

const DWORD		KEY_DOWN		= 0x00000001;
const DWORD		KEY_UP			= 0x00000002;
const DWORD		KEY_LEFT		= 0x00000004;
const DWORD		KEY_RIGHT		= 0x00000008;

const DWORD		KEY_W			= 0x00000010;
const DWORD		KEY_A			= 0x00000020;
const DWORD		KEY_S			= 0x00000040;
const DWORD		KEY_D			= 0x00000080;

const DWORD		KEY_LBUTTON		= 0x00000100;
const DWORD		KEY_RBUTTON		= 0x00000200;
const DWORD		KEY_R			= 0x00000400;
const DWORD		KEY_Q			= 0x00000800;

const DWORD		KEY_I			= 0x00001000;
const DWORD		KEY_E			= 0x00002000;


class CKeyMgr
{
private:
	CKeyMgr() : m_dwCurrentKey(0), m_dwKeyDown(0), m_dwKeyUp(0) {}
	CKeyMgr(const CKeyMgr&) {}
	CKeyMgr operator=(const CKeyMgr&) {}
	~CKeyMgr() {}
private:
	static CKeyMgr*	m_pInstance;
public:
	static CKeyMgr*	GetInstance() {
		if (m_pInstance == NULL)
			m_pInstance = new CKeyMgr;
		return m_pInstance;
	}
	static void		DestroyInstance() {
		if (m_pInstance)
			delete m_pInstance;
		m_pInstance = NULL;
	}
private:
	DWORD	m_dwCurrentKey;
	DWORD	m_dwKeyUp;
	DWORD	m_dwKeyDown;
	DWORD	m_dwStartTime;
	list<pair<DWORD, int>> m_lstTimeKey;
public:
	void	Update();
	bool	KeyPressing(DWORD dwKey);
	bool	KeyDown(DWORD dwKey);
	bool	KeyUp(DWORD	dwKey);
};

