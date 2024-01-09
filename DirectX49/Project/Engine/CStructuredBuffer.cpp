#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_elementSize(0)
	, m_elementCount(0)
	, m_type(SB_TYPE::READ_ONLY)
	, m_bSysMemMove(false)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}



int CStructuredBuffer::Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type, bool _bSysMemMove, void* _pSysMem)
{
	// 구조화버퍼 크기가 16의 배수인지 체크
	assert(!(_elementSize % 16));

	m_SB = nullptr;
	m_SRV = nullptr;
	m_SB_Read = nullptr;
	m_SB_Write = nullptr;


	m_elementSize = _elementSize;
	m_elementCount = _elementCount;
	m_type = _type;
	m_bSysMemMove = _bSysMemMove;

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

	if (m_bSysMemMove)
	{
		// 쓰기용 버퍼
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;
		hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Write.GetAddressOf());

		// 읽기용 버퍼
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Read.GetAddressOf());
	}

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

void CStructuredBuffer::SetData(void* _sysMem, UINT _elementCount)
{
	assert(m_bSysMemMove);

	if (0 == _elementCount)
		_elementCount = m_elementCount;

	// 입력 데이터가 구조화버퍼보다 더 큰 경우
	if (m_elementCount < _elementCount)
	{
		Create(m_elementSize, _elementCount, m_type, m_bSysMemMove, nullptr);
	}

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _sysMem, m_elementSize * _elementCount);
	CONTEXT->Unmap(m_SB_Write.Get(), 0);

	// Write Buffer -> Main Buffer
	CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _dest, UINT _elementCount)
{
	assert(m_bSysMemMove);

	if (0 == _elementCount)
		_elementCount = m_elementCount;

	// Min Buffer -> Read Buffer
	CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
	memcpy(_dest, tSub.pData, m_elementSize * _elementCount);
	CONTEXT->Unmap(m_SB_Read.Get(), 0);
}
