23.12.21<br>

## DirectXTex

### 파일추가
DirectTex 라이브러리 추가
```cpp
// header
path : $(SolutionDir)External\include\DirectTex
file : DirectXTex.h , DirectXTex.inl
// lib
path : $(SolutionDir)External\Library\DirectTex
file : DirectXTex.lib , DirectXTex_debug.lib
```

### 라이브러리 참조
```cpp
// [global.h]
#include <DirectXTex/DirectXTex.h>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif
```

## CTexture class

Texture 클래스 생성

### <span style="color:olive"> Load()
부모클래스인 CAsset class 에 Load 함수 추가하여 override<br>
CAssetMgr 클래스에서 CAsset 클래스의 private에 접근할 수 있도록 friend 선언추가해줌<br>
```cpp
// CAsset.h
virtual int Load(const wstring& _strFilePath) { return E_FAIL;}

friend class CAssetMgr;

// CTexture.h
virtual int Load(const wstring& _strFilePath) override;
```

## struct
상수버퍼 대응 구조체 tTransform에 멤버추가

### struct tTransform
```cpp
// before
struct tTransform
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
};

// after
struct tTransform
{
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	Matrix	matWV;
	Matrix	matWVP;
};
```

### cbuffer TRANSFORM : register(b0)
쉐이더 코드에서도 상수버퍼 구조체 변경
```cpp
// [std2d.fx]
// after
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
}
```

### CTransform
Trasform 클래스에서 상수버퍼에 tTransform 데이터를 옮겨주기전에 WV, WVP 행렬계산추가
```cpp
// [UpdateData()]
// before
g_transform.matWorld = m_matWorld;

// after
g_transform.matWorld = m_matWorld;
g_transform.matWV = g_transform.matWorld * g_transform.matView;
g_transform.matWVP = g_transform.matWV * g_transform.matProj;
```


### VS in Shader Code
상수버퍼의 구조가 변경됨으로인해 W,V,P 의 값이 WVP matrix 하나로 합쳐짐으로 VS코드의 Traslation의 코드가 변경된다.
```cpp
// before
float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
float4 vViewPos = mul(vWorldPos, g_matView);
float4 vProjPos = mul(vViewPos, g_matProj);
output.vPosition = vProjPos;

// after
output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
```

## Texture Load

### CAssetMgr class
AssetMgr 에 템플릿 함수로 로딩할 이미지의 경로와 key 값을 받는 에셋 로딩함수 추가<br>
**_Tip_**: CPathMgr 에서 ```GetContentPath()```로 content 폴더 경로까지 불러와주기에 그 이후의 경로만 적어주면 된다. ($(SolutionDir)OutputFile\content\...)
```cpp
// 선언
template<typename T>
T* Load(const wstring& _strKey, const wstring& _strRelativePath);
// 정의
template<typename T>
inline T* CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
	CAsset* pAsset = FindAsset<T>(_strKey);

	// 로딩할 때 사용할 키로 이미 다른 에셋이 있다면
	if (nullptr != pAsset)
	{
		return (T*)pAsset;
	}

	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	pAsset = new T;

	if (FAILED(pAsset->Load(strFilePath)))
	{
		MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 실패", MB_OK);
		delete pAsset;
		return nullptr;
	}

	pAsset->SetKey(_strKey);
	pAsset->SetRelativePath(_strRelativePath);
	AddAsset(_strKey, (T*)pAsset);

	return (T*)pAsset;
}
```

### CLevelMgr class


## 이미지 출력

### 쉐이더 파일
이미지 출력을 위해 쉐이더 파일에 변수 추가
```cpp
Texture2D g_tex_0 : register(t0);
SamplerState g_sam_0 : register(s0);
```
PS를 추가한 변수를 받아주는 코드로 변경
```cpp
// [PS_Std2D()] 
// before
_in.vColor.a = 0.1f;
return _in.vColor;
// after
float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
return vColor;
```

### CTexture class
UpdateData() 함수 추가
```cpp
// 선언
void CTexture::UpdateData(int _RegisterNum);
// 정의
void CTexture::UpdateData(int _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}
```
Load() 함수 재정의
```cpp
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

	return S_OK;
}
```

### CLevelMgr Init() 코드 추가
이거 왜 넣은건지 공부하기
```cpp
// Init()
if (nullptr != pTex)
{
    pTex->UpdateData(0);
}
```

### CAssetMgr

GetAsset() 함수 타입 추가
```cpp
// CAssetMgr.h
template<typename T>
inline ASSET_TYPE CAssetMgr::GetAssetType()
{
	const type_info& info = typeid(T);

	ASSET_TYPE Type = ASSET_TYPE::END;

	if (&info == &typeid(CMesh))
		Type = ASSET_TYPE::MESH;
	else if (&info == &typeid(CTexture))    // ★
		Type = ASSET_TYPE::TEXTURE;
	else if (&info == &typeid(CGraphicsShader))
		Type = ASSET_TYPE::GRAPHICS_SHADER;

	return Type;
}
```

uv 좌표 세팅
```cpp
// [CAssetMgr.cpp]
// 사각형 정점 위치 설정
arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
arrVtx[0].vUV = Vec2(0.f, 0.f);     // ★

arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
arrVtx[1].vUV = Vec2(1.f, 0.f);     // ★

arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
arrVtx[2].vUV = Vec2(1.f, 1.f);     // ★

arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
arrVtx[3].vUV = Vec2(0.f, 1.f);     // ★
```