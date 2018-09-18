/*
	FileName: SystemClass.cpp
*/
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}
// 객체들의 포인터를 null 값으로 초기화합니다. 이 이유는 초기화에 실패하면
// shutdown 함수를 호출하게 되는데 이 함수에서는 개체가 null이 아니라면 
// 이를 올바르게 생성된 객체로 취급하고 이 객체의 메모리를 정리해야 한다고 생각하고
// 실제로 정리를 수행하려하기 때문입니다.


SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}
// 아무것도 하지 않는 복사 생성자와 아무것도 하지 않는 파괴자.

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// 함수에 높이와 너비를 전달하기 전 변수를 0으로 초기화 해줌.
	screenWidth = 0;
	screenHeight = 0;

	// WinApi를 사용하여 초기화한다.
	InitizlizeWindows(screenWidth, screenHeight);

	// Input 객체를 생성합니다. 이 객체는 유저로 부터 들어오는 키보드 입력을 받습니다.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input 객체를 초기화 해줍니다.
	m_Input->Initialize();

	// Graphics 객체를 생성해 줍니다. 이 객체는 어플리케이션의 모든 그래픽 요소를 그려줍니다.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// graphics 객체를 초기화합니다.
	result = m_Graphics->Initialize(screenWidth, screenHegiht, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}
// Initizlize 함수에서는 어플리케이션의 모든 초기화 작업을 수행합니다.
// 처음으로 InitializeWindows 함수를 호출하여 어플리케이션이 사용할 윈도우를 생성합니다.
// 또한 m_Input과 m_Graphics 객체를 초기화하여 유저의 입력을 받아들이고 그래픽을 그릴 수 있게 합니다.

void SystemClass::Shutdown()
{
	// Graphics 객체를 반환합니다.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input 객체를 반환합니다.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// 창을 종료시킵니다.
	ShutdownWindows();

	return;
}
// Shutdown 함수는 객체를 정리합니다. 이것은 종료되면서 m_Graphics와 m_Input객체와
// 관련된 모든 것들을 반환하며 윈도우와 관련된 핸들도 정리합니다.

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// 메시지 구조체를 초기화합니다.
	ZeroMemory(&msg, sizeof(MSG));

	// 유저로부터 종료 메시지를 받을 때까지 루프를 돕니다.
	done = false;
	while (!done)
	{
		// 윈도우 메시지를 처리합니다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나갑니다.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 그 외에는 Frame 함수를 처리합니다.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	
	return;
}
// Run 함수는 프로그램이 종료될 때까지 루프를 돌면서 어플리케이션의 모든 작업을 처리합니다.
// 어플리케이션의 모든 작업은 매 루프마다 불리는 Frame 함수에서 수행합니다.
// 이것은 어플리케이션의 다른 부분을 작성할 때에도 항상 기억해야하는 개념입니다.

/*	<의사코드>
	While 종료되지 않은 동안

	윈도우의 시스템 메시지를 확인

	메시지 처리
	
	어플리케이션 작업

	유저가 작업중 프로그램의 종료를 원하는지 확인.
*/

bool SystemClass::Frame()
{
	bool result;

	// 유저가 Esc를 눌러 어플리케이션을 종료하기 원하는지 확인합니다.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics 객체의 작업을 처리합니다.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}
// Frame 함수는 어플리케이션의 모든 작업들이 처리되는 곳입니다.
// 보다시피 Esc키를 눌러 종료하기를 원하는지를 체크하고 있습니다.
// 그리고 Graphics 객체를 통해 화면에 그리는 작업을 수행합니다.

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		// 키가 눌렸다면 input 객체에 이 사실을 전달하여 기록하도록 합니다.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}
	default:
		break;
	}
}