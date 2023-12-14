#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CTransform.h"
#include "CMeshRender.h"

vector<CGameObject*>	g_vecObj;

CMesh*					g_RectMesh		= nullptr;
CMesh*					g_CircleMesh	= nullptr;
CGraphicsShader*		g_Shader		= nullptr;

int TestInit()
{
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

		g_RectMesh = new CMesh;
		g_RectMesh->Create(arrVtx, 4, arrIdx, 6);
	}

	// Circle Mesh 생성
	{
		vector<Vtx>		vecVtx;
		vector<UINT>	vecIdx;

		// 중심 점
		Vtx			v;
		v.vPos		= Vec3(0.f, 0.f, 0.f);
		v.vColor	= Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV		= Vec2(0.5f, 0.5f);
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

		g_CircleMesh = new CMesh;
		g_CircleMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	}

	// Shader 생성
	{
		g_Shader = new CGraphicsShader;
		g_Shader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
		g_Shader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	}

	// GameObject 생성
	{
		CGameObject* pObj = nullptr;
		pObj = new CGameObject;

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);

		pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));

		pObj->MeshRender()->SetMesh(g_RectMesh);
		pObj->MeshRender()->SetShader(g_Shader);

		g_vecObj.push_back(pObj);

		pObj = new CGameObject;

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);

		pObj->Transform()->SetRelativePos(Vec3(0.5f, 0.25f, 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));

		pObj->MeshRender()->SetMesh(g_RectMesh);
		pObj->MeshRender()->SetShader(g_Shader);

		g_vecObj.push_back(pObj);
	}

	return S_OK;
}

void Tick()
{
	for (size_t i = 0; i < g_vecObj.size(); ++i)
	{
		g_vecObj[i]->Tick();
		g_vecObj[i]->FinalTick();
	}
	//// Transform
	//if (KEY_PRESSED(KEY::LEFT))
	//{
	//	g_Transform.vWorldPos.x -= DT;
	//}
	//if (KEY_PRESSED(KEY::RIGHT))
	//{
	//	g_Transform.vWorldPos.x += DT;
	//}
	//if (KEY_PRESSED(KEY::UP))
	//{
	//	g_Transform.vWorldPos.y += DT;
	//}
	//if (KEY_PRESSED(KEY::DOWN))
	//{
	//	g_Transform.vWorldPos.y -= DT;
	//}
	//if (KEY_PRESSED(KEY::Q))
	//{
	//	g_Transform.vWorldScale += DT * Vec4(1.f, 1.f, 1.f, 1.f);
	//}
	//if (KEY_PRESSED(KEY::W))
	//{
	//	g_Transform.vWorldScale -= DT * Vec4(1.f, 1.f, 1.f, 1.f);
	//}
}

void Render()
{
	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	for (size_t i = 0; i < g_vecObj.size(); ++i)
	{
		g_vecObj[i]->Render();
	}

	CDevice::GetInst()->Present();
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

	Delete_Vec(g_vecObj);
}