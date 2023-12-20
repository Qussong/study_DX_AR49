#pragma once
#include "CScript.h"

class CCameraMoveScript :
	public CScript
{
public:
	CCameraMoveScript();
	~CCameraMoveScript();

private:
	float	m_camSpeed;
	
private:
	void MoveOrthographic();
	void MovePerspective();
	
public:
	virtual void Tick();
};

