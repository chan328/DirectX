#pragma once
/*
	systemclass.h
*/

#ifndef  _SYSTEMCLASS_H_ 
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN // Win32~�� define���� ������ ���� ������ �ʴ� API�� ��� �ִ� WIn32
// ����� �������� �������� ���� �ӵ��� ���̱� ���ؼ��Դϴ�.

#include <Windows.h> // �� �����ؾ� �츮�� �����츦 ����, �����ϴ� �Լ����� ȣ���� �� �ִ�.

#include "inputclass.h" // SystemClass���� ����ϱ� ���� �����ӿ�ũ�� �ٸ� �� Ŭ������ ����� �����Ѵ�.
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