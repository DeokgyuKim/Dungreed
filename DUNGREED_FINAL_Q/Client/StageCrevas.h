#pragma once
#include "Scene.h"
class CStageCrevas :
	public CScene
{
public:
	CStageCrevas();
	virtual ~CStageCrevas();
public:
	virtual HRESULT Initiailize() override;
	virtual int		Update() override;
	virtual void	LateUpdate() override;
	virtual void	Render() override;
	virtual void	Release() override;
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

	bool			m_bSeal;
};

