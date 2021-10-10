#include "stdafx.h"
#include "NiflDeadEffect.h"


CNiflDeadEffect::CNiflDeadEffect()
{
}


CNiflDeadEffect::~CNiflDeadEffect()
{
}

int CNiflDeadEffect::Update()
{
	if (m_fLifeTime >= m_fDeadTime)
		m_tInfo.bDead = true;
	if (m_tInfo.bDead)
	{
		Set_Frame(L"Boss", L"RealDead", 0.f, 1.f);
		m_fTimeCount += GET_TIME;
		if (m_fTimeCount >= 3.f)
			return DEAD_BOSS;
	}

	m_fLifeTime += GET_TIME;
	return NO_EVENT;
}

void CNiflDeadEffect::LateUpdate()
{
	CMyEffect::LateUpdate();
	CScrollMgr::Initialize(m_tInfo.vPos - VEC3{ WINCX * 0.5f, WINCY * 0.5f, 0.f });
}

HRESULT CNiflDeadEffect::Initialize()
{
	CMyEffect::Initialize();
	m_eEffType = EFFECTMGR::NIFLDEAD_EFFECT;
	m_fTimeCount = 0.f;
	return S_OK;
}

CNiflDeadEffect * CNiflDeadEffect::Create(VEC3 vPos, float fLifeTime, TCHAR * wstrObjectKey, TCHAR * wstrStateKey, float fCount)
{
	CNiflDeadEffect*	pInstance = new CNiflDeadEffect;
	if (FAILED(pInstance->Initialize()))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	pInstance->m_tInfo.vPos = vPos;
	pInstance->m_fLifeTime = 0.f;
	pInstance->m_fDeadTime = fLifeTime;
	pInstance->Set_Frame(wstrObjectKey, wstrStateKey, 0.f, fCount);
	pInstance->m_fLateCount = fLifeTime;

	return pInstance;
}
