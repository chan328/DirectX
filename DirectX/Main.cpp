/*
	FileName: Main.cpp 
*/
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pscmdline, int icmdshow)
{
	// WinMain �Լ��� SystemClass�� ������ �� �ʱ�ȭ�մϴ�. ���� �ƹ� ���� ���� �ʱ�ȭ�Ǹ� Run �Լ��� ȣ���մϴ�.
	SystemClass* System;
	bool result;

	// System ��ü�� �����Ѵ�.
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// system ��ü�� �ʱ�ȭ�ϰ� run�� ȣ���Ѵ�.
	result = System->Initialize();
	if (result)
	{
		System->Run(); // Run �Լ��� ���������� ������ ������ ������ ����� ������ ��� ���ø����̼� �ڵ带 �����մϴ�.
	}

	System->Shutdown(); // Run �Լ��� ������ Shutdown �Լ��� ȣ���� ��
	delete System; // Systemclass ��ü�� �����մϴ�.
	System = 0;

	return 0;
}