#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel
	: public CEntity
{
public:
	CLevel();
	~CLevel();

private:
	CLayer* m_arrLayer[LAYER_MAX];

public:
	void Begin();
	void Tick();
	void FinalTick();
	void Render();

	void AddObject(CGameObject* _object, int _layerIdx);
};

