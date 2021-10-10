#pragma once
class CInventory;
class CPlayer;
class CUiMgr
{
	DECLARE_SINGLETON(CUiMgr)
private:
	CUiMgr();
	~CUiMgr();
public:
	inline void	SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
public:
	void	RenderUi();
private:
	void	RenderHpBar();
	void	RenderDashBar();
	void	RenderDamagedEffect();
	void	RenderBossUi();
public:
	void	RenderInventroy();
private:
	inline void SetFrame(FRAME& tFrame, TCHAR* pObject, TCHAR* pState, float fStart, float fCount) {
		tFrame.pObjectKey = pObject;
		tFrame.pStateKey = pState;
		tFrame.fStart = fStart;
		tFrame.fCount = fCount;
		tFrame.bReverse = false;
	}
	inline void FrameUpdate(FRAME& tFrame)
	{
		tFrame.fStart += tFrame.fCount * CTimeMgr::GetInstance()->GetDeltaTime();

		if (tFrame.fStart > tFrame.fCount)
			tFrame.fStart = 0.f;
	}
private:
	CPlayer*		m_pPlayer;
	CObj*			m_pBoss;
	CInventory*		m_pInven;
	FRAME			m_tHpBarWaveFrame;
	MATRIX			matScale, matTrans, matWorld;
};

