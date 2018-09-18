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
	default:
		break;
	}
}