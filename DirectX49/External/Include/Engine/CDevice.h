#pragma once

class CConstBuffer;

// DX11 기준 CPU 제어
class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);

private:
	// Basic Object
	ComPtr<ID3D11Device>			m_Device;
	ComPtr<ID3D11DeviceContext>		m_Context;
	ComPtr<IDXGISwapChain>			m_SwapChain;

	// Output Merge State (OM) 
	ComPtr<ID3D11Texture2D>			m_RTTex;
	ComPtr<ID3D11RenderTargetView>	m_RTView;
	ComPtr<ID3D11Texture2D>			m_DSTex;
	ComPtr<ID3D11DepthStencilView>	m_DSView;

	HWND							m_hRenderWnd;
	Vec2							m_vRenderResolution;

	CConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void ClearRenderTarget(float(&_color)[4]);
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

	CConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }

private:
	int CreateDevice();
	int CreateSwapChain();
	int CreateTargetView();
	int CreateConstBuffer();
};

