#pragma once

// DirectX 11 ���� GPU ����
class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
private:

public:
	int init(HWND _hWnd, Vec2 _vResolution);
};

