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
    ComPtr<ID3D11Buffer> m_SB;
    ComPtr<ID3D11ShaderResourceView> m_SRV;

    UINT    m_elementSize;
    UINT    m_elementCount;

    SB_TYPE m_type;

public:
    int Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type = SB_TYPE::READ_ONLY, void* _pSysMem = nullptr);
    void UpdateData(UINT _registerNum);


};

