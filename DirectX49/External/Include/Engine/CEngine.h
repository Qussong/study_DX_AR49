#pragma once

class CEngine
	: public CSingleton<CEngine>
{
	SINGLE(CEngine);
private:
	HWND	m_hMainWnd;		// ���� Window �ڵ�
	Vec2	m_vResolution;	// �ػ� ����
	
public:
	int init(HWND _hWnd, Vec2 _vResolution);
	void progress();
};

