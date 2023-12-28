#pragma once

#include "CComponent.h"

class CMesh;
class CMaterial;

class CRenderComponent :
	public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE _type);
	~CRenderComponent();

private:
	CMesh* m_mesh;
	//CGraphicsShader* m_shader;
	CMaterial* m_mtrl;

public:
	void SetMesh(CMesh* _mesh) { m_mesh = _mesh; }
	void SetMaterial(CMaterial* _mtrl) { m_mtrl = _mtrl; }

	CMesh* GetMesh() { return m_mesh; }
	CMaterial* GetMaterial() { return m_mtrl; }

public:
	virtual void FinalTick() {};
	virtual void UpdateData() = 0;
	virtual void Render() = 0;
};

