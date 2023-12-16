#pragma once

#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
public:
    CLayer();
    ~CLayer();

private:
    vector<CGameObject*>    m_vecParent;

private:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();

private:
    void AddObject(CGameObject* _obj) { m_vecParent.push_back(_obj); }

    friend class CLevel;
};

