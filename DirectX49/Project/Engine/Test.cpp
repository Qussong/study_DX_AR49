#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

CMesh*					g_RectMesh		= nullptr;
CMesh*					g_CircleMesh	= nullptr;
ComPtr<ID3D11Buffer>	g_CB			= nullptr;
CGraphicsShader*		g_Shader		= nullptr;
tTransform				g_Transform		= { Vec4(1.f, 1.f, 1.f, 1.f), Vec4(0.f, 0.f, 0.f, 0.f)};

int TestInit()
{
	// Rectangle Mesh 생성
	CreateRectMesh();
	// Circle Mesh 생성
	CreateCircleMesh();

	// Constant Buffer 생성
	CreateConstantBuffer();

	// Shader 생성
	g_Shader = new CGraphicsShader;
	g_Shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	g_Shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	return S_OK;
}

void TestProgress()
{
	Tick();

	Render();
}

void TestRelease()
{
	if (nullptr != g_RectMesh)
		delete g_RectMesh;

	if (nullptr != g_CircleMesh)
		delete g_CircleMesh;

	if (nullptr != g_Shader)
		delete g_Shader;
}

void Tick()
{
	// Transform
	if (KEY_PRESSED(KEY::LEFT))
	{
		g_Transform.vWorldPos.x -= DT;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		g_Transform.vWorldPos.x += DT;
	}

	if (KEY_PRESSED(KEY::UP))
	{
		g_Transform.vWorldPos.y += DT;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		g_Transform.vWorldPos.y -= DT;
	}
	// Scale
	if (KEY_PRESSED(KEY::Q))
	{
		g_Transform.vWorldScale += DT * Vec4(1.f, 1.f, 1.f, 1.f);
	}

	if (KEY_PRESSED(KEY::W))
	{
		g_Transform.vWorldScale -= DT * Vec4(1.f, 1.f, 1.f, 1.f);
	}

	// SystemMem -> GPUMem
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(g_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &g_Transform, sizeof(tTransform) * 1);
	CONTEXT->Unmap(g_CB.Get(), 0);
}

void Render()
{
	// Window 배경색 설정
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// 상수버퍼 전달(바인딩)
	CONTEXT->VSSetConstantBuffers(0, 1, g_CB.GetAddressOf());

	// 쉐이더 세팅
	g_Shader->UpdateData();

	// 메시 선택 및 렌더
	//g_RectMesh->render();
	g_CircleMesh->render();

	// 메모리 상에 작업한 삼각형 그림을 Window 에 그려준다.
	CDevice::GetInst()->Present();
}

void CreateRectMesh()
{
	Vtx		arrVtx[4] = {};		// 사각형의 정점 정보 (사각형의 정점개수 = 4개)
	UINT	arrIdx[6] = {};		// 사각형 정점의 인덱스 정보
	{
		// 0(R)--- 1(B)	     
		//  |   \   |	     
		// 3(G)--- 2(M)  

		// 사각형 정점 위치 설정
		arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
		arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		arrVtx[0].vUV = Vec2(0.f, 0.f);

		arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
		arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
		arrVtx[1].vUV = Vec2(0.f, 0.f);

		arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
		arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
		arrVtx[2].vUV = Vec2(0.f, 0.f);

		arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
		arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
		arrVtx[3].vUV = Vec2(0.f, 0.f);

		// 사각형 인덱스 설정
		arrIdx[0] = 0;
		arrIdx[1] = 1;
		arrIdx[2] = 2;

		arrIdx[3] = 0;
		arrIdx[4] = 2;
		arrIdx[5] = 3;
	}

	// Rectangle Mesh 생성
	g_RectMesh = new CMesh;
	g_RectMesh->Create(arrVtx, 4, arrIdx, 6);
}

void CreateCircleMesh()
{
	vector<Vtx>		vecVtx;
	vector<UINT>	vecIdx;

	// 중심 점
	Vtx			v;
	v.vPos		= Vec3(0.f, 0.f, 0.f);
	v.vColor	= Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV		= Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT	iSlice	= 40;
	float	fTheta	= 0.f;
	float	fRadius = 0.5f;

	for (int i = 0; i <= iSlice; ++i)
	{
		fTheta = (XM_2PI / iSlice) * i;

		v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.f, 0.f);

		vecVtx.push_back(v);
	}

	for (int i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	g_CircleMesh = new CMesh;
	g_CircleMesh->Create(
		vecVtx.data()
		, (UINT)vecVtx.size()
		, vecIdx.data()
		, (UINT)vecIdx.size());

}

void CreateConstantBuffer()
{
	// 상수 버퍼 생성 구조체
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	{
		bufferDesc.ByteWidth = sizeof(tTransform);				// 상수버퍼의 크기
		bufferDesc.StructureByteStride = sizeof(tTransform);	// 상수버퍼의 인자 하나의 크기
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// 용도설정 = 상수버퍼 
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// 버퍼에 쓰기 가능
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}

	// 상수버퍼
	HRESULT hr = DEVICE->CreateBuffer(&bufferDesc, nullptr, g_CB.GetAddressOf());
	if (FAILED(hr))
		MessageBox(nullptr, L"Constant Buffer 생성 실패", L"TestInit 오류", MB_OK);
}