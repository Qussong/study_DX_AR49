#pragma once

#include "CComponent.h"

enum class PROJ_TYPE
{
	ORTHOGRAPHIC, // 직교 투영
	PERSPECTIVE,  // 원근 투영
};

class CCamera :
	public CComponent
{
public:
	CCamera();
	~CCamera();

private:
	PROJ_TYPE	m_projType;

	// 원근투영(Perspective)
	float		m_fov;			// 시야각(Filed Of View)

	// 직교투영(Orthographic)
	float		m_width;		// 직교투영 가로 길이
	float		m_scale;		// 직교투영 배율

	// Both
	float		m_aspectRatio;	// 종횡비
	float		m_far;			// 투영 최대 거리

	// 변환 행렬
	Matrix		m_matView;
	Matrix		m_matProj;

public:
	PROJ_TYPE GetProjType() { return m_projType; }
	void SetProjType(PROJ_TYPE _type) { m_projType = _type; }

	float GetScale() { return m_scale; }
	void SetScale(float _scale) { m_scale = _scale; }

	float GetFOV() { return m_fov; }
	void SetFOV(float _fov) { m_fov = _fov; }

public:
	virtual void FinalTick() override;
};

