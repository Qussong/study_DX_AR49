#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh()
    : CAsset(ASSET_TYPE::MESH)
    , mVBDesc{}
    , mIBDesc{}
    , mVtxCount(0)
    , mIdxCount(0)
    , mVtxSysMem(nullptr)
    , mIdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
    if (nullptr != mVtxSysMem)
        delete mVtxSysMem;

    if (nullptr != mIdxSysMem)
        delete mIdxSysMem;
}

int CMesh::Create(void* _vtx, UINT _vtxCnt, void* _idx, UINT _idxCnt)
{
    mVtxCount = _vtxCnt;
    mIdxCount = _idxCnt;

    // Vertex Buffer 생성
    {
        mVBDesc = {};
        ZeroMemory(&mVBDesc, sizeof(mVBDesc));
        {
            mVBDesc.ByteWidth = sizeof(Vtx) * _vtxCnt;		// 버텍스버퍼의 크기
            mVBDesc.StructureByteStride = sizeof(Vtx);		// 버텍스버퍼의 인자 하나의 크기

            mVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 용도설정 = 버텍스버퍼

            mVBDesc.CPUAccessFlags = 0;						// 버퍼에 데이터 쓰기 불가능
            mVBDesc.Usage = D3D11_USAGE_DEFAULT;
        }

        // 버텍스버퍼에 들어갈 정점들의 초기 값 설정
        D3D11_SUBRESOURCE_DATA tSubData = {};
        tSubData.pSysMem = _vtx;

        // 버텍스버퍼 생성
        HRESULT hr = DEVICE->CreateBuffer(&mVBDesc, &tSubData, mVB.GetAddressOf());
        if (FAILED(hr))
        {
            MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"오류", MB_OK);
            return E_FAIL;
        }
    }

    // Index Buffer 생성
    {
        mIBDesc = {};
        ZeroMemory(&mIBDesc, sizeof(mIBDesc));
        {
            mIBDesc.ByteWidth = sizeof(UINT) * _idxCnt;		// 인덱스버퍼의 크기
            mIBDesc.StructureByteStride = sizeof(UINT);	// 인덱스버퍼의 인자 하나의 크기
            mIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 용도설정 = 인덱스버퍼
            mIBDesc.CPUAccessFlags = 0;					// 버퍼에 데이터 쓰기 불가능
            mIBDesc.Usage = D3D11_USAGE_DEFAULT;
        }

        // 인덱스버퍼에 들어갈 정점들의 초기 값 설정
        D3D11_SUBRESOURCE_DATA tSubData = {};
        tSubData.pSysMem = _idx;

        // 인덱스버퍼
        HRESULT hr = DEVICE->CreateBuffer(&mIBDesc, &tSubData, mIB.GetAddressOf());
        CHECK(hr);
    }

    // 원본 정점정보 및 인덱스 정보를 동적할당한 곳에다가 저장시켜두고 관리
    {
        mVtxSysMem = new Vtx[mVtxCount];
        mIdxSysMem = new Vtx[mIdxCount];
        memcpy(mVtxSysMem, _vtx, sizeof(Vtx) * mVtxCount);
        memcpy(mIdxSysMem, _idx, sizeof(UINT) * mIdxCount);
    }

    return 0;
}

void CMesh::UpdateData()
{
    UINT iStride = sizeof(Vtx);
    UINT iOffset = 0;

    CONTEXT->IASetVertexBuffers(0, 1, mVB.GetAddressOf(), &iStride, &iOffset);
    CONTEXT->IASetIndexBuffer(mIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::render()
{
    UpdateData();

    CONTEXT->DrawIndexed(mIdxCount, 0, 0);
}