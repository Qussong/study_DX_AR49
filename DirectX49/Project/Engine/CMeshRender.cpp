#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
	if (nullptr != GetShader())
	{
		GetShader()->UpdateData();
	}

	GetOwner()->Transform()->UpdateData();
}

void CMeshRender::Render()
{
	if (nullptr == GetMesh() || nullptr == GetShader())
		return;

	UpdateData();

	// 이미지 선택 상수버퍼
	{
		ImgNum imgNum;
		imgNum.num = Vec4(0.f, 0.f, 0.f ,(float)(GetOwner()->GetImgNum()));

		// GLOBAL type 상수버퍼 얻어와서
		CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
		// 위치정보를 IMGNUM 상수버퍼에 보내고
		pCB->SetData(&imgNum);
		// B1 레지스터에 바인딩 해둠
		pCB->UpdateData(1);
	}

	GetMesh()->Render();
}
