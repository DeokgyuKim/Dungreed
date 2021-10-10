#pragma once
class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9 GetDevice() const;
	LPD3DXSPRITE GetSprite() const;
	LPD3DXFONT GetFont() const;

public:
	HRESULT InitDevice(DISPLAY_MODE eMode);
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	void Release();

private:
	//IDirect3D9 COM 객체: 다이렉트 환경에서 사용할 장치를 조사하고 장치 오브젝트를 생성한다.
	//IDirect3D9의 포인터	
	LPDIRECT3D9	m_pSDK;

	// IDirect3DDevice9 COM 객체: 그래픽 카드 제어 관련 기능들(렌더링 등)을 제공한다.
	//IDirect3DDevice9의 포인터
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	// ID3DXSprite COM 객체: 다이렉트 환경에서 2D 텍스처 렌더링을 제공한다.
	// ID3DXSprite의 포인터
	LPD3DXSPRITE	m_pSprite;

	// 다이렉트 폰트: 2D 폰트 렌더링 COM객체
	LPD3DXFONT		m_pFont;

	// COM (Component Object Model)
	// 마이크로소프트사에서 고안한 프로그래밍 모델.
	// 프로그래밍 또한 부품 조립하듯 완성해낼 수 있다! -> 생산비용 최소화! 능률 증대!
	// COM객체 타입들은 보통 대문자 I로 시작됨. Interface의 약자.
};

