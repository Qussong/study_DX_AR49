#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh()
    : CAsset(ASSET_TYPE::MESH)
    , m_VBDesc{}
    , m_IBDesc{}
    , m_VtxCount(0)
    , m_IdxCount(0)
    , m_VtxSysMem(nullptr)
    , m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
    if (nullptr != m_VtxSysMem)
        delete m_VtxSysMem;

    if (nullptr != m_IdxSysMem)
        delete m_IdxSysMem;
}

int CMesh::Create(void* _vtx, UINT _vtxCnt, void* _idx, UINT _idxCnt)
{
    m_VtxCount = _vtxCnt;
    m_IdxCount = _idxCnt;

    // Vertex Buffer 생성
    {
        m_VBDesc = {};
        ZeroMemory(&m_VBDesc, sizeof(m_VBDesc));
        {
            m_VBDesc.ByteWidth = sizeof(Vtx) * _vtxCnt;		// 버텍스버퍼의 크기
            m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
            m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 용도설정 = 버텍스버퍼
            m_VBDesc.CPUAccessFlags = 0;					// 버퍼에 데이터 쓰기 불가능
            m_VBDesc.MiscFlags = 0;
            m_VBDesc.StructureByteStride = sizeof(Vtx);		// 버텍스버퍼의 인자 하나의 크기
        }

        // 버텍스버퍼에 들어갈 정점들의 초기 값 설정
        D3D11_SUBRESOURCE_DATA tSubData = {};
        tSubData.pSysMem = _vtx;

        // 버텍스버퍼 생성
        HRESULT hr = DEVICE->CreateBuffer(&m_VBDesc, &tSubData, m_VB.GetAddressOf());
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to create Vertex Buffer", L"Vertex Buffer", MB_OK);
            return E_FAIL;
        }
    }

    // Index Buffer 생성
    {
        m_IBDesc = {};
        ZeroMemory(&m_IBDesc, sizeof(m_IBDesc));
        {
            m_IBDesc.ByteWidth = sizeof(UINT) * _idxCnt;		// 인덱스버퍼의 크기
            m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
            m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	    // 용도설정 = 인덱스버퍼
            m_IBDesc.CPUAccessFlags = 0;					    // 버퍼에 데이터 쓰기 불가능
            m_IBDesc.MiscFlags = 0;
            m_IBDesc.StructureByteStride = sizeof(UINT);	    // 인덱스버퍼의 인자 하나의 크기
        }

        // 인덱스버퍼에 들어갈 정점들의 초기 값 설정
        D3D11_SUBRESOURCE_DATA tSubData = {};
        tSubData.pSysMem = _idx;

        // 인덱스버퍼
        HRESULT hr = DEVICE->CreateBuffer(&m_IBDesc, &tSubData, m_IB.GetAddressOf());
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"Failed to create Index Buffer", L"Index Buffer", MB_OK);
            return E_FAIL;
        }
    }

    // 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
    {
        m_VtxSysMem = new Vtx[m_VtxCount];
        m_IdxSysMem = new Vtx[m_IdxCount];
        memcpy(m_VtxSysMem, _vtx, sizeof(Vtx) * m_VtxCount);
        memcpy(m_IdxSysMem, _idx, sizeof(UINT) * m_IdxCount);
    }

    return 0;
}

void CMesh::UpdateData()
{
    UINT iStride = sizeof(Vtx);
    UINT iOffset = 0;

    CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
    CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
    UpdateData();

    CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}