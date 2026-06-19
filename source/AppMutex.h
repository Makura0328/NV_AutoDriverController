// アプリケーションの多重起動を防ぐ
#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

class AppMutex
{
public:
	AppMutex()
	{
		m_mutex = CreateMutexW(nullptr, TRUE, L"NV AutoDriverController");
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_isAlreadyRunning = true;
		}
	}

	bool IsAlreadyRunning() const
	{
		return m_isAlreadyRunning;
	}

	~AppMutex()
	{
		if (m_mutex)
		{
			CloseHandle(m_mutex);
		}
	}

private:
	HANDLE m_mutex;
	bool m_isAlreadyRunning = false;
};