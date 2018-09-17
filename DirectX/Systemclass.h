#pragma once
/*
	systemclass.h
*/

#ifndef  _SYSTEMCLASS_H_ 
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN // Win32~를 define으로 설정해 자주 사용되지 않는 API를 담고 있는 WIn32
// 헤더를 포함하지 않음으로 빌드 속도를 높이기 위해서입니다.

#include <Windows.h> // 를 선언해야 우리가 윈도우를 생성, 제거하는 함수들을 호출할 수 있다.

#include "inputclass.h" // SystemClass에서 사용하기 위한 프레임워크의 다른 두 클래스의 헤더를 포함한다.
#include "graphicsclass.h"

class SystemClass 
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitizlizeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass m_Graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif // ! _SYSTEMCLASS_H_