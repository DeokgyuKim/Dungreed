#pragma once

#define WINCX 320
#define WINCY 180

#define RATIO			4
#define REVERSE_RATIO	0.25f

#define WINCX_TOOL	1280
#define WINCY_TOOL	720

#define RUBY		0x01/* 0000 0001*/
#define SAPPHIRE	0x02/* 0000 0010*/
#define DIAMOND		0x04/* 0000 0100*/

#define PI			3.141592f

#define TILE_COUNT_X 40
#define TILE_COUNT_Y 30

#define TILECX 16
#define TILECY 16

#define NO_EVENT		0
#define DEAD_OBJ		1
#define REVERSE_END		2
#define LOAD_FAIL		3
#define DEAD_BOSS		4

#define NULL_CHECK(ptr) if(nullptr == (ptr)) return;
#define NULL_CHECK_RETURN(ptr, val) if(nullptr == (ptr)) return val;
#define NULL_CHECK_MSG(ptr, msg) if(nullptr == (ptr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return; }
#define NULL_CHECK_MSG_RETURN(ptr, msg, val) if(nullptr == (ptr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return val; }

#define FAILED_CHECK(hr) if(FAILED(hr)) return;
#define FAILED_CHECK_RETURN(hr, val) if(FAILED(hr)) return val;
#define FAILED_CHECK_MSG(hr, msg) if(FAILED(hr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return; }
#define FAILED_CHECK_MSG_RETURN(hr, msg, val) if(FAILED(hr)) { ::MessageBox(0, msg, L"System Error", MB_OK); return val; }

#define NO_COPY(ClassName)						\
private:										\
	ClassName(const ClassName&);				\
	ClassName operator=(const ClassName&);		\

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* GetInstance();			\
	void DestroyInstance();						\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::GetInstance()				\
{												\
	if(nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
void ClassName::DestroyInstance()				\
{												\
	if(m_pInstance)								\
	{											\
		delete m_pInstance;						\
		m_pInstance = nullptr;					\
	}											\
}

//TYPE
#define VEC3			D3DXVECTOR3
#define VEC3NORMAL		D3DXVec3Normalize
#define VEC3DOT			D3DXVec3Dot
#define VEC3CROSS		D3DXVec3Cross
#define VEC3LENGTH		D3DXVec3Length
#define VEC3_T_COORD	D3DXVec3TransformCoord
#define VEC3_T_NORMAL	D3DXVec3TransformNormal

#define MATRIX			D3DXMATRIX
#define MATROTATIONZ	D3DXMatrixRotationZ
#define MATI			D3DXMatrixIdentity
#define MATSCAILING		D3DXMatrixScaling
#define MATTRANSLATION	D3DXMatrixTranslation

#define TORADIAN		D3DXToRadian
#define TODEGREE		D3DXToDegree

//STR
#define MIN_STR			64
#define MID_STR			128
#define MAX_STR			256

//ScrollMgr
#define SCRX			+ CScrollMgr::Get_Scroll().x
#define SCRY			+ CScrollMgr::Get_Scroll().y

//Move
#define SPEED(speed, time)	speed * time

//4. CAST
#define SCAST(type, value) static_cast<type>(value)
#define DCAST(type, value) dynamic_cast<type>(value)

#define GET_TIME		CTimeMgr::GetInstance()->GetDeltaTime()
#define GET_PLAYER_OB	DCAST(CPlayerObserver*, m_pObserver)

//5. BOSS
#define BOSS_HP 1000

//6. PLAYER
#define PLAYER_HP 74

//7. Weapon
#define RELOAD_TIME 1.f

//8. Scene
#define SCENE_CHANGE -1