// PathTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "afxdialogex.h"
#include "DirectoryMgr.h"


// CPathTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHTOOL, pParent)
{

}

CPathTool::~CPathTool()
{
	for_each(m_PathInfoList.begin(), m_PathInfoList.end(), SafeDelete<PATH_INFO*>);
	m_PathInfoList.clear();
}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}

void CPathTool::SetHorizontalScrollSizes()
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


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CPathTool 메시지 처리기입니다.


void CPathTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnDropFiles(hDropInfo);

	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);

	TCHAR szFilePath[MAX_STR] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
		CDirectoryMgr::ExtractPathInfo(szFilePath, m_PathInfoList);
	}

	wstring wstrCombined = L"";
	TCHAR szFileCount[MIN_STR] = L"";

	for (auto& pPathInfo : m_PathInfoList)
	{
		// 정수 -> 문자 변환.
		_itow_s(pPathInfo->iFileCount, szFileCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szFileCount + L"|" + pPathInfo->wstrRelativePath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SetHorizontalScrollSizes();
}


void CPathTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L".txt", L"제목 없음.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files|*.txt|Data Files|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		// C++의 파일 출력 오브젝트
		wofstream fout;

		// C++의 파일 개방
		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		wstring wstrCombined = L"";
		TCHAR szFileCount[MIN_STR] = L"";

		for (auto& pPathInfo : m_PathInfoList)
		{
			// 정수 -> 문자 변환.
			_itow_s(pPathInfo->iFileCount, szFileCount, 10);

			wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
				L"|" + szFileCount + L"|" + pPathInfo->wstrRelativePath;

			// C++의 텍스트모드 출력
			fout << wstrCombined << endl;

			// C++의 바이너리모드 출력
			//fout.write()
		}

		fout.close();
	}

	// 윈도우즈의 기본 응용 프로그램 실행하는 시스템 함수.
	WinExec("notepad.exe ../Data/PathInfo.txt", SW_SHOW);
}


void CPathTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L".txt", L"제목 없음.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files|*.txt|Data Files|*.dat||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	::GetCurrentDirectory(MAX_STR, szCurrentDir);
	::PathRemoveFileSpec(szCurrentDir);
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (IDOK == Dlg.DoModal())
	{
		if (!m_PathInfoList.empty())
		{
			for_each(m_PathInfoList.begin(), m_PathInfoList.end(), SafeDelete<PATH_INFO*>);
			m_PathInfoList.clear();

			m_ListBox.ResetContent();
		}

		// C++ 파일 입력
		wifstream fin;

		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		TCHAR szObjectKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szCount[MAX_STR] = L"";
		TCHAR szPath[MAX_STR] = L"";

		PATH_INFO* pPathInfo = nullptr;

		while (true)
		{
			fin.getline(szObjectKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_STR);

			// EOF를 만나면 break!
			if (fin.eof())
				break;

			pPathInfo = new PATH_INFO;
			pPathInfo->wstrObjectKey = szObjectKey;
			pPathInfo->wstrStateKey = szStateKey;
			pPathInfo->iFileCount = _ttoi(szCount);
			pPathInfo->wstrRelativePath = szPath;

			m_PathInfoList.push_back(pPathInfo);
		}

		fin.close();
	}	

	wstring wstrCombined = L"";
	TCHAR szFileCount[MIN_STR] = L"";

	for (auto& pPathInfo : m_PathInfoList)
	{
		// 정수 -> 문자 변환.
		_itow_s(pPathInfo->iFileCount, szFileCount, 10);

		wstrCombined = pPathInfo->wstrObjectKey + L"|" + pPathInfo->wstrStateKey +
			L"|" + szFileCount + L"|" + pPathInfo->wstrRelativePath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	SetHorizontalScrollSizes();
}
