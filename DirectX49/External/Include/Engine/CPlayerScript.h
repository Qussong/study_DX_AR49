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

public:
	virtual void Tick() override;
};

