#pragma once

#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
	public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE _type);
	~CRenderComponent();

private:
	Ptr<CMesh>      m_mesh;
	Ptr<CMaterial>  m_mtrl;

public:
	void SetMesh(Ptr<CMesh> _mesh) { m_mesh = _mesh; }
	void SetMaterial(Ptr<CMaterial> _mtrl) { m_mtrl = _mtrl; }

	Ptr<CMesh> GetMesh() { return m_mesh; }
	Ptr<CMaterial> GetMaterial() { return m_mtrl; }

public:
	virtual void FinalTick() {};
	virtual void UpdateData() = 0;
	virtual void Render() = 0;
};

