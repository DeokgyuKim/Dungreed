#pragma once
class CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	CFrameMgr();
	~CFrameMgr();

public:
	bool LimitFPS(float fps);

private:
	LARGE_INTEGER	m_OldTime1;
	LARGE_INTEGER	m_OldTime2;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CPUTick;

	float			m_fTimeCount;
};

