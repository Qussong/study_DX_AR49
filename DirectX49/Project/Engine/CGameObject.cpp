#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGC.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_renderCom(nullptr)
	, m_parent(nullptr)
	, m_iLayerIdx(-1)	// 어떠한 레벨(레이어)에 소속되어있지 않다.
	, m_bDead(false)
{
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

void CGameObject::Begin()
{
	// 부모객체
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->Begin();
		}
	}
	// 자식객체
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Tick()
{
	// 부모객체
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
	// 자식객체
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	// 부모객체
	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			m_arrCom[i]->FinalTick();
		}
	}

	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterGameObject(this);

	// 자식객체
	vector<CGameObject*>::iterator itr = m_vecChild.begin();
	for (; itr != m_vecChild.end();)
	{
		(*itr)->FinalTick();

		if ((*itr)->m_bDead)
		{
			CGC::GetInst()->Add(*itr);
			itr = m_vecChild.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void CGameObject::Render()
{
	// 부모객체
	if (nullptr != m_renderCom)
	{
		m_renderCom->Render();
	}
	// 자식객체
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
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

void CGameObject::DisconnectWithParent()
{
	vector<CGameObject*>::iterator iter = m_parent->m_vecChild.begin();
	for (; iter != m_parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_parent->m_vecChild.erase(iter);
			m_parent = nullptr;
			return;
		}
	}

	// 부모가 없는 오브젝트에 DisconnectWithParent 함수를 호출 했거나
	// 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
	assert(nullptr);
}

void CGameObject::DisconnectWithLayer()
{
	if (-1 == m_iLayerIdx)
		return;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);
	pCurLayer->DetachGameObject(this);
}

void CGameObject::AddChild(CGameObject* _Child)
{
	if (_Child->m_parent)
	{
		// 이전 부모 오브젝트랑 연결 해제
		_Child->DisconnectWithParent();
	}

	// 부모 자식 연결
	_Child->m_parent = this;
	m_vecChild.push_back(_Child);
}
