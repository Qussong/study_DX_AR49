#pragma once
#include "CEntity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};


class CStructuredBuffer :
    public CEntity
{
public:
    CStructuredBuffer();
    ~CStructuredBuffer();

private:
    // 기본 구조화 버퍼
    ComPtr<ID3D11Buffer>                m_SB;
    // 구조화 버퍼의 정보를 넘겨주기 위한 Shader Resource View
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    // 읽기전용 구조화 버퍼
    ComPtr<ID3D11Buffer>                m_SB_Read;
    // 쓰기전용 구조화 버퍼
    ComPtr<ID3D11Buffer>                m_SB_Write;

    UINT                                m_elementSize;
    UINT                                m_elementCount;

    SB_TYPE                             m_type;
    bool                                m_bSysMemMove;

public:
    int Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type, bool _bSysMemMove = false, void* _pSysMem = nullptr);
    void UpdateData(UINT _registerNum);

    void SetData(void* _sysMem, UINT _elementCount = 0);
    void GetData(void* _dest, UINT _elementCount = 0);

    UINT GetElementSize() { return m_elementSize; }
    UINT GetElementCount() { return m_elementCount; }


};

