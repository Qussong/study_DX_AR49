#pragma once

#include "CEntity.h"

class CConstBuffer :
	public CEntity
{
public:
	CConstBuffer();
	~CConstBuffer();

private:
	D3D11_BUFFER_DESC		m_desc = {};
	ComPtr<ID3D11Buffer>	m_CB;

	UINT	m_elementSize;
	UINT	m_elementCnt;

public:
	int Create(UINT _size, UINT _cnt);
	void SetData(void* _src, UINT _cnt = 0);
	void UpdateData(UINT _registerNum);
};

