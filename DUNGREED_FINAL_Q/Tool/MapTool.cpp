// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "afxdialogex.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_byDrawID(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtrl);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelectTile)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


// WM_DROPFILES 메시지 발생할 때 호출됨.
void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	//::AfxMessageBox(L"파일 가져다 놓았니?");

	TCHAR szFileName[MAX_STR] = L"";
	TCHAR szRelative[MAX_STR] = L"";

	// 두번째 인자에 -1을 전달하면 드래그 앤 드롭된 전체 대상의 갯수를 반환한다.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iCount; ++i)
	{
		// DragQueryFile(HDROP, index, filename, length)
		// 드래그 앤 드롭된 대상 중 index번째의 파일이름(경로포함)을 filename에 기록하는 함수.
		::DragQueryFile(hDropInfo, i, szFileName, MAX_STR);

		// 절대경로 -> 상대경로
		CDirectoryMgr::ConvertRelativePath(szRelative, szFileName);

		// 파일명만 남겨보기.
		TCHAR* pFileName = ::PathFindFileName(szRelative);
		lstrcpy(szFileName, pFileName);

		// 확장자 제거하기.
		::PathRemoveExtension(szFileName);

		m_ListBox.AddString(szFileName);
	}	

	SetHorizontalScrollSizes();
}

void CMapTool::SetHorizontalScrollSizes()
{
	CString strFileName = L"";
	CString strLonger = L"";
	int iLength = 0;

	// GetCount: 리스트박스 안에 들어있는 데이터의 전체 개수 반환.
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strFileName);

		if (iLength < strFileName.GetLength())
		{
			strLonger = strFileName;
			iLength = strLonger.GetLength();
		}
	}

	// 문자열이 가장 긴 녀석들 기준으로 수평 스크롤 크기를 조정한다.
	// 문자열의 길이를 픽셀 단위로 변환.
	CDC* pDC = m_ListBox.GetDC();

	// GetTextExtent: 현재 문자열의 크기를 픽셀단위로 변환해서 CSize로 반환함.
	int iCX = pDC->GetTextExtent(strLonger).cx;

	// GetHorizontalExtent: 현재 수평 스크롤 범위.
	if (iCX > m_ListBox.GetHorizontalExtent())
	{
		// 수평 스크롤 범위를 설정하는 함수.
		m_ListBox.SetHorizontalExtent(iCX);
	}	

	// GetDC로 얻어왔으면 반드시 ReleaseDC로 반환.
	m_ListBox.ReleaseDC(pDC);
}


void CMapTool::OnLbnSelectTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strFileName = L"";
	m_ListBox.GetText(iIndex, strFileName);

	int i = 0;

	for ( ; i < strFileName.GetLength(); ++i)
	{
		// 현재 문자가 숫자라면 true반환. 글자라면 false반환.
		if (isdigit(strFileName[i]))
			break;
	}

	// CString::Delete(index, count): 현재 문자열의 index부터 count만큼 제거.
	strFileName.Delete(0, i);

	// _ttoi: 문자 -> 정수로 변환.
	m_byDrawID = _ttoi(strFileName);

	// 선택된 타일 미리보기 기능
	const TEX_INFO* pTexInfo = m_pTextureMgr->Get_TextInfo(L"Terrain", L"Tile", m_byDrawID);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, (float)WINCX / TILECX, (float)WINCY / TILECY, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	m_pDeviceMgr->Render_Begin();

	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDeviceMgr->Render_End(m_PictureCtrl.m_hWnd);
}


void CMapTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files|*.dat|Text Files|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		CTileMgr::GetInstance()->SaveData(Dlg.GetPathName());
	}
}


void CMapTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files|*.dat|Text Files|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CToolView* pToolView = dynamic_cast<CToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		CTileMgr::GetInstance()->LoadData(Dlg.GetPathName());
		pToolView->Invalidate(FALSE);
	}
}
