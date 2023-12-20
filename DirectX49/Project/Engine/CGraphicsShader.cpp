#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// Vertex Shader 생성
	{
		wstring strContentPath = CPathMgr::GetContentPath();
		wstring strFilePath = strContentPath + _strRelativePath;

		// HLSL 버텍스 쉐이더 함수 컴파일
		HRESULT hr = D3DCompileFromFile(
			strFilePath.c_str()
			, nullptr
			, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, _strFuncName.c_str(), "vs_5_0"
			, D3DCOMPILE_DEBUG
			, 0
			, m_VSBlob.GetAddressOf()
			, m_ErrBlob.GetAddressOf());

		if (FAILED(hr))
		{
			if (nullptr != m_ErrBlob)
			{
				char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
				MessageBoxA(nullptr, pErrMsg, "Vertex Shader Compile Failed!!", MB_OK);
			}
			return E_FAIL;
		}

		hr = DEVICE->CreateVertexShader(
			m_VSBlob->GetBufferPointer()
			, m_VSBlob->GetBufferSize()
			, nullptr
			, m_VS.GetAddressOf());
		CHECK(hr);
	}

	// Layout(정점 구조정보) 생성
	{
		D3D11_INPUT_ELEMENT_DESC arrElement[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA},	//  0 ~ 11 (12Byte)
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA},	// 12 ~ 27 (16Byte)
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA},		// 28 ~ 35 ( 8Byte)
		};

		HRESULT hr = DEVICE->CreateInputLayout(
			arrElement
			, 3
			, m_VSBlob->GetBufferPointer()
			, m_VSBlob->GetBufferSize()
			, m_Layout.GetAddressOf());
		CHECK(hr);
	}

	return S_OK;
}

int CGraphicsShader::CreateHullShader()
{
	return 0;
}

int CGraphicsShader::CreateDomainShader()
{
	return 0;
}

int CGraphicsShader::CreateGeometryShader()
{
	return 0;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// Pixel Shader 생성
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// HLSL 픽셀 쉐이더 함수 컴파일
	HRESULT hr = D3DCompileFromFile(
		strFilePath.c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0"
		, D3DCOMPILE_DEBUG
		, 0
		, m_PSBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Pixel Shader Compile Failed!!", MB_OK);
		}
		return E_FAIL;
	}

	hr = DEVICE->CreatePixelShader(
		m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize()
		, nullptr
		, m_PS.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

void CGraphicsShader::UpdateData()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);

	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());

	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);
}