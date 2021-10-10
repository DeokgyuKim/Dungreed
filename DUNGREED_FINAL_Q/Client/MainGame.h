#pragma once

class CObj;
class CMainGame
{
private:
	CMainGame();

public:
	~CMainGame();

public:	
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();
	void RenderFPS();

public:
	static CMainGame* Create();

private:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CKeyMgr*		m_pKeyMgr;
	CObjectMgr*		m_pObjectMgr;
	CTimeMgr*		m_pTimeMgr;
	CAstarMgr*		m_pAstarMgr;
	CSubject*		m_pSubject;
	CSceneMgr*		m_pSceneMgr;

	float			m_fTimeCount;
	int				m_iFPS;
	TCHAR			m_szFPS[MIN_STR];
};

