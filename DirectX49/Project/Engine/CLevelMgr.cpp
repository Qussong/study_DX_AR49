#include "pch.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "components.h"
#include "CPlayerScript.h"

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

	// GameObject 생성
	CGameObject* pObj = nullptr;

	pObj = new CGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

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
