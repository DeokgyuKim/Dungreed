#pragma once
#include "Obj.h"
class CNiflheim;
class CIcePillar :
	public CObj
{
public:
	CIcePillar();
	virtual ~CIcePillar();
public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static  CIcePillar* Create(CNiflheim* pOwner, int Num);
public:
	inline int GetNum() { return m_iNum; }
	void	RotateGong(float fAngle) { if(!m_tInfo.bDead) m_fGongAngle += fAngle; }
	void	RotateZa(float fAngle) { if (!m_tInfo.bDead) m_fZaAngle += fAngle; }
	void	SetDist(float fDist) { if (!m_tInfo.bDead) m_fDist = fDist; }
	void	PlusDist(float fDist) { if (!m_tInfo.bDead)m_fDist += fDist; }
	void	SetGongAngle(float fAngle) { m_fGongAngle = fAngle; }
	void	SetZaAngle(float fAngle) { m_fZaAngle = fAngle; }
	inline  float GetDist() { return m_fDist; }
	void	ShotIce();
	void	ShotDirIce(VEC3 vDir, float fSpeed);
	void	ShotTargetIce(VEC3 vPos);
	void	ShotTargetIceImmediately(VEC3 vPos);
public:
	virtual void	UpdateRect();
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue);
	inline virtual const RECT*	Get_Rect() const { return m_tCollRect; }
private:
	int			m_iNum;
	float		m_fGongAngle;
	float		m_fZaAngle;
	float		m_fDist;
	CNiflheim*	m_pOnwer;
	CObserver*	m_pObserver;
	const VEC3	m_pOrizin;
	bool		m_bCreate;
	bool		m_bDestroy;
	float		m_fTimeCount;
	RECT		m_tCollRect[3];
};

