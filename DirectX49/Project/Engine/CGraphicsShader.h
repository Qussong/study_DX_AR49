#pragma once
#include "CShader.h"

class CGraphicsShader :
	public CShader
{
private:
	ComPtr<ID3DBlob>	mVSBlob;
	ComPtr<ID3DBlob>	mHSBlob;
	ComPtr<ID3DBlob>	mDSBlob;
	ComPtr<ID3DBlob>	mGSBlob;
	ComPtr<ID3DBlob>	mPSBlob;

	ComPtr<ID3D11VertexShader>		mVS;
	ComPtr<ID3D11HullShader>		mHS;
	ComPtr<ID3D11DomainShader>		mDS;
	ComPtr<ID3D11GeometryShader>	mGS;
	ComPtr<ID3D11PixelShader>		mPS;

	ComPtr<ID3D11InputLayout>		mLayout;

	D3D11_PRIMITIVE_TOPOLOGY		mTopology;

public:
	int CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName);
	int CreateHullShader();
	int CreateDomainShader();
	int CreateGeometryShader();
	int CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName);

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { mTopology = _topology; }

public:
	virtual void UpdateData();

public:
	CGraphicsShader();
	~CGraphicsShader();
};

