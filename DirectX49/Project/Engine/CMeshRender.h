#pragma once

#include "CRenderComponent.h"

class CMeshRender :
	public CRenderComponent
{
public:
	CMeshRender();
	~CMeshRender();

public:
	virtual void UpdateData() override;
	virtual void Render() override;
};

