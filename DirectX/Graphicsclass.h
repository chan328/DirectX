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
// GraphicsClass는 SystemClass가 생성하는 또 다른 객체입니다.
// 이 어플리케이션은 모든 그래픽 기능이 이 객체 안에 캡슐화 되어있습니다.
// 저는 또한 이 헤더에 풀스크린 모드와 같은 그래픽과 관련된 전역 설정을 할 수 있도록 하였습니다.
// 이 클래스에는 모든 그래픽 객체를 담을 에정..
#endif // !_GRAPHICSCLASS_H_