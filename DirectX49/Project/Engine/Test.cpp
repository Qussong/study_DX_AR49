#include "pch.h"
#include "Test.h"

#include "CDevice.h"

// �ﰢ���� ���� ���� (�ﰢ���� �������� = 3��)
Vtx g_vtx[3] = {};
// �簢���� ���� ���� (�簢���� �������� = 4��)
Vtx g_vtx2[4] = {};

ComPtr<ID3D11Buffer>		g_VB = nullptr;	// ��������
ComPtr<ID3D11VertexShader>	g_VS = nullptr;	// ���ؽ� ���̴�
ComPtr<ID3D11PixelShader>	g_PS = nullptr;	// �ȼ� ���̴�


int TestInit()
{
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
		MessageBox(nullptr, L"���ؽ� ���� ���� ����", L"TextInit ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

int CreateVertexBuffer()
{
	// ���ؽ� ���� ���� ����ü
	D3D11_BUFFER_DESC bufferDesc = {};
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

void TestProgress()
{
	// Window ���� ����
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// �ﰢ�� �׸���
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);
	//CONTEXT->Draw(3, 0);

	// �޸� �� �۾��� �ﰢ�� �׸��� Window �� �׷��ش�.
	CDevice::GetInst()->Present();
}

int TestInit2()
{
	g_vtx2[0].vPos = Vec3(0.5f, 0.5f, 0.f);
	g_vtx2[0].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[0].vUV = Vec2(0.f, 0.f);
		 
	g_vtx2[1].vPos = Vec3(0.5f, -0.5f, 0.f);
	g_vtx2[1].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[1].vUV = Vec2(0.f, 0.f);
		 
	g_vtx2[2].vPos = Vec3(-0.5f, -0.5f, 0.f);
	g_vtx2[2].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[2].vUV = Vec2(0.f, 0.f);

	g_vtx2[3].vPos = Vec3(-0.5f, 0.5f, 0.f);
	g_vtx2[3].vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	g_vtx2[3].vUV = Vec2(0.f, 0.f);

	// �������� ���� & �ʱ�ȭ
	if (FAILED(CreateVertexBuffer2()))
	{
		MessageBox(nullptr, L"���ؽ� ���� ���� ����", L"TextInit ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

int CreateVertexBuffer2()
{
	// ���ؽ� ���� ���� ����ü
	D3D11_BUFFER_DESC bufferDesc = {};
	{
		bufferDesc.ByteWidth = sizeof(Vtx) * 4;				// ������ ũ��
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

void TestProgress2()
{
	// Window ���� ����
	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(clearColor);

	// �ﰢ�� �׸���
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->VSSetShader(g_VS.Get(), 0, 0);
	CONTEXT->PSSetShader(g_PS.Get(), 0, 0);
	CONTEXT->Draw(4, 0);

	// �޸� �� �۾��� �簢�� �׸��� Window �� �׷��ش�.
	CDevice::GetInst()->Present();
}

void TestRelease()
{

}