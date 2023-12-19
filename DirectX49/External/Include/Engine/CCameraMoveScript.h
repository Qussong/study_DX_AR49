#pragma once
#include "CScript.h"

class CCameraMoveScript :
	public CScript
{
public:
	CCameraMoveScript();
	~CCameraMoveScript();
	
public:
	virtual void Tick();
};

