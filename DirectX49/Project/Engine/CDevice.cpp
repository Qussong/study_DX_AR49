#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

CDevice::CDevice()
	: m_hRenderWnd(nullptr)
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
	, m_arrSampler{}
{
}

CDevice::~CDevice()
{
	Delete_Array(m_arrCB);
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// 출력 윈도우 핸들
	m_hRenderWnd = _hWnd;

	// 출력 윈도우 해상도, 버퍼 해상도
	m_vRenderResolution = _vResolution;

	// Device, DeviceContext 생성 & 초기화
	if (FAILED(CreateDevice()))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// SwapChain 생성 & 초기화
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// RenderTarget, RenderTargetView, DepthStencil, DepthStencilView 생성 & 초기화
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"타겟 및 View 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// Viewport 설정
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	{
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.f;
		viewport.TopLeftX = 0.f;
		viewport.TopLeftY = 0.f;
		viewport.Width = m_vRenderResolution.x;
		viewport.Height = m_vRenderResolution.y;
	}
	CONTEXT->RSSetViewports(1, &viewport);

	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"Rasterizer State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencil State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}


	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"Blend State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSamplerState()))
	{
		MessageBox(nullptr, L"Sampler State 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearRenderTarget(float(&_color)[4])
{
	CONTEXT->ClearRenderTargetView(m_RTView.Get(), _color);
	CONTEXT->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, (UINT8)0.f);
}

void CDevice::Present()
{
	m_SwapChain->Present(0, 0);
}

int CDevice::CreateDevice()
{
	// 장치 초기화
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	// Device 생성
	HRESULT hr = D3D11CreateDevice(
		nullptr
		, D3D_DRIVER_TYPE_HARDWARE
		, nullptr
		, D3D11_CREATE_DEVICE_DEBUG
		, nullptr, 0
		, D3D11_SDK_VERSION
		, m_Device.GetAddressOf()
		, &eLevel
		, m_Context.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CDevice::CreateSwapChain()
{
	// 생성할 SwapChain 생성 구조체
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferCount = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
		desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Flags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = true; // 창모드
		desc.OutputWindow = m_hRenderWnd; // SwapChain 의 출력 윈도우 지정
	}

	// SwapChain 생성을 위해 Factory에 접근
	ComPtr<IDXGIDevice>		pIdxgiDevice	= nullptr;
	ComPtr<IDXGIAdapter>	pAdapter		= nullptr;
	ComPtr<IDXGIFactory>	pFactory		= nullptr;
	{
		DEVICE->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
		pIdxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
		pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());
	}

	// SwapChain
	HRESULT hr = pFactory->CreateSwapChain(m_Device.Get(), &desc, m_SwapChain.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CDevice::CreateTargetView()
{
	HRESULT hr;

	// Render Target Texture
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());
	CHECK(hr);

	// Render Target Texture View
	hr = DEVICE->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());
	CHECK(hr);

	// DepthStencil Texture
	D3D11_TEXTURE2D_DESC desc = {};
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.Width = (UINT)m_vRenderResolution.x;
		desc.Height = (UINT)m_vRenderResolution.y;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.ArraySize = 1;
	}
	hr = DEVICE->CreateTexture2D(&desc, nullptr, m_DSTex.GetAddressOf());
	CHECK(hr);

	// DepthStencil Texture View
	hr = DEVICE->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());
	CHECK(hr);

	CONTEXT->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC tDesc = {};

	// Cull Back
	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// Cull Front
	{
		tDesc.CullMode = D3D11_CULL_FRONT;
		tDesc.FillMode = D3D11_FILL_SOLID;
	}
	hr = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
	CHECK(hr);

	// Cull None
	{
		tDesc.CullMode = D3D11_CULL_NONE;
		tDesc.FillMode = D3D11_FILL_SOLID;
	}
	hr = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());
	CHECK(hr);

	// Cull Frame
	{
		tDesc.CullMode = D3D11_CULL_NONE;
		tDesc.FillMode = D3D11_FILL_WIREFRAME;
	}
	hr = DEVICE->CreateRasterizerState(&tDesc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;

	// Less Equal
	{
		tDesc.DepthEnable = true;
		tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		tDesc.StencilEnable = false;
	}
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());
	CHECK(hr);

	// Greater
	{
		tDesc.DepthEnable = true;
		tDesc.DepthFunc = D3D11_COMPARISON_GREATER;
		tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		tDesc.StencilEnable = false;
	}
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER].GetAddressOf());
	CHECK(hr);

	// Greater Equal
	{
		tDesc.DepthEnable = true;
		tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		tDesc.StencilEnable = false;
	}
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::GRATER_EQUAL].GetAddressOf());
	CHECK(hr);

	// No Test
	{
		tDesc.DepthEnable = false;
		tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
		tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		tDesc.StencilEnable = false;
	}
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST].GetAddressOf());
	CHECK(hr);

	// No Write
	{
		tDesc.DepthEnable = true;
		tDesc.DepthFunc = D3D11_COMPARISON_LESS;
		tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		tDesc.StencilEnable = false;
	}
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());
	CHECK(hr);

	// NoTest NoWrite
	{
		tDesc.DepthEnable = false;
		tDesc.DepthFunc = D3D11_COMPARISON_NEVER;
		tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		tDesc.StencilEnable = false;
	}
	hr = DEVICE->CreateDepthStencilState(&tDesc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC tDesc = {};

	// AlphaBlend
	{
		tDesc.AlphaToCoverageEnable = false;
		tDesc.IndependentBlendEnable = false;	// false : 0번의 설정이 나머지 7개에도 적용됨
		// RenderTarget 8개까지 설정 가능
		tDesc.RenderTarget[0].BlendEnable = true;
		tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());
		CHECK(hr);
	}

	// ONE_ONE
	{
		tDesc.AlphaToCoverageEnable = false;
		tDesc.IndependentBlendEnable = false;
		tDesc.RenderTarget[0].BlendEnable = true;
		tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());
		CHECK(hr);
	}

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	HRESULT hr = S_OK;

	// 
	D3D11_SAMPLER_DESC tDesc = {};
	{
		tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		tDesc.Filter = D3D11_FILTER_ANISOTROPIC;

		tDesc.MinLOD = 0;
		tDesc.MaxLOD = 1;
	}
	hr = DEVICE->CreateSamplerState(&tDesc, m_arrSampler[0].GetAddressOf());
	CHECK(hr);

	CONTEXT->VSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_arrSampler[0].GetAddressOf());

	// 
	{
		tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		tDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

		tDesc.MinLOD = 0;
		tDesc.MaxLOD = 1;
	}
	hr = DEVICE->CreateSamplerState(&tDesc, m_arrSampler[1].GetAddressOf());
	CHECK(hr);

	CONTEXT->VSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_arrSampler[1].GetAddressOf());

	return hr;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	return S_OK;
}
