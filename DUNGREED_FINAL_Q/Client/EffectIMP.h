#pragma once

// ±¸ÇöÃþ

class CEffectIMP
{
public:
	CEffectIMP();
	virtual ~CEffectIMP();

public:
	virtual int Update()		PURE;
	virtual void LateUpdate()	PURE;
	virtual void Render(const D3DXMATRIX& matWorld)		PURE;

private:
	virtual HRESULT Initialize() PURE;
	virtual void Release()		PURE;

protected:
	CTimeMgr*		m_pTimeMgr;
	CTextureMgr*	m_pTextureMgr;
	CDeviceMgr*		m_pDeviceMgr;
	wstring			m_wstrObjectKey;
	wstring			m_wstrStateKey;
};

