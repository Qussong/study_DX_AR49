#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

Vtx							g_vtx[6]		= {};		// 사각형의 정점 정보 (사각형의 정점개수 = 삼각형*2 = 6개)
ComPtr<ID3D11Buffer>		g_VB			= nullptr;	// 정점버퍼
ComPtr<ID3DBlob>			g_VSBlob		= nullptr;	// VS 컴파일 정보 저장	
ComPtr<ID3D11VertexShader>	g_VS			= nullptr;	// 버텍스 쉐이더
ComPtr<ID3D11InputLayout>	g_Layout		= nullptr;	// 정점 하나의 구조를 알리는 객체
ComPtr<ID3DBlob>			g_PSBlob		= nullptr;	// PS 컴파일 정보 저장
ComPtr<ID3D11PixelShader>	g_PS			= nullptr;	// 픽셀 쉐이더
ComPtr<ID3DBlob>			g_ErrBlob		= nullptr;	// Shader 생성중 발생한 Error 메세지 저장

int TestInit()
{
	// 사각형 정점 위치 설정
	// 1번 삼각형
	{
		// 0(R)
		// |   \
		// 2(B)-1(G)
		g_vtx[0].vPos	= Vec3(-0.5f, 0.5f, 0.f);
		g_vtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		g_vtx[0].vUV	= Vec2(0.f, 0.f);
						  
		g_vtx[1].vPos	= Vec3(0.5f, -0.5f, 0.f);
		g_vtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
		g_vtx[1].vUV	= Vec2(0.f, 0.f);
						  
		g_vtx[2].vPos	= Vec3(-0.5f, -0.5f, 0.f);
		g_vtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
		g_vtx[2].vUV	= Vec2(0.f, 0.f);
	}					  
	// 2번 삼각형		  
	{					  
		// 3(R)-4(B)	  
		//	  \   |		  
		//	   5(G)		  
		g_vtx[3].vPos	= Vec3(-0.5f, 0.5f, 0.f);
		g_vtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		g_vtx[3].vUV	= Vec2(0.f, 0.f);
						  
		g_vtx[4].vPos	= Vec3(0.5f, 0.5f, 0.f);
		g_vtx[4].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
		g_vtx[4].vUV	= Vec2(0.f, 0.f);
						  
		g_vtx[5].vPos	= Vec3(0.5f, -0.5f, 0.f);
		g_vtx[5].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
		g_vtx[5].vUV	= Vec2(0.f, 0.f);
	}

	// 정점버퍼 생성 & 초기화
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(nullptr, L"Vertex Buffer 생성 실패", L"TestInit 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateVertexShader()))
	{
		MessageBox(nullptr, L"Vertex Shader 생성 실패", L"TestInit 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateInputLayout()))
	{
		MessageBox(nullptr, L"Input Layout 생성 실패", L"TestInit 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreatePixelShader()))
	{
		MessageBox(nullptr, L"Pixel Shader 생성 실패", L"TestInit 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void TestProgress()
{
	Tick();

	Render();

	
}

void TestRelease()
{

}

void Tick()
{
	// Vertex Buffer의 정점정보를 변경하여 객체를 움직인다.

	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 6; ++i)
		{
			g_vtx[i].vPos.x -= DT;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 6; ++i)
		{
			g_vtx[i].vPos.x += DT;
		}
	}

	if (KEY_PRESSED(KEY::UP))
	{
		for (int i = 0; i < 6; ++i)
		{
			g_vtx[i].vPos.y += DT;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 6; ++i)
		{
			g_vtx[i].vPos.y -= DT;
		}
	}

	// SystemMem -> GPUMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(g_VB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy(tSub.pData, g_vtx, sizeof(Vtx) * 6);

	CONTEXT->Unmap(g_VB.Get(), 0);
}

void Render()
{
	// Window 배경색 설정
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// 사각형 그리기
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CONTEXT->IASetInputLayout(g_Layout.Get());

	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	CONTEXT->Draw(6, 0);

	// 메모리 상에 작업한 삼각형 그림을 Window 에 그려준다.
	CDevice::GetInst()->Present();
}

int CreateVertexBuffer()
{
	// 버텍스 버퍼 생성 구조체
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	{
		bufferDesc.ByteWidth = sizeof(Vtx) * 6;				// 버퍼의 크기
		bufferDesc.StructureByteStride = sizeof(Vtx);		// 정점 하나의 크기
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 용도설정 = 버텍스 버퍼
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// Buffer에 데이터 쓰기 기능
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}

	// 버텍스 버퍼에 들어갈 정점들의 초기 값 설정
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = g_vtx;

	// Vertex Buffer
	HRESULT hr = DEVICE->CreateBuffer(&bufferDesc, &tSubData, g_VB.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CreateVertexShader()
{
	HRESULT hr = NULL;

	// shader 파일 경로
	wstring strFilePath = CPathMgr::GetContentPath();

	// HLSL 버텍스 쉐이더 함수 컴파일
	hr = D3DCompileFromFile(
		wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Std2D", "vs_5_0"
		, D3DCOMPILE_DEBUG
		, 0
		, g_VSBlob.GetAddressOf()
		, g_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Vertex Shader Compile Failed!!", MB_OK);
		}
		return E_FAIL;
	}

	hr = DEVICE->CreateVertexShader(
		g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, nullptr
		, g_VS.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CreateInputLayout()
{
	// 삼각형 정점 구조정보(Layout)
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};
	{
		arrElement[0].InputSlot = 0;
		arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrElement[0].SemanticName = "POSITION";
		arrElement[0].SemanticIndex = 0;
		arrElement[0].InstanceDataStepRate = 0;
		arrElement[0].AlignedByteOffset = 0;
		arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

		arrElement[1].InputSlot = 0;
		arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrElement[1].SemanticName = "COLOR";
		arrElement[1].SemanticIndex = 0;
		arrElement[1].InstanceDataStepRate = 0;
		arrElement[1].AlignedByteOffset = 12;
		arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		arrElement[2].InputSlot = 0;
		arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrElement[2].SemanticName = "TEXCOORD";
		arrElement[2].SemanticIndex = 0;
		arrElement[2].InstanceDataStepRate = 0;
		arrElement[2].AlignedByteOffset = 28;
		arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	}

	// Layout 생성
	HRESULT hr = DEVICE->CreateInputLayout(arrElement, 3
		, g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CreatePixelShader()
{
	HRESULT hr = NULL;

	// shader 파일 경로
	wstring strFilePath = CPathMgr::GetContentPath();

	// HLSL 버텍스 쉐이더 함수 컴파일
	hr = D3DCompileFromFile(
		wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Std2D", "ps_5_0"
		, D3DCOMPILE_DEBUG
		, 0
		, g_PSBlob.GetAddressOf()
		, g_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Pixel Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	hr = DEVICE->CreatePixelShader(
		g_PSBlob->GetBufferPointer()
		, g_PSBlob->GetBufferSize()
		, nullptr
		, g_PS.GetAddressOf());
	CHECK(hr);

	return S_OK;
}