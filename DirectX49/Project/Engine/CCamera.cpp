#include "pch.h"
#include "CCamera.h"

#include "CTransform.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_fov(0.f)
	, m_width(0.f)
	, m_scale(0.f)
	, m_aspectRatio(1.f)
	, m_far(10000.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::FinalTick()
{
	Vec3 vCamPos = Transform()->GetRelativePos();

	m_matView = XMMatrixIdentity();

	m_matView._41 = -vCamPos.x;
	m_matView._42 = -vCamPos.y;
	m_matView._43 = -vCamPos.z;

	m_matProj = XMMatrixIdentity();

	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
}
