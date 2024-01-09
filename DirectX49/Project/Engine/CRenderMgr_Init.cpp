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
	m_light2DBuffer->Create(sizeof(Vec4), 3, SB_TYPE::READ_ONLY, arr);
	m_light2DBuffer->UpdateData(14);

	//
	m_pDebugObj = new CGameObject;
	m_pDebugObj->AddComponent(new CTransform);
	m_pDebugObj->AddComponent(new CMeshRender);
}