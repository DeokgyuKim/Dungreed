#include "stdafx.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
	: m_fTimeCount(0.f)
{
	QueryPerformanceCounter(&m_OldTime1);
	QueryPerformanceCounter(&m_OldTime2);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CPUTick);
}


CFrameMgr::~CFrameMgr()
{
}

bool CFrameMgr::LimitFPS(float fps)
{
	QueryPerformanceCounter(&m_CurTime);

	// 주기적으로 주파수 갱신.
	if (m_CurTime.QuadPart - m_OldTime2.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OldTime2 = m_CurTime;
	}

	m_fTimeCount += float(m_CurTime.QuadPart - m_OldTime1.QuadPart) / m_CPUTick.QuadPart;
	m_OldTime1 = m_CurTime;

	if (m_fTimeCount < 1.f / fps)
		return false;

	m_fTimeCount -= 1.f / fps;
	return true;
}
