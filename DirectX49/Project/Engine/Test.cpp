#include "pch.h"
#include "Test.h"

#include "CDevice.h"

// 삼각형의 정점 정보 (삼각형의 정점개수 = 3개)
Vtx g_vtx[3] = {};
// 사각형의 정점 정보 (사각형의 정점개수 = 4개)
Vtx g_vtx2[4] = {};

ComPtr<ID3D11Buffer>		g_VB = nullptr;	// 정점버퍼
ComPtr<ID3D11VertexShader>	g_VS = nullptr;	// 버텍스 쉐이더
ComPtr<ID3D11PixelShader>	g_PS = nullptr;	// 픽셀 쉐이더


int TestInit()
{
	g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);

	g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_vtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_vtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[2].vUV = Vec2(0.f, 0.f);

	// 정점버퍼 생성 & 초기화
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"TextInit 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

int CreateVertexBuffer()
{
	// 버텍스 버퍼 생성 구조체
	D3D11_BUFFER_DESC bufferDesc = {};
	{
		bufferDesc.ByteWidth = sizeof(Vtx) * 3;				// 버퍼의 크기
		bufferDesc.StructureByteStride = sizeof(Vtx);		// 정점 하나의 크기
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 용도설정 = 버텍스 버퍼
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void TestProgress()
{
	// Window 배경색 설정
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// 삼각형 그리기
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);
	//CONTEXT->Draw(3, 0);

	// 메모리 상에 작업한 삼각형 그림을 Window 에 그려준다.
	CDevice::GetInst()->Present();
}

int TestInit2()
{
	g_vtx2[0].vPos = Vec3(0.5f, 0.5f, 0.f);
	g_vtx2[0].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[0].vUV = Vec2(0.f, 0.f);
		 
	g_vtx2[1].vPos = Vec3(0.5f, -0.5f, 0.f);
	g_vtx2[1].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[1].vUV = Vec2(0.f, 0.f);
		 
	g_vtx2[2].vPos = Vec3(-0.5f, -0.5f, 0.f);
	g_vtx2[2].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[2].vUV = Vec2(0.f, 0.f);

	g_vtx2[3].vPos = Vec3(-0.5f, 0.5f, 0.f);
	g_vtx2[3].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[3].vUV = Vec2(0.f, 0.f);

	// 정점버퍼 생성 & 초기화
	if (FAILED(CreateVertexBuffer2()))
	{
		MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"TextInit 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

int CreateVertexBuffer2()
{
	// 버텍스 버퍼 생성 구조체
	D3D11_BUFFER_DESC bufferDesc = {};
	{
		bufferDesc.ByteWidth = sizeof(Vtx) * 4;				// 버퍼의 크기
		bufferDesc.StructureByteStride = sizeof(Vtx);		// 정점 하나의 크기
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 용도설정 = 버텍스 버퍼
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void TestProgress2()
{
	// Window 배경색 설정
	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// 삼각형 그리기
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);
	CONTEXT->Draw(4, 0);

	// 메모리 상에 작업한 사각형 그림을 Window 에 그려준다.
	CDevice::GetInst()->Present();
}

void TestRelease()
{

}