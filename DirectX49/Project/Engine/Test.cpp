#include "pch.h"
#include "Test.h"

#include "CDevice.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

// �ﰢ���� ���� ���� (�ﰢ���� �������� = 3��)
Vtx g_vtx[3] = {};
// �簢���� ���� ���� (�簢���� �������� = 4��)
Vtx g_vtx2[4] = {};

ComPtr<ID3D11Buffer>		g_VB			= nullptr;	// ��������
ComPtr<ID3DBlob>			g_VSBlob		= nullptr;	// VS ������ ���� ����	
ComPtr<ID3D11VertexShader>	g_VS			= nullptr;	// ���ؽ� ���̴�
D3D11_INPUT_ELEMENT_DESC	g_arrElement[3]	= {};		// �ﰢ�� ���� ��������(Layout)
ComPtr<ID3D11InputLayout>	g_Layout		= nullptr;	// ���� �ϳ��� ������ �˸��� ��ü
ComPtr<ID3DBlob>			g_PSBlob		= nullptr;	// PS ������ ���� ����
ComPtr<ID3D11PixelShader>	g_PS			= nullptr;	// �ȼ� ���̴�
ComPtr<ID3DBlob>			g_ErrBlob		= nullptr;


int TestInit()
{
	// [�ﰢ�� ��ġ ����]
	//       0(Red)
	//      /    \
	//    2(G) - 1(Blue)
	g_vtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	g_vtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[0].vUV = Vec2(0.f, 0.f);

	g_vtx[1].vPos = Vec3(1.f, -1.f, 0.f);
	g_vtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[1].vUV = Vec2(0.f, 0.f);

	g_vtx[2].vPos = Vec3(-1.f, -1.f, 0.f);
	g_vtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	g_vtx[2].vUV = Vec2(0.f, 0.f);

	// �������� ���� & �ʱ�ȭ
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(nullptr, L"Vertex Buffer ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateVertexShader()))
	{
		MessageBox(nullptr, L"Vertex Shader ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateInputLayout()))
	{
		MessageBox(nullptr, L"Input Layout ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreatePixelShader()))
	{
		MessageBox(nullptr, L"Pixel Shader ���� ����", L"TestInit ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

int CreateVertexBuffer()
{
	// ���ؽ� ���� ���� ����ü
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	{
		bufferDesc.ByteWidth = sizeof(Vtx) * 3;				// ������ ũ��
		bufferDesc.StructureByteStride = sizeof(Vtx);		// ���� �ϳ��� ũ��
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �뵵���� = ���ؽ� ����
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	}

	// ���ؽ� ���ۿ� �� �������� �ʱ� �� ����
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = g_vtx;

	// Vertex Buffer
	HRESULT hr = DEVICE->CreateBuffer(&bufferDesc, &tSubData, g_VB.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CreateVertexShader()
{
	HRESULT hr = NULL;

	// shader ���� ���
	wstring strFilePath = CPathMgr::GetContentPath();

	// HLSL ���ؽ� ���̴� �Լ� ������
	hr = D3DCompileFromFile(
		wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Std2D", "vs_5_0"
		, D3DCOMPILE_DEBUG
		, 0
		, g_VSBlob.GetAddressOf()
		, g_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Vertex Shader Compile Failed!!", MB_OK);
		}
		return E_FAIL;
	}

	hr = DEVICE->CreateVertexShader(
		g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, nullptr
		, g_VS.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CreateInputLayout()
{
	// ���� ��������
	{
		g_arrElement[0].InputSlot = 0;
		g_arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		g_arrElement[0].SemanticName = "POSITION";
		g_arrElement[0].SemanticIndex = 0;
		g_arrElement[0].InstanceDataStepRate = 0;
		g_arrElement[0].AlignedByteOffset = 0;
		g_arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		
		g_arrElement[1].InputSlot = 0;
		g_arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		g_arrElement[1].SemanticName = "COLOR";
		g_arrElement[1].SemanticIndex = 0;
		g_arrElement[1].InstanceDataStepRate = 0;
		g_arrElement[1].AlignedByteOffset = 12;
		g_arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		
		g_arrElement[2].InputSlot = 0;
		g_arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		g_arrElement[2].SemanticName = "TEXCOORD";
		g_arrElement[2].SemanticIndex = 0;
		g_arrElement[2].InstanceDataStepRate = 0;
		g_arrElement[2].AlignedByteOffset = 28;
		g_arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	}

	// Layout ����
	HRESULT hr = DEVICE->CreateInputLayout(g_arrElement, 3
		, g_VSBlob->GetBufferPointer()
		, g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

int CreatePixelShader()
{
	HRESULT hr = NULL;

	// shader ���� ���
	wstring strFilePath = CPathMgr::GetContentPath();

	// HLSL ���ؽ� ���̴� �Լ� ������
	hr = D3DCompileFromFile(
		wstring(strFilePath + L"shader\\std2d.fx").c_str()
		, nullptr
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Std2D", "ps_5_0"
		, D3DCOMPILE_DEBUG
		, 0
		, g_PSBlob.GetAddressOf()
		, g_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != g_ErrBlob)
		{
			char* pErrMsg = (char*)g_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Pixel Shader Compile Failed!!", MB_OK);
		}

		return E_FAIL;
	}

	hr = DEVICE->CreatePixelShader(
		g_PSBlob->GetBufferPointer()
		, g_PSBlob->GetBufferSize()
		, nullptr
		, g_PS.GetAddressOf());
	CHECK(hr);

	return S_OK;
}

void TestProgress()
{
	// Window ���� ����
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// �ﰢ�� �׸���
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CONTEXT->IASetInputLayout(g_Layout.Get());

	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);

	CONTEXT->Draw(3, 0);

	// �޸� �� �۾��� �ﰢ�� �׸��� Window �� �׷��ش�.
	CDevice::GetInst()->Present();
}

void TestRelease()
{

}