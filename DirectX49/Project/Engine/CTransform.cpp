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
	//UpdateData();
}

void CTransform::UpdateData()
{
	tTransform transform = {};
	transform.vWorldPos = m_vRelativePos;
	transform.vWorldScale = m_vRelativeScale;

	// TRANSFORM type 상수버퍼 얻어와서
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	// 위치정보를 Transform 상수버퍼에 보내고
	pCB->SetData(&transform);
	// B0 레지스터에 바인딩 해둠
	pCB->UpdateData(0);
}
