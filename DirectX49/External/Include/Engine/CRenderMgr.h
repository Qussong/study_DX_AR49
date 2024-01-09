#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>        m_vecCam;

    CStructuredBuffer*      m_light2DBuffer;

    list<tDebugShapeInfo>   m_dbgShapeInfo;

    CGameObject*            m_pDebugObj;

    bool                    m_bDebugPos;

public:
    void RegisterCamera(CCamera* _cam, int _idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _info)
    {
        m_dbgShapeInfo.push_back(_info);
    }

    void SetDebugPosition(bool _flag) { m_bDebugPos = _flag; }
    bool IsDebugPosition() { return m_bDebugPos; }

public:
    void Init();
    void Tick();

private:
    void Render();
    void Render_debug();
};