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

		case WM_KEYUP:
		{
			// 키가 때어졌다면 input 객체에 이 사실을 전달하여 이 키를 해제하도록 한다.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// 다른 메시지느 사용하지 않으므로 기본 메시지 처리기에 전달.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		break;
	}
}
// MessageHandler 함수는 윈도우의 시스템 메시지가 전달되는 곳입니다.
// 이렇게 함으로서 우리는 관심있는 몇 가지 정보들을 들을 수 있습니다.
// 현재 상태로서는 그냥 키가 눌려있는지, 때어지는지를 알 수 있고, 이를 m_input
// 객체에 전달하고 다른 정보들은 윈도우의 기본 메시지 처리기에 전달합니다.

void SystemClass::InitizlizeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부 포인터를 이 객체로 설정합니다.
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 가져옵니다.
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션의 이름을 설정합니다.
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본 설정으로 맞춥니다.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 클래스를 등록합니다.
	RegisterClassEx(&wc);

	// 모니터 화면의 해상도를 알아옵니다.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 모드 변수의 값에 따라 화면 설정을 합니다.
	if (FULL_SCREEN)
	{
		// 만약 풀스크린모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 합니다.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정을 합니다.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 위치를 화면의 왼쪽 위로 맞춥니다.
		posX = posY = 0;
	}
	else
	{
		// 윈도우 모드라면 800x600의 크기를 가지게 합니다.
		screenWidth = 800;
		screenHeight = 600;

		// 창을 모니터의 중앙에 오도록 합니다.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 설정한 것을 가지고 창을 만들어 그 핸들을 가져옵니다.
	m_hwnd = CreateWindow(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스를 줍니다.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서를 표시하지 않습니다.
	ShowCursor(false);

	return;
}
// InitializeWindows 함수에는 우리가 렌더링을 하게 될 윈도우를 만드는 코드가 들어갑니다.
// 이 함수는 호출함 함수에게로 screenWidth와 screenHeight 값을 다시 되돌려주므로
// 이 값을 어플리케이션에서 활용할 수 있습니다.

void SystemClass::ShutdownWindows()
{
	// 마우스 커서를 표시합니다.
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿉니다.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 제거합니다.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스를 제거합니다.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조를 제거합니다.
	ApplicationHandle = NULL;

	return;
}
// ShutdownWindows는 화면 설정을 되돌리고 윈도우와 그 핸들들을 반환합니다.

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY: // 윈도우가 제거되었는지 확인합니다.
		{
			PostQuitMessage(0);
			return 0;
		}
		 
		case WM_CLOSE: // 윈도우가 닫히는지 확인합니다.
		{
			PostQuitMessage(0);
			return 0;
		}

		// 다른 모든 메시지들은 system 클래스의 메시지 처리기에 전달합니다.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}