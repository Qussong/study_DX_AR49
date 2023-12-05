#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	: m_hRenderWnd(nullptr)
	, m_Viewport{}
{
}

CDevice::~CDevice()
{
}

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	// ��� ������ �ڵ�
	m_hRenderWnd = _hWnd;

	// ��� ������ �ػ�, ���� �ػ�
	m_vRenderResolution = _vResolution;

	// Device, DeviceContext ���� & �ʱ�ȭ
	if (FAILED(CreateDevice()))
	{
		MessageBox(nullptr, L"Device, Context ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// SwapChain ���� & �ʱ�ȭ
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"SwapChain ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// RenderTarget, RenderTargetView, DepthStencil, DepthStencilView ���� & �ʱ�ȭ
	if (FAILED(CreateTargetView()))
	{
		MessageBox(nullptr, L"Ÿ�� �� View ���� ����", L"Device �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearRenderTarget(float(&_color)[4])
{
	m_Context->ClearRenderTargetView(m_RTView.Get(), _color);
	m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, (UINT8)0.f);
	m_Context->RSSetViewports(1, &m_Viewport);
}

void CDevice::Present()
{
	m_SwapChain->Present(0, 0);
}

int CDevice::CreateDevice()
{
	// ��ġ �ʱ�ȭ
	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;

	// Device ����
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
	// ������ SwapChain ���� ����ü
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		//desc.BufferCount = 1;
		//desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
		//desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
		//desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//desc.BufferDesc.RefreshRate.Denominator = 1;
		//desc.BufferDesc.RefreshRate.Numerator = 60;
		//desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		//desc.Flags = 0;
		//desc.SampleDesc.Count = 1;
		//desc.SampleDesc.Quality = 0;
		//desc.Windowed = true;
		//desc.OutputWindow = m_hRenderWnd;

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
		desc.Windowed = true; // â���
		desc.OutputWindow = m_hRenderWnd; // SwapChain �� ��� ������ ����
	}

	// SwapChain ������ ���� Factory�� ����
	ComPtr<IDXGIDevice>		pIdxgiDevice	= nullptr;
	ComPtr<IDXGIAdapter>	pAdapter		= nullptr;
	ComPtr<IDXGIFactory>	pFactory		= nullptr;
	{
		m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pIdxgiDevice.GetAddressOf());
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
	hr = m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf());
	CHECK(hr);

	// DepthStencil Texture ���� ����ü
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

	// DepthStencil Texture
	hr = m_Device->CreateTexture2D(&desc, nullptr, m_DSTex.GetAddressOf());
	CHECK(hr);
	// DepthStencil Texture View
	hr = m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf());
	CHECK(hr);

	CONTEXT->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

	// Viewport ����
	SetViewport();

	return S_OK;
}

void CDevice::SetViewport()
{
	// Viewport ����
	ZeroMemory(&m_Viewport, sizeof(m_Viewport));
	{
		m_Viewport.MinDepth = 0;
		m_Viewport.MaxDepth = 1.f;
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = m_vRenderResolution.x;
		m_Viewport.Height = m_vRenderResolution.y;
	}
}
