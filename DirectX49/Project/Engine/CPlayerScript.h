#pragma once

#include "CScript.h"

class CPlayerScript :
	public CScript
{
public:
	CPlayerScript();
	~CPlayerScript();

private:
	float       m_speed;

public:
	virtual void Tick() override;

	virtual void BeginOverlap(CCollider2D* _collider, CGameObject* _otherObj, CCollider2D* _otherCollider) override;
	virtual void Overlap(CCollider2D* _collider, CGameObject* _otherObj, CCollider2D* _otherCollider) override;
	virtual void EndOverlap(CCollider2D* _collider, CGameObject* _otherObj, CCollider2D* _otherCollider) override;
};

