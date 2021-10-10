#pragma once
class CDirectoryMgr
{
private:
	CDirectoryMgr();
	~CDirectoryMgr();

public:
	static void ConvertRelativePath(TCHAR* pOut, const TCHAR* pTo);
	static void ExtractPathInfo(CString strFilePath, list<PATH_INFO*>& rPathInfoList);
	static int CountFile(CString strFilePath);
};

