#pragma once
#include <Windows.h>
#include <winreg.h>
#include <libloaderapi.h>
#include <cstdint>
#include <print>
#include <bit>
#include <iostream>
#include <fstream>

inline void addToAutorun()
{
	wchar_t exepath[MAX_PATH];
	DWORD len = GetModuleFileNameW(nullptr, exepath, MAX_PATH);
	HKEY hKey;
	LONG lnRes = RegOpenKeyExW(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_WRITE,
		&hKey
	);

	if (ERROR_SUCCESS == lnRes)
	{
		RegSetValueExW(
			hKey,
			L"WinAPI lab",
			0,
			REG_SZ,
			reinterpret_cast<const BYTE*>(exepath),
			static_cast<DWORD>((len + 1) * sizeof(wchar_t))
		);
		RegCloseKey(hKey);
	}
}

inline HANDLE startProcess(wchar_t* cmd, HANDLE out)
{
	STARTUPINFOW si{ .cb = sizeof(STARTUPINFOW), .dwFlags = STARTF_USESTDHANDLES, .hStdOutput = out, .hStdError = out };
	PROCESS_INFORMATION pi{};
	CreateProcessW(nullptr, cmd, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
	CloseHandle(pi.hThread);
	return pi.hProcess;
}

inline void restart()
{
	/* 
	*
	2)	Создайте программу, которая добавляет ваше приложение в автозагрузку операционной системы, 
		после перезагружает компьютер :
		• Для Windows : добавьте запись в реестр для автоматического запуска.
	*
	*/
	if (RegGetValueW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		L"WinAPI lab", RRF_RT_REG_SZ, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
		return;

	addToAutorun();

	HANDLE token;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);
	TOKEN_PRIVILEGES tp{ .PrivilegeCount = 1, .Privileges = { { .Attributes = SE_PRIVILEGE_ENABLED } } };
	LookupPrivilegeValueW(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
	AdjustTokenPrivileges(token, FALSE, &tp, 0, nullptr, nullptr);
	CloseHandle(token);

	InitiateShutdownW(nullptr, nullptr, 1800, SHUTDOWN_RESTART, SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_FLAG_PLANNED);
}

inline void sysLoad()
{
	/*
	* 4) Создайте программу, которая отображает текущую загрузку процессора 
		 и свободное место в оперативной памяти каждые 5 секунд, 
		 а также добавьте ее в автозагрузку.
	*/
	addToAutorun();

	FILETIME idle, kernel, user;
	GetSystemTimes(&idle, &kernel, &user);
	uint64_t prevIdle = std::bit_cast<uint64_t>(idle);
	uint64_t prevTotal = std::bit_cast<uint64_t>(kernel) + std::bit_cast<uint64_t>(user);
	MEMORYSTATUSEX mem{ .dwLength = sizeof(MEMORYSTATUSEX) };

	while (true)
	{
		Sleep(5000);
		GetSystemTimes(&idle, &kernel, &user);
		uint64_t curIdle = std::bit_cast<uint64_t>(idle);
		uint64_t curTotal = std::bit_cast<uint64_t>(kernel) + std::bit_cast<uint64_t>(user);
		GlobalMemoryStatusEx(&mem);

		std::println("CPU: {:5.1f}% | Free RAM: {} MB / {} MB",
			100.0 * (1.0 - static_cast<double>(curIdle - prevIdle) / (curTotal - prevTotal)),
			mem.ullAvailPhys / (1024 * 1024),
			mem.ullTotalPhys / (1024 * 1024));

		prevIdle = curIdle;
		prevTotal = curTotal;
	}
}

inline void openCMD()
{
	/*
	* 6) Создайте программу, которая запускает несколько системных команд (например, ping и tracert) 
		 одновременно и отображает результат их выполнения, 
		 а также сохраняет результат в файлах.
	*/
	SECURITY_ATTRIBUTES sa{ .nLength = sizeof(SECURITY_ATTRIBUTES), .bInheritHandle = TRUE };
	HANDLE pingFile = CreateFileW(L"ping.txt", GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	HANDLE tracertFile = CreateFileW(L"tracert.txt", GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	wchar_t pingCmd[] = L"ping -n 4 8.8.8.8";
	wchar_t tracertCmd[] = L"tracert -d 8.8.8.8";
	HANDLE procs[]{ startProcess(pingCmd, pingFile), startProcess(tracertCmd, tracertFile) };
	WaitForMultipleObjects(2, procs, TRUE, INFINITE);

	for (HANDLE h : { procs[0], procs[1], pingFile, tracertFile })
		CloseHandle(h);

	std::println("===== ping.txt =====");
	std::cout << std::ifstream("ping.txt").rdbuf();
	std::println("\n===== tracert.txt =====");
	std::cout << std::ifstream("tracert.txt").rdbuf();
}

inline void netConf()
{
	/*
	* 8) Напишите программу, которая добавляет правило в брандмауэр, 
		 чтобы разрешить входящие соединения на порту 8080. Windows (с помощью netsh) 
		 и реализуйте проверку работоспособности портов и IP-адресов с помощью команд ping, tracert и ipconfig.
	*/
	wchar_t addRule[] = L"netsh advfirewall firewall add rule name=\"Lab5 TCP 8080\" dir=in action=allow protocol=TCP localport=8080";
	wchar_t showRule[] = L"netsh advfirewall firewall show rule name=\"Lab5 TCP 8080\"";
	wchar_t ipconfig[] = L"ipconfig";
	wchar_t ping[] = L"ping -n 4 8.8.8.8";
	wchar_t tracert[] = L"tracert -d 8.8.8.8";

	for (wchar_t* cmd : { addRule, showRule, ipconfig, ping, tracert })
	{
		HANDLE proc = startProcess(cmd, GetStdHandle(STD_OUTPUT_HANDLE));
		WaitForSingleObject(proc, INFINITE);
		CloseHandle(proc);
	}
}
