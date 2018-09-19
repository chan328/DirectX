/*
	FileName: SystemClass.cpp
*/
#include "systemclass.h"


SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
}
// ��ü���� �����͸� null ������ �ʱ�ȭ�մϴ�. �� ������ �ʱ�ȭ�� �����ϸ�
// shutdown �Լ��� ȣ���ϰ� �Ǵµ� �� �Լ������� ��ü�� null�� �ƴ϶�� 
// �̸� �ùٸ��� ������ ��ü�� ����ϰ� �� ��ü�� �޸𸮸� �����ؾ� �Ѵٰ� �����ϰ�
// ������ ������ �����Ϸ��ϱ� �����Դϴ�.


SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}
// �ƹ��͵� ���� �ʴ� ���� �����ڿ� �ƹ��͵� ���� �ʴ� �ı���.

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// �Լ��� ���̿� �ʺ� �����ϱ� �� ������ 0���� �ʱ�ȭ ����.
	screenWidth = 0;
	screenHeight = 0;

	// WinApi�� ����Ͽ� �ʱ�ȭ�Ѵ�.
	InitizlizeWindows(screenWidth, screenHeight);

	// Input ��ü�� �����մϴ�. �� ��ü�� ������ ���� ������ Ű���� �Է��� �޽��ϴ�.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Input ��ü�� �ʱ�ȭ ���ݴϴ�.
	m_Input->Initialize();

	// Graphics ��ü�� ������ �ݴϴ�. �� ��ü�� ���ø����̼��� ��� �׷��� ��Ҹ� �׷��ݴϴ�.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	// graphics ��ü�� �ʱ�ȭ�մϴ�.
	result = m_Graphics->Initialize(screenWidth, screenHegiht, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}
// Initizlize �Լ������� ���ø����̼��� ��� �ʱ�ȭ �۾��� �����մϴ�.
// ó������ InitializeWindows �Լ��� ȣ���Ͽ� ���ø����̼��� ����� �����츦 �����մϴ�.
// ���� m_Input�� m_Graphics ��ü�� �ʱ�ȭ�Ͽ� ������ �Է��� �޾Ƶ��̰� �׷����� �׸� �� �ְ� �մϴ�.

void SystemClass::Shutdown()
{
	// Graphics ��ü�� ��ȯ�մϴ�.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input ��ü�� ��ȯ�մϴ�.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// â�� �����ŵ�ϴ�.
	ShutdownWindows();

	return;
}
// Shutdown �Լ��� ��ü�� �����մϴ�. �̰��� ����Ǹ鼭 m_Graphics�� m_Input��ü��
// ���õ� ��� �͵��� ��ȯ�ϸ� ������� ���õ� �ڵ鵵 �����մϴ�.

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// �޽��� ����ü�� �ʱ�ȭ�մϴ�.
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ���� �޽����� ���� ������ ������ ���ϴ�.
	done = false;
	while (!done)
	{
		// ������ �޽����� ó���մϴ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ���������ϴ�.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �� �ܿ��� Frame �Լ��� ó���մϴ�.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	
	return;
}
// Run �Լ��� ���α׷��� ����� ������ ������ ���鼭 ���ø����̼��� ��� �۾��� ó���մϴ�.
// ���ø����̼��� ��� �۾��� �� �������� �Ҹ��� Frame �Լ����� �����մϴ�.
// �̰��� ���ø����̼��� �ٸ� �κ��� �ۼ��� ������ �׻� ����ؾ��ϴ� �����Դϴ�.

/*	<�ǻ��ڵ�>
	While ������� ���� ����

	�������� �ý��� �޽����� Ȯ��

	�޽��� ó��
	
	���ø����̼� �۾�

	������ �۾��� ���α׷��� ���Ḧ ���ϴ��� Ȯ��.
*/

bool SystemClass::Frame()
{
	bool result;

	// ������ Esc�� ���� ���ø����̼��� �����ϱ� ���ϴ��� Ȯ���մϴ�.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics ��ü�� �۾��� ó���մϴ�.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}
// Frame �Լ��� ���ø����̼��� ��� �۾����� ó���Ǵ� ���Դϴ�.
// ���ٽ��� EscŰ�� ���� �����ϱ⸦ ���ϴ����� üũ�ϰ� �ֽ��ϴ�.
// �׸��� Graphics ��ü�� ���� ȭ�鿡 �׸��� �۾��� �����մϴ�.

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			// Ű�� ���ȴٸ� input ��ü�� �� ����� �����Ͽ� ����ϵ��� �մϴ�.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		case WM_KEYUP:
		{
			// Ű�� �������ٸ� input ��ü�� �� ����� �����Ͽ� �� Ű�� �����ϵ��� �Ѵ�.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// �ٸ� �޽����� ������� �����Ƿ� �⺻ �޽��� ó���⿡ ����.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
		break;
	}
}
// MessageHandler �Լ��� �������� �ý��� �޽����� ���޵Ǵ� ���Դϴ�.
// �̷��� �����μ� �츮�� �����ִ� �� ���� �������� ���� �� �ֽ��ϴ�.
// ���� ���·μ��� �׳� Ű�� �����ִ���, ������������ �� �� �ְ�, �̸� m_input
// ��ü�� �����ϰ� �ٸ� �������� �������� �⺻ �޽��� ó���⿡ �����մϴ�.

void SystemClass::InitizlizeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ� �����͸� �� ��ü�� �����մϴ�.
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� �����ɴϴ�.
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼��� �̸��� �����մϴ�.
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� ����ϴ�.
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

	// ������ Ŭ������ ����մϴ�.
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� �˾ƿɴϴ�.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ��� ������ ���� ���� ȭ�� ������ �մϴ�.
	if (FULL_SCREEN)
	{
		// ���� Ǯ��ũ������� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� �մϴ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ������ �մϴ�.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ���� ���� ���� ����ϴ�.
		posX = posY = 0;
	}
	else
	{
		// ������ ����� 800x600�� ũ�⸦ ������ �մϴ�.
		screenWidth = 800;
		screenHeight = 600;

		// â�� ������� �߾ӿ� ������ �մϴ�.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �� �ڵ��� �����ɴϴ�.
	m_hwnd = CreateWindow(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ݴϴ�.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ǥ������ �ʽ��ϴ�.
	ShowCursor(false);

	return;
}
// InitializeWindows �Լ����� �츮�� �������� �ϰ� �� �����츦 ����� �ڵ尡 ���ϴ�.
// �� �Լ��� ȣ���� �Լ����Է� screenWidth�� screenHeight ���� �ٽ� �ǵ����ֹǷ�
// �� ���� ���ø����̼ǿ��� Ȱ���� �� �ֽ��ϴ�.

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ���մϴ�.
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲߴϴ�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� �����մϴ�.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ��� �����մϴ�.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ������ �����մϴ�.
	ApplicationHandle = NULL;

	return;
}
// ShutdownWindows�� ȭ�� ������ �ǵ����� ������� �� �ڵ���� ��ȯ�մϴ�.

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY: // �����찡 ���ŵǾ����� Ȯ���մϴ�.
		{
			PostQuitMessage(0);
			return 0;
		}
		 
		case WM_CLOSE: // �����찡 �������� Ȯ���մϴ�.
		{
			PostQuitMessage(0);
			return 0;
		}

		// �ٸ� ��� �޽������� system Ŭ������ �޽��� ó���⿡ �����մϴ�.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}