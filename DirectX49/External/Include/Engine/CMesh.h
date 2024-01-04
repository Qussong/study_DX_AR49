#pragma once
#include "CAsset.h"

class CMesh
	: public CAsset
{
private:
	ComPtr<ID3D11Buffer>	m_VB;
	ComPtr<ID3D11Buffer>	m_IB;

	D3D11_BUFFER_DESC		m_VBDesc;
	D3D11_BUFFER_DESC		m_IBDesc;

	UINT					m_VtxCount;
	UINT					m_IdxCount;

	void*					m_VtxSysMem;
	void*					m_IdxSysMem;

private:
	void UpdateData();

public:
	int Create(void* _vtx, UINT _vtxCnt, void* _idx, UINT _idxCnt);
	void Render();

public:
	CMesh();
	~CMesh();
};