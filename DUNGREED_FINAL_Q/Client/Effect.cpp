#include "stdafx.h"
#include "Effect.h"
#include "EffectIMP.h"

CEffect::CEffect()
	: m_pImp(nullptr)
{
}


CEffect::~CEffect()
{
	Release();
}

void CEffect::Release()
{
	SafeDelete(m_pImp);
}
