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
	//void Render();

	void AddObject(CGameObject* _object, int _layerIdx, bool _bChildMove = true);
	void AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove = true);

	CLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
	CLayer* GetLayer(const wstring& _strLayerName);

private:
	void Clear();

	friend class CLevelMgr;
};

