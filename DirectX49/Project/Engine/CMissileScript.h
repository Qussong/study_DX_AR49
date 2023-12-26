#pragma once
#include "CScript.h"

class CMissileScript :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void Tick() override;

public:
    CMissileScript();
    ~CMissileScript();
};

