#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_renderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->Begin();
		}
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->Tick();
		}
	}
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->FinalTick();
		}
	}
}

void CGameObject::Render()
{
	if (nullptr != m_renderCom)
	{
		m_renderCom->Render();
	}
}

void CGameObject::AddComponent(CComponent* _comp)
{
	COMPONENT_TYPE type = _comp->GetType();

	m_arrCom[(UINT)type] = _comp;
	_comp->m_owner = this;

	m_renderCom = dynamic_cast<CRenderComponent*>(_comp);
}
