#include "pch.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CTexture.h"
#include "CCollisionMgr.h"

CLevelMgr::CLevelMgr()
	: m_curLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_curLevel)
		delete m_curLevel;
}

void CLevelMgr::Init()
{
	// 초기 레벨 구성
	// Layer maxCnt = 32
	m_curLevel = new CLevel;
	m_curLevel->GetLayer(0)->SetName(L"Default");
	m_curLevel->GetLayer(1)->SetName(L"Background");
	m_curLevel->GetLayer(2)->SetName(L"Tile");
	m_curLevel->GetLayer(3)->SetName(L"Player");
	m_curLevel->GetLayer(4)->SetName(L"Monster");
	m_curLevel->GetLayer(31)->SetName(L"UI");

	// 충돌 설정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");

	// MainCamera Object 생성
	CGameObject* pCamObj = new CGameObject;
	{
		pCamObj->SetName(L"MainCamera");
		pCamObj->AddComponent(new CTransform);
		pCamObj->AddComponent(new CCamera);
		pCamObj->AddComponent(new CCameraMoveScript);

		pCamObj->Transform()->SetRelativePos(Vec3(0.0f, 0.f, 0.f));
		pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

		pCamObj->Camera()->SetCameraPriority(0);
		pCamObj->Camera()->LayerCheckAll();
		pCamObj->Camera()->LayerCheck(L"UI", false);

		m_curLevel->AddObject(pCamObj, 0);
	}

	// SubCamera Object 생성
	{
		pCamObj = new CGameObject;
		pCamObj->SetName(L"UICamera");
		pCamObj->AddComponent(new CTransform);
		pCamObj->AddComponent(new CCamera);

		pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
		pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

		pCamObj->Camera()->SetCameraPriority(1);
		pCamObj->Camera()->LayerCheck(L"UI", true);

		m_curLevel->AddObject(pCamObj, 0);
	}


	// Player Object 생성
	CGameObject* pObj = nullptr;
	{
		pObj = new CGameObject;
		pObj->SetName(L"Player");
		// Component
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CCollider2D);
		pObj->AddComponent(new CAnimator2D);
		pObj->AddComponent(new CPlayerScript);

		pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
		pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

		pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
		pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
		//pObj->Collider2D()->SetColliderType(COLLIDER2D_TYPE::CIRCLE);

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.f);

		Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTexture", L"texture\\player2_bmp.bmp");
		pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);

		Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"AnimAtlasTex", L"texture\\link.png");
		pObj->Animator2D()->Create(L"Explosion", pAltasTex, Vec2(0.f, 520.f)
			, Vec2(120.f, 130.f), Vec2(0.f, 0.f), 10, 24.f);
		pObj->Animator2D()->Play(L"Explosion");

		m_curLevel->AddObject(pObj, L"Player", false);
	}

	// Monster Object 생성
	{
		pObj = new CGameObject;
		pObj->SetName(L"Monster");

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CCollider2D);

		pObj->Transform()->SetRelativePos(Vec3(300.f, 100.f, 400.f));
		pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

		pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
		pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.f);

		m_curLevel->AddObject(pObj, L"Monster", false);
	}

	// Monster Object 생성 2
	{
		pObj = new CGameObject;
		pObj->SetName(L"Monster");

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CCollider2D);

		pObj->Transform()->SetRelativePos(Vec3(300.f, -100.f, 400.f));
		pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

		pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
		pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
		pObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.f);

		m_curLevel->AddObject(pObj, L"Monster", false);
	}

	// UI Object 생성
	{
		pObj = new CGameObject;
		pObj->SetName(L"UI");

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);

		pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
		pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

		m_curLevel->AddObject(pObj, L"UI", false);
	}

	//GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
}

void CLevelMgr::Tick()
{
	if (nullptr == m_curLevel)
		return;

	// 이전 프레임에 등록된 오브젝트들 clear
	m_curLevel->Clear();

	m_curLevel->Tick();
	m_curLevel->FinalTick();
}

//void CLevelMgr::Render()
//{
//	if (nullptr == m_curLevel)
//		return;
//
//	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
//	CDevice::GetInst()->ClearRenderTarget(ClearColor);
//
//	m_curLevel->Render();
//
//	CDevice::GetInst()->Present();
//}
