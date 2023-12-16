#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type)	C##Type* Type()\
									{\
										return m_owner->Type();\
									}

class CComponent :
	public CEntity
{
public:
	CComponent(COMPONENT_TYPE _type);
	~CComponent();

private:
	const COMPONENT_TYPE m_type;
	CGameObject* m_owner;

public:
	virtual void Begin() {};
	virtual void Tick() {};
	virtual void FinalTick() = 0;
	virtual void UpdateData() {};

public:
	COMPONENT_TYPE GetType() { return m_type; }
	CGameObject* GetOwner() { return m_owner; }

	GET_OTHER_COMPONENT(Transform);
	GET_OTHER_COMPONENT(MeshRender);
	
	friend class CGameObject;
};

