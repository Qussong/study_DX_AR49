#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"

CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const wstring& _strFilePath)
{
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	HRESULT hr = S_OK;

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
	{
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	}
	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
	{
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}
	else
	{
		// png, bmp, jpg, jpeg
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture Load Failed", L"Texture Error", MB_OK);
		return E_FAIL;
	}

	// 1. ID3D11Texture2D 객체 생성
	// 2. ScratchImage 에 있는 데이터를 ID3D11Texture2D 객체 로 전달
	// 3. ID3D11Texture2D 를 이용한 ShaderResourceView 만들기
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	// 만들어진 ShaderResourceView 를 이용해서 생성된 ID3D11Texture2D 객체를 알아낸다.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// ID3D11Texture2D 객체를 통해서 생성될때 사용된 옵션(DESC) 정보를 가져온다.
	m_Tex2D->GetDesc(&m_Desc);

	/*
		D3D11_TEXTURE2D_DESC 의 정보
	 
	    UINT				Width;
		UINT				Height;
		UINT				MipLevels;
		UINT				ArraySize;
		DXGI_FORMAT			Format;
		DXGI_SAMPLE_DESC	SampleDesc;
		D3D11_USAGE			Usage;
		UINT				BindFlags;
		UINT				CPUAccessFlags;
		UINT				MiscFlags;
	*/

	return S_OK;
}

void CTexture::UpdateData(int _registerNum)
{
	CONTEXT->VSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Clear(int _registerNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_registerNum, 1, &pSRV);
}