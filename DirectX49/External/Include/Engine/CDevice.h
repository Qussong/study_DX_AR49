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

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_arrSampler[2];

public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void ClearRenderTarget(float(&_color)[4]);
	void Present();

	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

	CConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_arrCB[(UINT)_type]; }

	Vec2 GetRenderResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11RasterizerState>	GetRSState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
	ComPtr<ID3D11BlendState>		GetBSState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }

private:
	int CreateDevice();	// mine
	int CreateSwapChain();
	int CreateTargetView();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

	int CreateConstBuffer();
};

