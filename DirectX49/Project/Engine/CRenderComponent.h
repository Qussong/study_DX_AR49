#pragma once

#include "CComponent.h"

class CMesh;
class CGraphicsShader;

class CRenderComponent :
	public CComponent
{
public:
	CRenderComponent(COMPONENT_TYPE _type);
	~CRenderComponent();

private:
	CMesh* m_mesh;
	CGraphicsShader* m_shader;

public:
	void SetMesh(CMesh* _mesh) { m_mesh = _mesh; }
	void SetShader(CGraphicsShader* _shader) { m_shader = _shader; }

	CMesh* GetMesh() { return m_mesh; }
	CGraphicsShader* GetShader() { return m_shader; }

public:
	virtual void FinalTick() {};
	virtual void UpdateData() = 0;
	virtual void Render() = 0;
};

