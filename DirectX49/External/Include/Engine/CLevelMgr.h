#pragma once

#include "singleton.h"

class CLevel;

class CLevelMgr :
	public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);

private:
	CLevel* m_curLevel;

public:
	void Init();
	void Tick();
	void Render();

public:
	CLevel* GetCurrentLevel() { return m_curLevel; }
};

