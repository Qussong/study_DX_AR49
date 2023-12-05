#include "pch.h"
#include "CEngine.h"

// DX
#include "CDevice.h"
// Mgr
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

// �ﰢ�� �׸���
#include "Test.h"

CEngine::CEngine()
    : m_hMainWnd(nullptr)
    , m_vResolution()
{
}

CEngine::~CEngine()
{
    // �ﰢ�� �׸���
    TestRelease();
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
    m_hMainWnd = _hWnd;
    m_vResolution = _vResolution;

    RECT rt = { 0,0,(int)m_vResolution.x, (int)m_vResolution.y };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
    SetWindowPos(m_hMainWnd, nullptr, 10.f, 10.f, rt.right - rt.left, rt.bottom - rt.top, 0);

    if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
    {
        MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"�ʱ�ȭ ����", MB_OK);
        return E_FAIL;
    }

    // Manager init
    CPathMgr::init();
    CTimeMgr::GetInst()->init();
    CKeyMgr::GetInst()->init();

    // �ﰢ�� �׸���
    if (FAILED(TestInit()))
    {
        MessageBox(nullptr, L"Test �ʱ�ȭ ����", L"Test ����", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

void CEngine::progress()
{
    // Manager Update
    CTimeMgr::GetInst()->tick();
    CKeyMgr::GetInst()->tick();

    // �ﰢ�� �׸���
    TestProgress();
}