#pragma once

class CPathMgr
{
private:
	static wchar_t g_szContent[255];
	

public:
	static void Init();
	static const wchar_t* GetContentPath() { return g_szContent; }
};

