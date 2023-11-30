#pragma once

// DirectX 11 기준 GPU 제어
class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
private:

public:
	int init(HWND _hWnd, Vec2 _vResolution);
};

