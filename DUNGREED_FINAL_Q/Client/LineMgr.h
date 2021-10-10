#pragma once
class CLineMgr
{
private:
	CLineMgr();
	~CLineMgr();
public:
	static void DrawBox(RECT& rc);
	static void DrawBoxNoScroll(RECT& rc);
};

