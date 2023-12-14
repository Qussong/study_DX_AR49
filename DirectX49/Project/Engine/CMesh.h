#pragma once
#include "CAsset.h"

class CMesh
	: public CAsset
{
private:
	ComPtr<ID3D11Buffer>	mVB;
	ComPtr<ID3D11Buffer>	mIB;

	D3D11_BUFFER_DESC		mVBDesc;
	D3D11_BUFFER_DESC		mIBDesc;

	UINT					mVtxCount;
	UINT					mIdxCount;

	void*					mVtxSysMem;
	void*					mIdxSysMem;

private:
	void UpdateData();

public:
	int Create(void* _vtx, UINT _vtxCnt, void* _idx, UINT _idxCnt);
	void Render();

public:
	CMesh();
	~CMesh();
};