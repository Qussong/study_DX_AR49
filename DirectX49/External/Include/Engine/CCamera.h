#pragma once

#include "CComponent.h"

class CCamera :
	public CComponent
{
public:
	CCamera();
	~CCamera();

private:
	// 원근투영(Perspective)
	float m_fov;			// 시야각

	// 직교투영(Orthographic)
	float m_width;			// 직교투영 가로 길이
	float m_scale;			// 직교투영 배율

	// Both
	float m_aspectRatio;	// 종횡비
	float m_far;			// 투영 최대 거리

	// 변환 행렬
	Matrix m_matView;
	Matrix m_matProj;

};

