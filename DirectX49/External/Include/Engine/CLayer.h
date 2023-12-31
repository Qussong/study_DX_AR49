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
    vector<CGameObject*>    m_vecObjects;

    int                     m_iLayerIdx;

private:
    void Begin();
    void Tick();
    void FinalTick();
    //void Render();

public:
    void DetachGameObject(CGameObject* _object);    // 특정 오브젝트를 레이어에서 제거
    void RegisterGameObject(CGameObject* _object) { m_vecObjects.push_back(_object); }
    int GetLayerIdx() { return m_iLayerIdx; }

    const vector<CGameObject*>& GetParentObjects() { return  m_vecParent; }
    const vector<CGameObject*>& GetLayerObjects() { return m_vecObjects; }

private:
    void AddObject(CGameObject* _object, bool _bMove);

    friend class CLevel;
};

