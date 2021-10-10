#pragma once
class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)
private:
	CSceneMgr();
	~CSceneMgr();
public:
	enum SCENEID { SCENE_LOGO, SCENE_HOME, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3, SCENE_STAGE4, SCENE_STAGE5, SCENE_CREVAS, SCENE_END };
private:
	CScene*			m_pScene;
	SCENEID			m_eCurScene;
	SCENEID			m_eNextScene;
	int				m_iEvent;
public:
	inline SCENEID	Get_SceneID() { return m_eCurScene; }
	void			Update();
	void			LateUpdate();
	void			Render();
	void			Release();
public:
	void			ChangeScene(SCENEID eNextScene);
};

