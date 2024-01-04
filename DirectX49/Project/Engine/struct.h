#pragma once

// 3차원 공간에 배치되는 정점
struct Vtx
{
	Vec3 vPos;		// 정점의 좌표
	Vec4 vColor;	// 정점의 색상 정보
	Vec2 vUV;		// UV좌표계 or Texture Coodinate
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE	eShape;

	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRot;
	Matrix		matWorld;

	Vec3		vColor;
	float		fLifeTime;
	float		fDuration;
	bool		bDepthTest;
};

// ==================
// 상수버퍼 대응 구조체
// ==================
struct tTransform
{
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	Matrix	matWV;
	Matrix	matWVP;
};

extern tTransform g_transform;

// ==================
// 재질 대응 구조체
// ==================
struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int	bTex[TEX_PARAM::END];

	int iPadding[2];
};

struct tAnimData2D
{
	Vec2 vLeftTop;
	Vec2 vSlizeSize;
	Vec2 vOffset;
	int  useAnim2D;
	int	 iPadding;
};