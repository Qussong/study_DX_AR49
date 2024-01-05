#include "pch.h"
#include "CCollider2D.h"

#include "CTransform.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: Super(COMPONENT_TYPE::COLLIDER2D)
	, m_vOffsetScale{1.f, 1.f, 1.f}
	, m_collisionCount(0)
	, m_bAbsolute(true)
	, m_type(COLLIDER2D_TYPE::RECT)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::FinalTick()
{
	// 충돌체의 로컬 행렬 계산
	//m_matColWorld = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	//m_matColWorld *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	Matrix matObjLocal = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	matObjLocal *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	// 충돌체에 적용시킬 GameObject의 World Matrix
	const Matrix& matObjWorld = Transform()->GetWorldMat();

	// GameObject 의 Scale 에 영향을 받지 않으려는 경우
	if (m_bAbsolute)
	{
		// GameObject의 World Scale Matrix
		Vec3 vWorldScale = Transform()->GetWorldScale();

		// World Scale Matrix의 역행렬
		Matrix matScaleInv = XMMatrixScaling(vWorldScale.x, vWorldScale.y, vWorldScale.z);
		matScaleInv = XMMatrixInverse(nullptr, matScaleInv);

		// Object 의 Scale 의 역행렬을 사이에 배치해서 무효화 시킴
		// 충돌체의 World Matrix = Local Matrix * inv World Scale Matrix * World Matrix
		m_matColWorld = matObjLocal * matScaleInv * matObjWorld;
	}
	else
	{
		//m_matColWorld *= matObjWorld;
		m_matColWorld = matObjLocal * matObjWorld;
	}

	// 충돌체의 World Matrix 에서 Scale, Rotation, Position 분리
	Vec3		v3Scale;		// scale
	Quaternion	quatRotation;	// rotation
	Vec3		v3Positon;		// position
	m_matColWorld.Decompose(v3Scale, quatRotation, v3Positon);

	// 충돌중이면 Red, 충돌하고 있지 않으면 Green
	if (0 == m_collisionCount)
	{
		if (COLLIDER2D_TYPE::RECT == m_type)
		{
			GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(0.f, 1.f, 0.f), false);
		}
		else if (COLLIDER2D_TYPE::CIRCLE == m_type)
		{
			GamePlayStatic::DrawDebugCircle(v3Positon, m_vOffsetScale.x, Vec3(0.f, 1.f, 0.f), false);
		}
	}
	else
	{
		if (COLLIDER2D_TYPE::RECT == m_type)
		{
			GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(1.f, 0.f, 0.f), false);
		}
		else if (COLLIDER2D_TYPE::CIRCLE == m_type)
		{
			GamePlayStatic::DrawDebugCircle(v3Positon, m_vOffsetScale.x, Vec3(1.f, 0.f, 0.f), false);
		}
	}
}

void CCollider2D::BeginOverlap(CCollider2D* _otherCollider)
{
	++m_collisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(this, _otherCollider->GetOwner(), _otherCollider);
	}
}

void CCollider2D::Overlap(CCollider2D* _otherCollider)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->Overlap(this, _otherCollider->GetOwner(), _otherCollider);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _otherCollider)
{
	--m_collisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(this, _otherCollider->GetOwner(), _otherCollider);
	}
}
