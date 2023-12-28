#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMaterial.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::UpdateData()
{
	if (nullptr != GetMaterial())
	{
		GetMaterial()->UpdateData();
	}

	GetOwner()->Transform()->UpdateData();
}

void CMeshRender::Render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	UpdateData();

	GetMesh()->Render();
}
