/*
	FileName: Main.cpp 
*/
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pscmdline, int icmdshow)
{
	// WinMain 함수는 SystemClass를 생성한 뒤 초기화합니다. 만약 아무 문제 없이 초기화되면 Run 함수를 호출합니다.
	SystemClass* System;
	bool result;

	// System 객체를 생성한다.
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// system 객체를 초기화하고 run을 호출한다.
	result = System->Initialize();
	if (result)
	{
		System->Run(); // Run 함수는 내부적으로 루프를 가지고 있으며 종료될 때까지 모든 어플리케이션 코드를 실행합니다.
	}

	System->Shutdown(); // Run 함수가 끝나면 Shutdown 함수를 호출한 뒤
	delete System; // Systemclass 객체를 정리합니다.
	System = 0;

	return 0;
}