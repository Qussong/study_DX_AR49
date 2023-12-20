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

CLevelMgr::CLevelMgr()
	: m_curLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
}

void CLevelMgr::Init()
{
	// 초기 레벨 구성
	m_curLevel = new CLevel;

	// Camera Object 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.0f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	m_curLevel->AddObject(pCamObj, 0);

	// GameObject 생성
	CGameObject* pObj = nullptr;

	pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CPlayerScript);

	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.0f));	// 회전값이 왜 이상하게 출력되는가?
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

	m_curLevel->AddObject(pObj, 0);
}

void CLevelMgr::Tick()
{
	if (nullptr == m_curLevel)
		return;

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
