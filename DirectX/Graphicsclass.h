#pragma once
/*
	Filename: graphicsclass.h
*/
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"

const bool FULL_SCREEN = false;
const bool YSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D;
};
// GraphicsClass�� SystemClass�� �����ϴ� �� �ٸ� ��ü�Դϴ�.
// �� ���ø����̼��� ��� �׷��� ����� �� ��ü �ȿ� ĸ��ȭ �Ǿ��ֽ��ϴ�.
// ���� ���� �� ����� Ǯ��ũ�� ���� ���� �׷��Ȱ� ���õ� ���� ������ �� �� �ֵ��� �Ͽ����ϴ�.
// �� Ŭ�������� ��� �׷��� ��ü�� ���� ����..
#endif // !_GRAPHICSCLASS_H_