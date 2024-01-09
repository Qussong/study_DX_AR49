#include "pch.h"
#include "CRenderMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CStructuredBuffer.h"

void CRenderMgr::Init()
{
	// Structured Buffer test
	Vec4 arr[3] =
	{
		Vec4(1.f, 0.f, 0.f, 1.f),
		Vec4(0.f, 1.f, 0.f, 1.f),
		Vec4(0.f, 0.f, 1.f, 1.f)
	};
	m_light2DBuffer = new CStructuredBuffer;
	m_light2DBuffer->Create(sizeof(Vec4), 2, SB_TYPE::READ_ONLY, true);

	if (m_light2DBuffer->GetElementCount() < 3)
	{
		m_light2DBuffer->Create(sizeof(Vec4), 10, SB_TYPE::READ_ONLY, true);
	}
	m_light2DBuffer->SetData(arr, 3);
	m_light2DBuffer->UpdateData(14);

	Vec4 arrTest[3] = {};
	m_light2DBuffer->GetData(arrTest, 3);

	//
	m_pDebugObj = new CGameObject;
	m_pDebugObj->AddComponent(new CTransform);
	m_pDebugObj->AddComponent(new CMeshRender);
}