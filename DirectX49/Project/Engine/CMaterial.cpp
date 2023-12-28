#include "pch.h"

#include "CMaterial.h"

#include "CGraphicsShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial()
	: CAsset(ASSET_TYPE::MATERIAL)
	, m_const{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_const);
	pCB->UpdateData();

	if (nullptr != m_pShader)
	{
		m_pShader->UpdateData();
	}
}