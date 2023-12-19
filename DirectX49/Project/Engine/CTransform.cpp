#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(1.f, 1.f, 1.f)
	, m_vRealtiveRotation(0.f, 0.f, 0.f)
	, m_vRelativePos(0.f, 0.f, 0.f)
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalTick()
{
	m_matWorld = XMMatrixIdentity();

	//m_matWorld._11 = m_vRelativeScale.x;
	//m_matWorld._22 = m_vRelativeScale.y;
	//m_matWorld._33 = m_vRelativeScale.z;

	//m_matWorld._41 = m_vRelativePos.x;
	//m_matWorld._42 = m_vRelativePos.y;
	//m_matWorld._43 = m_vRelativePos.z;

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRotX = XMMatrixRotationX(m_vRealtiveRotation.x);
	Matrix matRotY = XMMatrixRotationY(m_vRealtiveRotation.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRealtiveRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

	// 물체의 방향값을 다시 계산한다.
	m_arrDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
	m_arrDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	// Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
	// XMVector3TransformCoord	- w 를 1로 확장
	// XMVector3TransformNormal - w 를 0으로 확장
	// mul(float4(_in.vPos, 1 or 0), g_matWorld); 
	// 적용 받을 상태행렬의 이동을 적용할지 말지 결정
	for (int i = 0; i < 3; ++i)
	{
		// m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
		m_arrDir[i] = XMVector3TransformNormal(m_arrDir[i], m_matWorld);
		m_arrDir[i].Normalize();
	}
}

void CTransform::UpdateData()
{
	//tTransform transform = {};
	//transform.matWorld = m_matWorld;
	g_transform.matWorld = m_matWorld;

	// TRANSFORM type 상수버퍼 얻어와서
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	// 위치정보를 Transform 상수버퍼에 보내고
	pCB->SetData(&g_transform);
	// B0 레지스터에 바인딩 해둠
	pCB->UpdateData(0);
}