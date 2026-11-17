#pragma once
#include <Windows.h>
#include <winreg.h>
#include <libloaderapi.h>
#include <securitybaseapi.h>
#include <reason.h>
#include <sysinfoapi.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <handleapi.h>
#include <cstdint>
#include <print>
#include <processenv.h>

inline void setAutorun(const wchar_t* subKey, const wchar_t* name, const wchar_t* value)
{
	HKEY hKey;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, subKey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		RegSetValueExW(
			hKey,
			name,
			0,
			REG_SZ,
			reinterpret_cast<const BYTE*>(value),
			static_cast<DWORD>((wcslen(value) + 1) * sizeof(wchar_t))
		);
		RegCloseKey(hKey);
	}
}

inline void addToAutorun()
{
	wchar_t exepath[MAX_PATH];
	GetModuleFileNameW(nullptr, exepath, MAX_PATH);
	setAutorun(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", L"WinAPI lab", exepath);
}

inline HANDLE startProcess(wchar_t* cmd, HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE))
{
	STARTUPINFOW si{ .cb = sizeof(STARTUPINFOW), .dwFlags = STARTF_USESTDHANDLES, .hStdOutput = out, .hStdError = out };
	PROCESS_INFORMATION pi{};
	if (!CreateProcessW(nullptr, cmd, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi))
		return nullptr;
	CloseHandle(pi.hThread);
	return pi.hProcess;
}

inline DWORD runProcess(wchar_t* cmd, HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE))
{
	HANDLE proc = startProcess(cmd, out);
	if (!proc)
		return static_cast<DWORD>(-1);
	WaitForSingleObject(proc, INFINITE);
	DWORD exitCode = 0;
	GetExitCodeProcess(proc, &exitCode);
	CloseHandle(proc);
	return exitCode;
}

inline void shutdownSystem(DWORD flags, DWORD timeoutSec)
{
	HANDLE token;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
	{
		TOKEN_PRIVILEGES tp{ .PrivilegeCount = 1, .Privileges = { { .Attributes = SE_PRIVILEGE_ENABLED } } };
		LookupPrivilegeValueW(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
		AdjustTokenPrivileges(token, FALSE, &tp, 0, nullptr, nullptr);
		CloseHandle(token);
	}

	InitiateShutdownW(nullptr, nullptr, timeoutSec, flags,
					  SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_FLAG_PLANNED);
}

inline ULONGLONG ftToU64(const FILETIME& ft)
{
	ULARGE_INTEGER u{ .LowPart = ft.dwLowDateTime, .HighPart = ft.dwHighDateTime };
	return u.QuadPart;
}

inline void monitorSysLoad()
{
	FILETIME idle, kernel, user;
	GetSystemTimes(&idle, &kernel, &user);
	uint64_t prevIdle = ftToU64(idle);
	uint64_t prevTotal = ftToU64(kernel) + ftToU64(user);
	MEMORYSTATUSEX mem{ .dwLength = sizeof(MEMORYSTATUSEX) };

	while (true)
	{
		Sleep(5000);
		GetSystemTimes(&idle, &kernel, &user);
		uint64_t curIdle = ftToU64(idle);
		uint64_t curTotal = ftToU64(kernel) + ftToU64(user);
		GlobalMemoryStatusEx(&mem);

		std::println("CPU: {:5.1f}% | Free RAM: {} MB / {} MB",
			100.0 * (1.0 - static_cast<double>(curIdle - prevIdle) / (curTotal - prevTotal)),
			mem.ullAvailPhys / (1024 * 1024),
			mem.ullTotalPhys / (1024 * 1024));

		prevIdle = curIdle;
		prevTotal = curTotal;
	}
}
