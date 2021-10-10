#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:	// Virtual Functions
	virtual BOOL OnInitDialog();

public:	// Message Functions
	afx_msg void OnBnClickedPush();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnLbnSelectUnit();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnEnChangeFindName();
	afx_msg void OnBnClickedDelete();

public:	// Value variables
	CString m_strInput; // 첫번째 Edit Control에서 문자열을 입력받음.	
	CString m_strOutput; // 두번째 Edit Control에 문자열을 출력함.
						 
	CString m_strName; // 캐릭터 이름	
	int m_iAtt; // 캐릭터 공격력	
	int m_iDef; // 캐릭터 방어력
				
	CString m_strFindName; // 이름 검색

public:	// Control variables
	CListBox m_ListBox;
	CButton m_RadioBtn[3];
	CButton m_CheckBox[3];

public: // User variables
	map<wstring, UNIT_INFO*>	m_mapUnit;			
	
};
