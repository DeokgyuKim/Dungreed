#pragma once
class CScene
{
public:
	CScene();
	virtual ~CScene();
public:
	virtual HRESULT Initiailize()	PURE;
	virtual int		Update()		PURE;
	virtual void	LateUpdate()	PURE;
	virtual void	Render()		PURE;
	virtual void	Release()		PURE;
};

