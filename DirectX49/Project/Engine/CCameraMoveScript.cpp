#include "pch.h"
#include "CCameraMoveScript.h"

CCameraMoveScript::CCameraMoveScript()
	: m_camSpeed(500.f)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Tick()
{
	if (KEY_TAP(KEY::P))
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
		{
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
			Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}
	}

	// Traslation
	if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
	{
		MoveOrthographic();
	}
	else
	{
		MovePerspective();
	}

	// Scale
	if(KEY_WHEEL < 0 /*KEY_PRESSED(KEY::O)*/)
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() + DT * 100.f);
		else
			Camera()->SetFOV(Camera()->GetFOV() + DT * 100.f);

		// 마우스 휠 값 초기화
		CKeyMgr::GetInst()->SetMouseWheel(0);
	}
	if(KEY_WHEEL > 0 /*KEY_PRESSED(KEY::I)*/)
	{
		if (Camera()->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			Camera()->SetScale(Camera()->GetScale() - DT * 100.f);
		else
			Camera()->SetFOV(Camera()->GetFOV() - DT * 100.f);

		// 마우스 휠 값 초기화s
		CKeyMgr::GetInst()->SetMouseWheel(0);
	}
}

void CCameraMoveScript::MoveOrthographic()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += DT * m_camSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= DT * m_camSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= DT * m_camSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += DT * m_camSpeed;
	}

	Transform()->SetRelativePos(vPos);
}

void CCameraMoveScript::MovePerspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT * m_camSpeed * vFront;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos += DT * m_camSpeed * -vFront;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos += DT * m_camSpeed * -vRight;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT * m_camSpeed * vRight;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vDrag = CKeyMgr::GetInst()->GetMouseDrag();
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDrag.x * DT * XM_PI * 3.f;
		vRot.x += vDrag.y * DT * XM_PI * 3.f;
		Transform()->SetRelativeRotation(vRot);
	}
}