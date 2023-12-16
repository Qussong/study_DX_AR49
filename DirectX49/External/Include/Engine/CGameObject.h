#pragma once
#include "CEntity.h"

#define GET_COMPONENT(Type, TYPE)\
		class C##Type* Type()\
		{\
			return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::##TYPE];\
		}

class CComponent;
class CRenderComponent;
class CScript;

class CGameObject :
	public CEntity
{
public:
	CGameObject();
	~CGameObject();

private:
	CComponent*			m_arrCom[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*	m_renderCom;
	vector<CScript*>    m_vecScript;

public:
	void Begin();
	void Tick();
	void FinalTick();
	void Render();

public:
	void AddComponent(CComponent* _comp);
	CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
};

