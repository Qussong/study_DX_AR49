#pragma once

class CGameObject;

namespace GamePlayStatic
{
	void SpawnGameObject(CGameObject* _target, int _layerIdx);
	void DestroyGameObject(CGameObject* _target);

	void DrawDebugRect(const Matrix& _worldMat, Vec3 _color, bool _bDepthTest, float _duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _color, bool _bDepthTest, float _duration = 0.f);

	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _color, bool _bDepthTest, float _duration = 0.f);
	
	//void DrawDebugCube();
	//void DrawDebugSphere();
}

template<typename T, UINT SIZE>
void Delete_Array(T* (&Arr)[SIZE])
{
	for (int i = i = 0; i < SIZE; ++i)
	{
		if (nullptr != Arr[i])
		{
			delete Arr[i];
			Arr[i] = nullptr;
		}
	}
}

template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}

	_vec.clear();
}