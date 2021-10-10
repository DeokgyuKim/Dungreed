#pragma once

class CMathMgr
{
private:
	CMathMgr();
	~CMathMgr();

public:
	static void MyMatrixIdentity(D3DXMATRIX* pOut);
	static void MyMatrixScaling(D3DXMATRIX* pOut, float x, float y, float z);
	static void MyMatrixRotationZ(D3DXMATRIX* pOut, float fRadian);
	static void MyMatrixTranslation(D3DXMATRIX* pOut, float x, float y, float z);
	static void MyTransformCoord(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, const D3DXMATRIX* pMat);
	static void MyTransformNormal(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn, const D3DXMATRIX* pMat);
};

