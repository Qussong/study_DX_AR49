#include "pch.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

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
	m_curLevel = new CLevel;

	// 파이리
	CTexture* pTex = CAssetMgr::GetInst()
								->Load<CTexture>(L"PlayerTexture", L"texture\\player.png");
	if (nullptr != pTex)
	{
		pTex->UpdateData(0);
	}
	// 꼬부기
	CTexture* pTex2 = CAssetMgr::GetInst()
		->Load<CTexture>(L"PlayerTexture2", L"texture\\player2.png");
	if (nullptr != pTex2)
	{
		pTex2->UpdateData(1);
	}

	// Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	{
		pCamObj->AddComponent(new CTransform);
		pCamObj->AddComponent(new CCamera);
		pCamObj->AddComponent(new CCameraMoveScript);

		pCamObj->Transform()->SetRelativePos(Vec3(0.0f, 0.f, 0.f));
		pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	}
	m_curLevel->AddObject(pCamObj, 0);

	// GameObject 생성
	CGameObject* pObj = nullptr;
	{
		pObj = new CGameObject;
		pObj->SetName(L"Player");

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CPlayerScript);

		pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
		pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

		pObj->SetImgNum((UINT)IMGNUM::Charmander);

		// 자식객체 생성
		CGameObject* pChildObj = new CGameObject;
		{
			pChildObj->SetName(L"Child");

			pChildObj->AddComponent(new CTransform);
			pChildObj->AddComponent(new CMeshRender);

			pChildObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 0.f));
			pChildObj->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
			pChildObj->Transform()->SetAbsolute(true);

			pChildObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			pChildObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

			pChildObj->SetImgNum((UINT)IMGNUM::Squirtle);

			pObj->AddChild(pChildObj);
		}
	}
	m_curLevel->AddObject(pObj, 0, false);
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

void CLevelMgr::Render()
{
	if (nullptr == m_curLevel)
		return;

	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	m_curLevel->Render();

	CDevice::GetInst()->Present();
}
