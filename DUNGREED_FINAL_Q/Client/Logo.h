#pragma once
#include "Scene.h"
class CLogo :
	public CScene
{
public:
	CLogo();
	virtual ~CLogo();
public:
	virtual HRESULT Initiailize() override;
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render() override;
	virtual void	Release() override;

	static unsigned __stdcall LoadingFunc(void* pParam);
private:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;
	CKeyMgr*		m_pKeyMgr;
	CObjectMgr*		m_pObjectMgr;
	CTimeMgr*		m_pTimeMgr;
	CAstarMgr*		m_pAstarMgr;
	CSubject*		m_pSubject;

	bool			m_bOpenScene;
	float			m_fTimeCountOpen;

	bool			m_bChangeScene;
	float			m_fTimeCountClose;

	float			m_fGateCount;

	int				m_iIndex;
	RECT			m_tRectDoor;

private:
	HANDLE m_hLoadingThread;
	CRITICAL_SECTION m_Critical;
};

