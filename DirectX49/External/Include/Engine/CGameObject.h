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
class CCamera;
class CCollider2D;

class CGameObject :
	public CEntity
{
public:
	CGameObject();
	~CGameObject();

private:
	CComponent*				m_arrCom[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*		m_renderCom;
	vector<CScript*>		m_vecScript;
	vector<CGameObject*>	m_vecChild;
	CGameObject*			m_parent;
	int						m_iLayerIdx;	// 객체가 소속되어있는 Layer의 idx
	bool                    m_bDead;

public:
	void Begin();
	void Tick();
	void FinalTick();
	void Render();

public:
	void AddComponent(CComponent* _comp);
	CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }

	// GameObject가 가지고 있는 componenet 객체 반환(Getter)
	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(Camera, CAMERA);
	GET_COMPONENT(Collider2D, COLLIDER2D);

	CGameObject* GetParent() { return m_parent; }
	void DisconnectWithParent();
	void DisconnectWithLayer();
	void AddChild(CGameObject* _Child);

	bool IsDead() { return m_bDead; }

	friend class CLayer;
	friend class CTaskMgr;
};

