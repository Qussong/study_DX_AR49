#include "pch.h"
#include "CAssetMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

CAssetMgr::CAssetMgr()
{
}

CAssetMgr::~CAssetMgr()
{
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		for (auto pair : m_mapAsset[i])
		{
			if (nullptr != pair.second)
			{
				delete pair.second;
			}
		}
		m_mapAsset[i].clear();
	}
}

void CAssetMgr::Init()
{
	CMesh* pMesh = nullptr;

	// Rectangle Mesh 생성
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

			// 인덱스 설정
			arrIdx[0] = 0;
			arrIdx[1] = 1;
			arrIdx[2] = 2;

			arrIdx[3] = 0;
			arrIdx[4] = 2;
			arrIdx[5] = 3;
		}

		pMesh = new CMesh;
		pMesh->Create(arrVtx, 4, arrIdx, 6);
		AddAsset(L"RectMesh", pMesh);
	}

	// Circle Mesh 생성
	{
		vector<Vtx>		vecVtx;
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

		pMesh = new CMesh;
		pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
		AddAsset(L"CircleMesh", pMesh);
	}

	// Shader 생성
	{
		CGraphicsShader* pShader = nullptr;

		pShader = new CGraphicsShader;
		pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
		pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

		AddAsset(L"Std2DShader", pShader);
	}
}