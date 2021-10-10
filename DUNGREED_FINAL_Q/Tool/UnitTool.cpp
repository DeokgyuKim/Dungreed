// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
	, m_strName(_T(""))
	, m_iAtt(0)
	, m_iDef(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
	for (auto& MyPair : m_mapUnit)
		SafeDelete(MyPair.second);

	m_mapUnit.clear();
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInput);
	DDX_Text(pDX, IDC_EDIT2, m_strOutput);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAtt);
	DDX_Text(pDX, IDC_EDIT5, m_iDef);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_RadioBtn[0]);
	DDX_Control(pDX, IDC_RADIO2, m_RadioBtn[1]);
	DDX_Control(pDX, IDC_RADIO3, m_RadioBtn[2]);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox[0]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBox[1]);
	DDX_Control(pDX, IDC_CHECK3, m_CheckBox[2]);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}

BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_RadioBtn[0].SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedPush)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnLbnSelectUnit)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedLoad)
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnEnChangeFindName)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnBnClickedDelete)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnBnClickedPush()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// UpdateData함수를 호출하면 DoDataExchange함수가 실행된다.
	// TRUE: 컨트롤 -> 변수로 데이터 이동
	// FALSE: 변수 -> 컨트롤로 데이터 이동
	UpdateData(TRUE);

	m_strOutput = m_strInput;

	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	auto iter_find = m_mapUnit.find(m_strName.GetString());

	if (m_mapUnit.end() != iter_find)
		return;

	UNIT_INFO* pUnit = new UNIT_INFO;
	pUnit->wstrName = m_strName.GetString();
	pUnit->iAtt = m_iAtt;
	pUnit->iDef = m_iDef;
	pUnit->iItem = 0;

	for (int i = 0; i < 3; ++i)
	{
		// 현재 버튼이 체크되어 있으면 TRUE 반환.
		if (m_RadioBtn[i].GetCheck())
		{
			pUnit->iJobIndex = i;
			break;
		}
	}

	if (m_CheckBox[0].GetCheck())
		pUnit->iItem |= RUBY;
	if (m_CheckBox[1].GetCheck())
		pUnit->iItem |= SAPPHIRE;
	if (m_CheckBox[2].GetCheck())
		pUnit->iItem |= DIAMOND;

	m_mapUnit.insert(make_pair(pUnit->wstrName, pUnit));

	// 리스트박스 목록에 데이터 추가.
	m_ListBox.AddString(m_strName);
}


void CUnitTool::OnLbnSelectUnit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// GetCurSel: 선택된 대상의 인덱스를 반환.
	int iIndex = m_ListBox.GetCurSel();

	// 잘못된 대상을 선택한다면 -1를 리턴.
	if (-1 == iIndex)
	{
		::AfxMessageBox(L"잘못된 대상입니다.");
		return;
	}

	CString strFindName = L"";

	// 인덱스의 해당하는 이름을 얻어옴.
	m_ListBox.GetText(iIndex, strFindName);

	auto iter_find = m_mapUnit.find(strFindName.GetString());

	if (m_mapUnit.end() == iter_find)
		return;

	m_strName = iter_find->second->wstrName.c_str();
	m_iAtt = iter_find->second->iAtt;
	m_iDef = iter_find->second->iDef;

	for (int i = 0; i < 3; ++i)
	{
		m_RadioBtn[i].SetCheck(FALSE); // 모든 버튼 체크 해제
		m_CheckBox[i].SetCheck(FALSE); // 모든 버튼 체크 해제
	}

	int iJobIndex = iter_find->second->iJobIndex;
	m_RadioBtn[iJobIndex].SetCheck(TRUE);

	int iItem = iter_find->second->iItem;

	if (iItem & RUBY)
		m_CheckBox[0].SetCheck(TRUE);
	if (iItem & SAPPHIRE)
		m_CheckBox[1].SetCheck(TRUE);
	if (iItem & DIAMOND)
		m_CheckBox[2].SetCheck(TRUE);

	UpdateData(FALSE);
}

void CUnitTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 파일 열기 혹은 파일 저장 관련 대화상자를 생성해주는 MFC 클래스.
	CFileDialog Dlg(FALSE, L".dat", L"제목 없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files|*.dat|Text Files|*.txt||", this);

	TCHAR szCurrentDir[MAX_STR] = L"";

	// 현재 작업경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentDir);

	// 현재 경로에서 파일명을 제거하는 함수. 파일명이 없다면 말단 폴더명을 제거.
	::PathRemoveFileSpec(szCurrentDir);
	//::AfxMessageBox(szCurrentDir);

	// PathCombine(A, B, C)
	// B와 C를 결합한 경로를 A에 저장. 이 때 B와 C 사이에 \\ 삽입.
	::PathCombine(szCurrentDir, szCurrentDir, L"Data");

	// 대화상자 초기경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentDir;

	// 창을 생성하는 2가지 방식
	// Modal: 제어권이 현재 창에만 있음. DoMadal함수
	// Modeless: 현재 창 뿐만아니라 다른 창에게도 제어권이 있음. Create함수
	if (IDOK == Dlg.DoModal())
	{
		// 대화상자에 입력한 파일이름(경로까지 포함된)을 얻어오는 함수. 
		//Dlg.GetPathName()
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwBytes = 0;

		for (auto& MyPair : m_mapUnit)
		{
			for (size_t i = 0; i < MyPair.second->wstrName.length() + 1; ++i)
				WriteFile(hFile, &MyPair.second->wstrName[i], sizeof(wchar_t), &dwBytes, nullptr);

			WriteFile(hFile, &MyPair.second->iAtt, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &MyPair.second->iDef, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &MyPair.second->iJobIndex, sizeof(int), &dwBytes, nullptr);
			WriteFile(hFile, &MyPair.second->iItem, sizeof(int), &dwBytes, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnBnClickedLoad()
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
		HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, nullptr,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		if (!m_mapUnit.empty())
		{
			for (auto& MyPair : m_mapUnit)
				SafeDelete(MyPair.second);

			m_mapUnit.clear();
			m_ListBox.ResetContent(); // 리스트박스 목록 비우기.
		}

		DWORD dwBytes = 0;
		wstring wstrName = L"";
		wchar_t ch = 0;
		int iAtt = 0, iDef = 0, iJobIndex = 0, iItem = 0;
		UNIT_INFO* pUnit = nullptr;

		while(true)
		{
			wstrName = L"";

			while (true)
			{
				ReadFile(hFile, &ch, sizeof(wchar_t), &dwBytes, nullptr);						

				if (0 == ch)
					break;

				wstrName += ch;
			}

			ReadFile(hFile, &iAtt, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iDef, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iJobIndex, sizeof(int), &dwBytes, nullptr);
			ReadFile(hFile, &iItem, sizeof(int), &dwBytes, nullptr);

			if (0 == dwBytes)
				break;

			pUnit = new UNIT_INFO;
			pUnit->wstrName = wstrName;
			pUnit->iAtt = iAtt;
			pUnit->iDef = iDef;
			pUnit->iJobIndex = iJobIndex;
			pUnit->iItem = iItem;

			m_mapUnit.insert(make_pair(pUnit->wstrName, pUnit));
			m_ListBox.AddString(pUnit->wstrName.c_str());
		}

		CloseHandle(hFile);
	}
}


void CUnitTool::OnEnChangeFindName()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	auto iter_find = m_mapUnit.find(m_strFindName.GetString());

	if (m_mapUnit.end() == iter_find)
		return;

	// 찾아낸 인덱스를 반환.
	int iIndex = m_ListBox.FindString(-1, m_strFindName);

	if (-1 == iIndex)
		return;

	// 인덱스를 선택하게 만듦.
	m_ListBox.SetCurSel(iIndex);
	CUnitTool::OnLbnSelectUnit();
}


void CUnitTool::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_ListBox.GetCurSel();

	if (-1 == iIndex)
		return;

	CString strFindName = L"";
	m_ListBox.GetText(iIndex, strFindName);

	auto iter_find = m_mapUnit.find(strFindName.GetString());

	if (m_mapUnit.end() == iter_find)
		return;

	SafeDelete(iter_find->second);
	m_mapUnit.erase(iter_find);

	// 해당 인덱스의 내용을 삭제함.
	m_ListBox.DeleteString(iIndex);	

	// 삭제 후 초기화.
	m_strName = L"";
	m_iAtt = 0;
	m_iDef = 0;

	for (int i = 0; i < 3; ++i)
	{
		m_RadioBtn[i].SetCheck(FALSE); // 모든 버튼 체크 해제
		m_CheckBox[i].SetCheck(FALSE); // 모든 버튼 체크 해제
	}

	UpdateData(FALSE);
}
