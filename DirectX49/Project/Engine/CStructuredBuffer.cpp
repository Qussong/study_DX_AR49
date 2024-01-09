#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_elementSize(0)
	, m_elementCount(0)
	, m_type(SB_TYPE::READ_ONLY)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type, void* _pSysMem)
{
	// 구조화버퍼 크기가 16의 배수인지 체크
	assert(!(_elementSize % 16));

	m_elementSize = _elementSize;
	m_elementCount = _elementCount;

	D3D11_BUFFER_DESC tDesc = {};
	tDesc.ByteWidth = m_elementSize * m_elementCount;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tDesc.StructureByteStride = m_elementSize;

	tDesc.CPUAccessFlags = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = E_FAIL;
	if (nullptr == _pSysMem)
	{
		hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB.GetAddressOf());
	}
	else
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pSysMem;
		hr = DEVICE->CreateBuffer(&tDesc, &tSub, m_SB.GetAddressOf());
	}

	if (FAILED(hr)) return E_FAIL;

	// Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = m_elementCount;

	hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

void CStructuredBuffer::UpdateData(UINT _registerNum)
{
	CONTEXT->VSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
}
