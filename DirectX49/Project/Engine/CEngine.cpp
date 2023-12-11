#include "pch.h"
#include "CEngine.h"

// DX
#include "CDevice.h"
// Mgr
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

// 삼각형 그리기
#include "Test.h"

CEngine::CEngine()
    : m_hMainWnd(nullptr)
    , m_vResolution()
{
}

CEngine::~CEngine()
{
    // test
    TestRelease();
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hMainWnd = _hWnd;
    m_vResolution = _vResolution;

    RECT rt = { 0,0,(int)m_vResolution.x, (int)m_vResolution.y };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(m_hMainWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);

    if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
    {
        MessageBox(nullptr, L"Device 초기화 실패", L"초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Manager init
    CPathMgr::init();
    CTimeMgr::GetInst()->init();
    CKeyMgr::GetInst()->init();

    // test
    if (FAILED(TestInit()))
    {
        MessageBox(nullptr, L"Test 초기화 실패", L"Test 실패", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void CEngine::progress()
{
    // Manager Update
    CTimeMgr::GetInst()->tick();
    CKeyMgr::GetInst()->tick();

    // test
    TestProgress();
}