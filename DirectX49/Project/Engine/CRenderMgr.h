#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>        m_vecCam;

    list<tDebugShapeInfo>   m_dbgShapeInfo;

    CGameObject*            m_pDebugObj;

public:
    void RegisterCamera(CCamera* _cam, int _idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info)
    {
        m_dbgShapeInfo.push_back(_info);
    }
public:
    void Init();
    void Tick();

private:
    void Render();
    void Render_debug();
};