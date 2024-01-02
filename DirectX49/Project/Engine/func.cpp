#include "pch.h"

#include "CTaskMgr.h"
#include "CRenderMgr.h"

void GamePlayStatic::SpawnGameObject(CGameObject* _target, int _layerIdx)
{
	FTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_layerIdx;
	task.Param_2 = (DWORD_PTR)_target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _target)
{
	FTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugRect(const Matrix& _worldMat, Vec3 _color, bool _bDepthTest, float _duration)
{
	tDebugShapeInfo info = {};
	{
		info.eShape = DEBUG_SHAPE::RECT;
		info.matWorld = _worldMat;
		info.vColor = _color;
		info.bDepthTest = _bDepthTest;
		info.fDuration = _duration;
	}

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _color, bool _bDepthTest, float _duration)
{
	tDebugShapeInfo info = {};
	{
		info.eShape = DEBUG_SHAPE::RECT;

		info.vWorldPos = _vWorldPos;
		info.vWorldScale = _vWorldScale;
		info.vWorldRot = _vWorldRot;

		info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
			* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
			* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

		info.vColor = _color;
		info.bDepthTest = _bDepthTest;
		info.fDuration = _duration;
	}

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _color, bool _bDepthTest, float _duration)
{
	tDebugShapeInfo info = {};
	info.eShape = DEBUG_SHAPE::CIRCLE;

	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRot = Vec3(0.f, 0.f, 0.f);

	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y)
		* XMMatrixRotationZ(info.vWorldRot.z) * XMMatrixTranslation(info.vWorldPos.x, info.vWorldPos.y, info.vWorldPos.z);

	info.vColor = _color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
