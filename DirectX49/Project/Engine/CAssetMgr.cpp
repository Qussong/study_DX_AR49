#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

CAssetMgr::CAssetMgr()
{
}

CAssetMgr::~CAssetMgr()
{

}

void CAssetMgr::Init()
{
	// Rectangle Mesh 생성
	CreateDefaultMesh();

	// Std2D Shader 생성
	CreateDefaultGraphicsShader();

	// Std2D Material 생성
	CreateDefaultMaterial();
}

void CAssetMgr::CreateDefaultMesh()
{
	// RectMesh 생성
	Vtx		arrVtx[4] = {};	// 사각형의 정점 정보 (사각형의 정점개수 = 4개)
	{
		// 0(R)--- 1(B)	     
		//  |   \   |	     
		// 3(G)--- 2(M)  
		UINT	arrIdx[6] = {};		// 사각형 정점의 인덱스 정보

		arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
		arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		arrVtx[0].vUV = Vec2(0.f, 0.f);

		arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
		arrVtx[1].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
		arrVtx[1].vUV = Vec2(1.f, 0.f);

		arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
		arrVtx[2].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
		arrVtx[2].vUV = Vec2(1.f, 1.f);

		arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
		arrVtx[3].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
		arrVtx[3].vUV = Vec2(0.f, 1.f);

		// 인덱스 설정
		arrIdx[0] = 0;
		arrIdx[1] = 1;
		arrIdx[2] = 2;

		arrIdx[3] = 0;
		arrIdx[4] = 2;
		arrIdx[5] = 3;

		CMesh* pRectMesh = new CMesh;
		pRectMesh->Create(arrVtx, 4, arrIdx, 6);
		AddAsset(L"RectMesh", pRectMesh);
	}

	// RectMesh_Debug (Topology LineStrip)
	{
		// 0(Red)-- 1(Blue)	     
		//  |       |	     
		// 3(G)---- 2(Magenta)
		UINT	arrIdx[5] = {};

		arrIdx[0] = 0;	
		arrIdx[1] = 1;	
		arrIdx[2] = 2;	
		arrIdx[3] = 3; 	
		arrIdx[4] = 0;

		CMesh* pDebugRectMesh = new CMesh;
		pDebugRectMesh->Create(arrVtx, 4, arrIdx, 5);
		AddAsset(L"RectMesh_Debug", pDebugRectMesh);
	}

	// CircleMesh 생성
	vector<Vtx>		vecVtx;
	{
		vector<UINT>	vecIdx;

		// 중심 점
		Vtx			v;
		v.vPos = Vec3(0.f, 0.f, 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.5f, 0.5f);
		vecVtx.push_back(v);

		UINT	iSlice = 40;
		float	fTheta = 0.f;
		float	fRadius = 0.5f;

		// 월 둘레를 이루는 정점의 위치값 설정
		for (int i = 0; i <= iSlice; ++i)
		{
			fTheta = (XM_2PI / iSlice) * i;

			v.vPos = Vec3(fRadius * cosf(fTheta), fRadius * sinf(fTheta), 0.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vUV = Vec2(0.f, 0.f);

			vecVtx.push_back(v);
		}

		// 정점 연결 순서 설정
		for (int i = 0; i < iSlice; ++i)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}

		CMesh* pCircleMesh = new CMesh;
		pCircleMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddAsset(L"CircleMesh", pCircleMesh);
	}

	// CircleMesh_Debug 생성
	{
		vector<UINT>	vecIdx;

		for (int i = 1; i < vecVtx.size(); ++i)
		{
			vecIdx.push_back(i);
		}

		CMesh* pDebugCircleMesh = new CMesh;
		pDebugCircleMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddAsset(L"CircleMesh_Debug", pDebugCircleMesh);
	}

	// CrossMesh 생성
	{
		Vtx				v;
		vector<UINT>	vecIdx;
		vecVtx.clear();

		v.vPos = Vec3(0.f, 0.5f, 0.f);
		v.vUV = Vec2(0.f, 0.f);
		v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
		vecVtx.push_back(v);

		v.vPos = Vec3(0.f, -0.5f, 0.f);
		vecVtx.push_back(v);

		v.vPos = Vec3(-0.5f, 0.f, 0.f);
		vecVtx.push_back(v);

		v.vPos = Vec3(0.5f, 0.f, 0.f);
		vecVtx.push_back(v);

		vecIdx.push_back(0); 
		vecIdx.push_back(1);
		vecIdx.push_back(2); 
		vecIdx.push_back(3);

		CMesh* pCrossMesh = new CMesh;
		pCrossMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddAsset(L"CrossMesh", pCrossMesh);
		vecVtx.clear();
		vecIdx.clear();
	}
}

void CAssetMgr::CreateDefaultGraphicsShader()
{
	// Std2D Shader 생성
	{
		CGraphicsShader* pShader = new CGraphicsShader;
		pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
		pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		//pShader->SetBSType(BS_TYPE::DEFAULT);
		pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
		pShader->SetDSType(DS_TYPE::LESS);

		AddAsset(L"Std2DShader", pShader);
	}

	// Effect Shader 생성
	{
		CGraphicsShader* pShader = new CGraphicsShader;
		pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
		pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetBSType(BS_TYPE::ONE_ONE);
		pShader->SetDSType(DS_TYPE::LESS);

		AddAsset(L"EffectShader", pShader);
	}

	// DebugShape Shader 생성
	{
		CGraphicsShader* pShader = new CGraphicsShader;
		pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
		pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");
		pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);	// default = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetBSType(BS_TYPE::DEFAULT);
		pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		AddAsset(L"DebugShapeShader", pShader);
	}
}

void CAssetMgr::CreateDefaultMaterial()
{
	// Std2D Material
	{
		CMaterial* pMtrl = new CMaterial;
		pMtrl->SetShader(FindAsset<CGraphicsShader>(L"Std2DShader"));
		AddAsset<CMaterial>(L"Std2DMtrl", pMtrl);
	}

	// DebugShape Material
	{
		CMaterial* pMtrl = new CMaterial;
		pMtrl->SetShader(FindAsset<CGraphicsShader>(L"DebugShapeShader"));
		AddAsset<CMaterial>(L"DebugShapeMtrl", pMtrl);
	}
}
