#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = NULL;

void CKeyMgr::Update()
{
	m_dwCurrentKey = 0;

	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurrentKey |= KEY_W;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurrentKey |= KEY_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurrentKey |= KEY_S;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurrentKey |= KEY_D;

	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurrentKey |= KEY_R;

	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurrentKey |= KEY_Q;

	if (GetAsyncKeyState('I') & 0x8000)
		m_dwCurrentKey |= KEY_I;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwCurrentKey |= KEY_E;

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurrentKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurrentKey |= KEY_RBUTTON;

}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurrentKey & dwKey)
		return true;
	return false;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	//현재 눌렸고, 이전에 눌리지 않음 
	if ((m_dwCurrentKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	if (!(m_dwCurrentKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
	}
	return false;
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	//현재 안눌렷고, 이전에 눌림
	if (!(m_dwCurrentKey & dwKey) && (m_dwKeyUp & dwKey))
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	if ((m_dwCurrentKey & dwKey) && !(m_dwKeyUp & dwKey))
	{
		m_dwKeyUp |= dwKey;
	}
	return false;
}
