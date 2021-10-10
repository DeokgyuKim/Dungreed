#pragma once
class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	virtual HRESULT Initialize()PURE;
	virtual int		Update()	PURE;
	virtual void	LateUpdate()PURE;
	virtual void	Render()	PURE;
	virtual void	Release()	PURE;
public:
	void Render(MATRIX* matWorld, RENDER::POS ePos = RENDER::LEFTTOP);
public:
	virtual int		ReverseUpdate();
	virtual void	ReverseLateUpdate();
public:
	inline virtual const INFO*	Get_Info() const { return &m_tInfo; }
	inline virtual const RECT*	Get_Rect() const { return &m_tRect; }
	inline OBJ::OBJ_TYPE		Get_Type() const { return m_eType; }
	inline FRAME				Get_Frame() const { return m_tFrame; }
public:
	inline void	Set_Dir(VEC3 vDir) { m_tInfo.vDir = vDir; }
	inline virtual void	Set_Pos(VEC3 vPos) { m_tInfo.vPos = vPos; }
	inline void	Set_Size(int CX, int CY) { m_tInfo.vSize = { (float)CX, (float)CY, 0.f }; }
	inline void Set_Att(int iAtt) { m_tInfo.iAtt = iAtt; }
	inline void Set_Frame(TCHAR* pObject, TCHAR* pState, float fStart, float fCount) {
		m_tFrame.pObjectKey = pObject;
		m_tFrame.pStateKey = pState;
		m_tFrame.fStart = fStart;
		m_tFrame.fCount = fCount;
		m_tFrame.bReverse = false;
	}
	inline void Set_Frame_Reverse(bool bReverse) { m_tFrame.bReverse = bReverse; }
	inline void Move_Pos(VEC3 vPos) { m_tInfo.vPos += vPos; }
	inline void Set_Dead(bool bDead) { m_tInfo.bDead = bDead; }
	inline void Set_Speed() { m_tInfo.fSpeed *= 0.25f; }
public:
	virtual void	FrameUpdate();
	virtual void	UpdateRect();
	virtual void	Collision(OBJ::OBJ_TYPE eType, vector<void*>* pVecValue) {}
	virtual void	DeleteOldInfo() { m_lstOldInfo.clear(); m_lstOldInfo.push_back(make_pair(m_tInfo, m_tFrame)); }
protected:
	bool			CheckDead();
public:
	virtual void	SaveOldInfo() {
		if (m_tInfo.bDead)
			return;
		m_lstOldInfo.emplace_back(make_pair(m_tInfo, m_tFrame));
		if (m_lstOldInfo.size() > 100)
			m_lstOldInfo.pop_front();
	}
protected:
	OBJ::OBJ_TYPE			m_eType;
	INFO					m_tInfo;
	RECT					m_tRect;
	FRAME					m_tFrame;
	list<pair<INFO, FRAME>>	m_lstOldInfo;
};

