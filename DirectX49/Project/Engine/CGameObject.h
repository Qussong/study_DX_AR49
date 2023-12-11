#pragma once
#include "CEntity.h"

class CComponent;

class CGameObject :
	public CEntity
{
private:
	CComponent*		mArrCom[(UINT)COMPONENT_TYPE::END];

public:
	CGameObject();
	~CGameObject();
};

