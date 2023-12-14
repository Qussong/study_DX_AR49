#include "pch.h"
#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER)
	, mTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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
			, mVSBlob.GetAddressOf()
			, mErrBlob.GetAddressOf());

		if (FAILED(hr))
		{
			if (nullptr != mErrBlob)
			{
				char* pErrMsg = (char*)mErrBlob->GetBufferPointer();
				MessageBoxA(nullptr, pErrMsg, "Vertex Shader Compile Failed!!", MB_OK);
			}
			return E_FAIL;
		}

		hr = DEVICE->CreateVertexShader(
			mVSBlob->GetBufferPointer()
			, mVSBlob->GetBufferSize()
			, nullptr
			, mVS.GetAddressOf());
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
			, mVSBlob->GetBufferPointer()
			, mVSBlob->GetBufferSize()
			, mLayout.GetAddressOf());
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

	// HLSL 버텍스 쉐이더 함수 컴파일
	HRESULT hr = D3DCompileFromFile(
		strFilePath.c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0"
		, D3DCOMPILE_DEBUG
		, 0
		, mPSBlob.GetAddressOf()
		, mErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != mErrBlob)
		{
			char* pErrMsg = (char*)mErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Pixel Shader Compile Failed!!", MB_OK);
		}
		return E_FAIL;
	}

	hr = DEVICE->CreatePixelShader(
		mPSBlob->GetBufferPointer()
		, mPSBlob->GetBufferSize()
		, nullptr
		, mPS.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

void CGraphicsShader::UpdateData()
{
	// IA
	CONTEXT->IASetInputLayout(mLayout.Get());
	CONTEXT->IASetPrimitiveTopology(mTopology);

	CONTEXT->VSSetShader(mVS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(mHS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(mDS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(mGS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(mPS.Get(), nullptr, 0);
}