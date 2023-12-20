23.12.20<br>

## Rasterizer State 추가
### <span style="color:olive"> <span style="color:olive">CDevice class
```cpp
// [header]
ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_arrRS[(UINT)_Type]; }
int CreateRasterizerState();

// [cpp]
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
```
### <span style="color:olive"> <span style="color:olive">define.h
```cpp
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,

	END,
};
```

## DepthStencil State 추가
### <span style="color:olive"> CDevice class
```cpp
// [header]
ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_arrDS[(UINT)_Type]; }
int CreateDepthStencilState();

// [cpp]
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
```
### <span style="color:olive"> define.h
```cpp
enum class DS_TYPE
{
	LESS,				// 작은게 통과			    깊이 기록 O
	LESS_EQUAL,			// 작거나 같으면 통과		깊이 기록 O

	GRATER,				// 더 멀면 통과			    깊이 기록 O
	GRATER_EQUAL,		// 더 멀거나 같으면 통과	깊이 기록 O

	NO_TEST,			// 깊이 테스트 X			깊이 기록 O
	NO_WRITE,			// 깊이 테스트 O			깊이 기록 X

	NO_TEST_NO_WRITE,	// 깊이 테스트 X			깊이 기록 X

	END,
};
```

## Blend State 추가
### <span style="color:olive"> CDevice class
```cpp
// [header]
ComPtr<ID3D11BlendState>		m_arrBS[(UINT)DS_TYPE::END];
ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_arrBS[(UINT)_Type]; }
int CreateBlendState();

// [cpp]
int CDevice::CreateBlendState()
{
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC tDesc = {};

	// AlphaBlend
	{
		tDesc.AlphaToCoverageEnable = false;
		tDesc.IndependentBlendEnable = true;

		tDesc.RenderTarget[0].BlendEnable = true;
		tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	HRESULT hr = DEVICE->CreateBlendState(&tDesc, m_arrBS[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());
	CHECK(hr);

	return S_OK;
}
```
### <span style="color:olive"> define.h
```cpp
enum class BS_TYPE
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_ONE,

	END,
};
```

## CGraphicShader class

### <span style="color:olive"> 멤버변수 추가
RasterizerState, OutputMerge(DepthStencilState, BlendState) 설정값 추가
```cpp
// [header]
RS_TYPE     m_RSType;
DS_TYPE     m_DSType;
BS_TYPE     m_BSType;
```
### <span style="color:olive"> UpdateData()
```cpp
void CGraphicsShader::UpdateData()
{
    // ...
    
    CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());
    CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
    CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);

    // ...
}
```

## CAssetMgr class
쉐이더 생성시 RS, DS, BS 값 설정 코드 추가
```cpp
	CGraphicsShader* pShader = new CGraphicsShader;
	{
        // ...

		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDSType(DS_TYPE::LESS);
		pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

        // ...
	}
```
