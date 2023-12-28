23.12.28<br>

## GarbageCollector 추가
```cpp
class CGC :
    public CSingleton<CGC>
{
    SINGLE(CGC);
private:
    vector<CEntity*>    m_vecEntity;    // 메모리해제할 애들

public:
    void Tick();

    void Add(CEntity* _entity)
    {
        m_vecEntity.push_back(_entity); // 메모리해제할 애들 추가
    }
};
```

## SamplerState 작업

### Device class
```cpp
// Device.h
private:
    ComPtr<ID3D11SamplerState>		m_arrSampler[2];

private:
    int CreateSamplerState();
```
```cpp
// Device.cpp
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
```

### CGameObject
```cpp
// CGameObject.h
private:
    bool    m_bDead;

public:
    bool IsDead() { return m_bDead; }
```
```cpp
// CGameObject.cpp
void CGameObject::FinalTick()
{

    // ...

	vector<CGameObject*>::iterator itr = m_vecChild.begin();
	for (; itr != m_vecChild.end();)
	{
		(*itr)->FinalTick();

		if ((*itr)->m_bDead)
		{
			CGC::GetInst()->Add(*itr);
			itr = m_vecChild.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}
```