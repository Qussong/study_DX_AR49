#pragma once

#include <stdlib.h>
#include <Windows.h>

// DirectX 11 ��� �� ���̺귯�� ����
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// Engine ��� ����
#include "singleton.h"
#include "define.h"
#include "struct.h"