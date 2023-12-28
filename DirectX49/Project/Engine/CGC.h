#pragma once
#include "singleton.h"

class CEntity;

class CGC :
    public CSingleton<CGC>
{
    SINGLE(CGC);
private:
    vector<CEntity*>    m_vecEntity;

public:
    void Tick();

    void Add(CEntity* _entity)
    {
        m_vecEntity.push_back(_entity);
    }
};

