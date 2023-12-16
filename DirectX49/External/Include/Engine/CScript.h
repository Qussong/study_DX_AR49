#pragma once

#include "CComponent.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
//#include "CResMgr.h"

#include "CGameObject.h"
#include "components.h"

class CScript :
	public CComponent
{
public:
	CScript();
	~CScript();

private:
	UINT        m_iScriptType;

public:
	virtual void FinalTick() final {}
};

