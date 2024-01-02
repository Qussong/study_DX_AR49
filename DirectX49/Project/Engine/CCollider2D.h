#pragma once
#include "CComponent.h"

enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,

	END,
};

class CCollider2D :
	public CComponent
{
	using Super = CComponent;

public:
	CCollider2D();
	~CCollider2D();

private:
	Vec3 m_vOffsetPos;
	Vec3 m_vOffsetScale;
	int m_collisionCount;	// 다른 충돌체와 충돌중인 횟수
	bool m_bAbsolute;

	Matrix	m_matColWorld;
	COLLIDER2D_TYPE m_type;

public:
	void SetAbsolute(bool _absol) { m_bAbsolute = _absol; }
	void SetOffsetPos(Vec2 _offset) { m_vOffsetPos = Vec3(_offset.x, _offset.y, 0.f); }
	void SetOffsetScale(Vec2 _scale) { m_vOffsetScale = Vec3(_scale.x, _scale.y, 1.f); }
	void SetColliderType(COLLIDER2D_TYPE _type) { m_type = _type; }

	bool IsAbsolute() { return m_bAbsolute; }
	Vec2 GetOffsetPos() { return Vec2(m_vOffsetPos.x, m_vOffsetPos.y); }
	Vec2 GetOffsetScale() { return Vec2(m_vOffsetScale.x, m_vOffsetScale.y); }
	COLLIDER2D_TYPE GetType() { return m_type; }

public:
	virtual void FinalTick() override;

};

