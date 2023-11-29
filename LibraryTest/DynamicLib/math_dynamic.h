#pragma once

#ifdef DYNAMICLIB_EXPORTS
#define MY_DLL extern "C" __declspec(dllexport)
#else
#define MY_DLL extern "C" __declspec(dllimport)
#endif

MY_DLL int Pow(int a, int b);