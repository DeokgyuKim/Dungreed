#include "stdafx.h"
#include "DirectoryMgr.h"


CDirectoryMgr::CDirectoryMgr()
{
}


CDirectoryMgr::~CDirectoryMgr()
{
}

void CDirectoryMgr::ConvertRelativePath(TCHAR* pOut, const TCHAR* pTo)
{
	// pFrom에서 pTo로 찾아가는 상대경로를 만들어서 pOut에 저장해주는 함수.
	// 단, pFrom과 pTo는 같은 드라이브에 존재해야함.
	//::PathRelativePathTo(pOut, pFrom, FILE_ATTRIBUTE_DIRECTORY,
	//	pTo, FILE_ATTRIBUTE_DIRECTORY);

	TCHAR szCurrentDir[MAX_STR] = L"";
	::GetCurrentDirectory(MAX_STR, szCurrentDir);

	::PathRelativePathTo(pOut, szCurrentDir, FILE_ATTRIBUTE_DIRECTORY,
		pTo, FILE_ATTRIBUTE_DIRECTORY);
}

void CDirectoryMgr::ExtractPathInfo(CString strFilePath, list<PATH_INFO*>& rPathInfoList)
{
	strFilePath += L"\\*.*";

	// 경로 탐색 관련 MFC 클래스.
	CFileFind	find;

	// 경로 유무를 조사하는 함수. 해당 경로가 존재하면 TRUE 반환.
	// 경로가 존재했고 해당 경로에 접근하기 위해서는 반드시 FindNextFile을 호출해주어야한다.
	// 만약, 조사하는 경로에 "*.*"이 포함된 경우에 FindNextFile을 호출하면 가장 첫번째 대상에게 접근할 수 있다.
	BOOL bIsFind = find.FindFile(strFilePath);

	while (bIsFind)
	{
		// 가장 마지막 파일에 도달한 경우 FALSE를 반환.
		bIsFind = find.FindNextFile();

		if(find.IsDots()) // 찾아낸 대상이 .과 ..같은 마커일 경우 건너뜀.
			continue;
		else if (find.IsDirectory()) // 찾아낸 대상이 폴더일 경우 TRUE
			ExtractPathInfo(find.GetFilePath(), rPathInfoList);
		else // 찾아낸 대상이 파일일 때
		{
			if(find.IsSystem()) // 찾아낸 대상이 시스템 관련 파일이라면 건너뜀.
				continue;

			// 상대경로
			CString str = find.GetFilePath();
			str.Replace(L"0.", L"%d."); // Replace(Old, New)

			TCHAR szRelative[MAX_STR] = L"";
			ConvertRelativePath(szRelative, str);

			PATH_INFO* pPathInfo = new PATH_INFO;
			pPathInfo->wstrRelativePath = szRelative;

			// 이미지 파일 개수
			::PathRemoveFileSpec(szRelative);
			pPathInfo->iFileCount = CountFile(szRelative);

			// StateKey
			pPathInfo->wstrStateKey = ::PathFindFileName(szRelative);

			// ObjectKey
			::PathRemoveFileSpec(szRelative);
			pPathInfo->wstrObjectKey = ::PathFindFileName(szRelative);

			rPathInfoList.push_back(pPathInfo);
			break;
		}
	}
}

int CDirectoryMgr::CountFile(CString strFilePath)
{
	strFilePath += L"\\*.*";

	CFileFind find;
	BOOL bIsFind = find.FindFile(strFilePath);

	int iCount = 0;

	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots() || find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}
