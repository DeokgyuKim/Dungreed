#pragma once
class CEffectMgr
{
	DECLARE_SINGLETON(CEffectMgr)
private:
	CEffectMgr();
	~CEffectMgr();
public:
	void	CreateEffect(EFFECTMGR::TYPE eType, VEC3 vPos, float fLifeTime, TCHAR* wstrObjectKey, TCHAR* wstrStateKey, 
		float fCount, float fRotate = 0.f, RENDER::POS ePos = RENDER::BOTTOM, bool RenderRight = true);
};

