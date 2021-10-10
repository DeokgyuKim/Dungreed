// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),
	m_MySheet(L"MySheet")
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedUnitTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnBnClickedPathTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnBnClickedMySheet)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	// MFC 폰트 생성
	m_Font.CreatePointFont(180, L"궁서");

	// 대화상자에 배치되어있는 오브젝트를 얻어오는 함수.
	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);

	m_UnitTool.Create(IDD_UNITTOOL);
	m_MapTool.Create(IDD_MAPTOOL);
	m_PathTool.Create(IDD_PATHTOOL);
	m_MySheet.Create(this, WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXTHELP);
}


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//AfxMessageBox(L"UnitTool 버튼을 클릭했습니다");

	if (nullptr != m_UnitTool.GetSafeHwnd())
	{
		// SW_SHOW: 창모양으로 출력.
		m_UnitTool.ShowWindow(SW_SHOW);
	}	
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_MapTool.GetSafeHwnd())
	{
		// SW_SHOW: 창모양으로 출력.
		m_MapTool.ShowWindow(SW_SHOW);
	}
}


void CMyForm::OnBnClickedPathTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_PathTool.GetSafeHwnd())
	{
		// SW_SHOW: 창모양으로 출력.
		m_PathTool.ShowWindow(SW_SHOW);
	}
}


void CMyForm::OnBnClickedMySheet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr != m_MySheet.GetSafeHwnd())
	{
		// SW_SHOW: 창모양으로 출력.
		m_MySheet.ShowWindow(SW_SHOW);
	}
}
