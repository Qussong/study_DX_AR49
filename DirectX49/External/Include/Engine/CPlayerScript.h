#pragma once

#include "CScript.h"

class CPlayerScript :
	public CScript
{
public:
	CPlayerScript();
	~CPlayerScript();

private:
	float       m_speed;

	// test
	UINT		m_cameraType = (UINT)PROJ_TYPE::PERSPECTIVE;

public:
	virtual void Tick() override;
};

