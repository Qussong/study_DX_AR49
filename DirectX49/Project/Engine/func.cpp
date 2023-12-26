#include "pch.h"

#include "CTaskMgr.h"

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
}
