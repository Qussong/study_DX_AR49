#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _type)
	: m_desc{}
	, m_Type(_type)
{
}

CConstBuffer::~CConstBuffer()
{
}

int CConstBuffer::Create(UINT _size, UINT _cnt)
{
	m_elementSize = _size;
	m_elementCnt = _cnt;

	ZeroMemory(&m_desc, sizeof(m_desc));
	{
		m_desc.ByteWidth = _size * _cnt;				// 상수버퍼의 크기
		m_desc.StructureByteStride = _size;				// 상수버퍼의 인자 하나의 크기
		m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 용도설정 = 상수버퍼 
		m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// 버퍼에 쓰기 가능
		m_desc.Usage = D3D11_USAGE_DYNAMIC;
	}

	HRESULT hr = DEVICE->CreateBuffer(&m_desc, nullptr, m_CB.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"상수 버퍼 생성 실패", L"Create ConstantBuffer Error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CConstBuffer::SetData(void* _src, UINT _cnt)
{
	if (0 == _cnt)
	{
		_cnt = m_elementCnt;
	}

	// SystemMem -> GPUMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _src, m_elementSize * _cnt);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstBuffer::UpdateData()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}