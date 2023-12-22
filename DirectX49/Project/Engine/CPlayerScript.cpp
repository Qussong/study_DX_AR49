#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
	: m_speed(1000.f)
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
}
