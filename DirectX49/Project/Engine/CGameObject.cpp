#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

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

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
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

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// Script 타입 Component 가 실제로 Script 클래스가 아닌 경우
		assert(dynamic_cast<CScript*>(_comp));

		m_vecScript.push_back((CScript*)_comp);
		_comp->m_owner = this;
	}
	else
	{
		// 이미 해당 타입의 컴포넌트를 보유하고 있는 경우 
		assert(!m_arrCom[(UINT)type]);

		m_arrCom[(UINT)type] = _comp;
		_comp->m_owner = this;

		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_comp);
		if (nullptr != pRenderCom)
		{
			// 이미 한 종류 이상의 RenderComponent 를 보유하고 있는 경우
			assert(!m_renderCom);

			m_renderCom = pRenderCom;
		}
	}
}
