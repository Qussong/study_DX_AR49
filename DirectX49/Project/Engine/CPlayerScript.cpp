#include "pch.h"
#include "CPlayerScript.h"
#include "CAssetMgr.h"

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: m_speed(500.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	// 이동
	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += DT * m_speed;
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= DT * m_speed;
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT * m_speed;
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT * m_speed;
	}

	// 회전
	if (KEY_PRESSED(KEY::X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += DT * XM_PI;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);

	if (KEY_TAP(KEY::SPACE))
	{
		//Destroy();

		// GameObject 생성
		CGameObject* pObj = nullptr;

		pObj = new CGameObject;
		pObj->SetName(L"Missile");
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CMissileScript);

		pObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		//pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));
		pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

		GamePlayStatic::SpawnGameObject(pObj, 0);
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(INT_0, 1);
		}
	}
	else if (KEY_RELEASED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(INT_0, 0);
		}
	}
	//GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
	//GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f), true);
}
