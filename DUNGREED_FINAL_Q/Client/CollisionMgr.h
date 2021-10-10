#pragma once
class CCollisionMgr
{
private:
	CCollisionMgr();
	NO_COPY(CCollisionMgr)
	~CCollisionMgr();
public:
	static void	Collision_Sphere(list<CObj*>* pDst, list<CObj*>* pSrc);
	static void	Collision_Rect(list<CObj*>* pDst, list<CObj*>* pSrc);
	static void	Collision_SphereRect(list<CObj*>* pDst, list<CObj*>* pSrc);
	static void Collision_Tile(list<CObj*>* pDst);
	static void Collision_Player(list<CObj*>* pDst);
private:
	static bool	CheckSphere(const INFO* pDst, const INFO* pSrc);
	static bool	CheckSphereRect(const INFO* pSphere, const RECT* pRc);
};

