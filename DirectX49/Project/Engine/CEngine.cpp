#include "pch.h"
#include "CEngine.h"

// DX
#include "CDevice.h"
// Mgr
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CGC.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"

CEngine::CEngine()
    : m_hMainWnd(nullptr)
    , m_vResolution()
{
}

CEngine::~CEngine()
{
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hMainWnd = _hWnd;
    m_vResolution = _vResolution;

    RECT rt = { 0,0,(int)m_vResolution.x, (int)m_vResolution.y };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

    if (FAILED(CDevice::GetInst()->Init(m_hMainWnd, m_vResolution)))
    {
        MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Manager init
    CPathMgr::Init();
    CTimeMgr::GetInst()->init();
    CKeyMgr::GetInst()->init();
    CAssetMgr::GetInst()->Init();
    CRenderMgr::GetInst()->Init();
    CLevelMgr::GetInst()->Init();

    return S_OK;
}

void CEngine::progress()
{
    // Manager Update
    CTimeMgr::GetInst()->tick();
    CKeyMgr::GetInst()->tick();

    // Level Update
    CLevelMgr::GetInst()->Tick();
    CCollisionMgr::GetInst()->Tick();
    CRenderMgr::GetInst()->Tick();

    // GC
    CGC::GetInst()->Tick();

    // Task
    CTaskMgr::GetInst()->Tick();

    DebugFunctionCheck();
}

void CEngine::DebugFunctionCheck()
{
    if (KEY_TAP(KEY::O))
    {
        CRenderMgr::GetInst()->IsDebugPosition() 
            ? CRenderMgr::GetInst()->SetDebugPosition(false) 
            : CRenderMgr::GetInst()->SetDebugPosition(true);
    }
}