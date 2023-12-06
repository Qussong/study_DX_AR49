#pragma once

int TestInit();
void TestProgress();
void TestRelease();

void Tick();
void Render();

int CreateVertexBuffer();
int CreateVertexShader();
int CreateInputLayout();
int CreatePixelShader();