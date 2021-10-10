#include "stdafx.h"
#include "EffectMgr.h"
#include "AlphaEffect.h"
#include "ResourceEffect.h"
#include "NiflDeadEffect.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr()
{
}


CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::CreateEffect(EFFECTMGR::TYPE eType, VEC3 vPos, float fLifeTime, TCHAR * wstrObjectKey, TCHAR * wstrStateKey,
	float fCount, float fRotate, RENDER::POS ePos, bool bRenderRight)
{
	switch (eType)
	{
	case EFFECTMGR::ALPHA_EFFECT:
		CObjectMgr::GetInstance()->AddObject(OBJECT_TYPE::OBJ_EFFECT, 
			CAlphaEffect::Create(vPos, fLifeTime, wstrObjectKey, wstrStateKey, fCount, bRenderRight));
		break;
	case EFFECTMGR::RESOURCE_EFFECT:
		CObjectMgr::GetInstance()->AddObject(OBJECT_TYPE::OBJ_EFFECT,
			CResourceEffect::Create(vPos, fLifeTime, wstrObjectKey, wstrStateKey, fCount, fRotate, ePos));
		break;
	case EFFECTMGR::NIFLDEAD_EFFECT:
		CObjectMgr::GetInstance()->AddObject(OBJECT_TYPE::OBJ_EFFECT,
			CNiflDeadEffect::Create(vPos, fLifeTime, wstrObjectKey, wstrStateKey, fCount));
		break;
	case EFFECTMGR::END_EFFECT:
		CObjectMgr::GetInstance()->AddObject(OBJECT_TYPE::OBJ_EFFECT,
			CMyEffect::Create(vPos, fLifeTime, wstrObjectKey, wstrStateKey, fCount));
		break;
	default:
		break;
	}
}
