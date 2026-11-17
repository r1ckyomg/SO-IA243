#pragma once
#include "helper.hpp"
#include <fileapi.h>
#include <processenv.h>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <Windows.h>

inline void runTask1()
{
	std::ofstream(L"restart_note.txt", std::ios::app).close();

	wchar_t cmd[] = L"notepad.exe restart_note.txt";
	DWORD exitCode = runProcess(cmd);
	std::println("Код выхода: {}", exitCode);

	wchar_t fullPath[MAX_PATH];
	GetFullPathNameW(L"restart_note.txt", MAX_PATH, fullPath, nullptr);
	std::wstring noteCmd = std::format(L"notepad.exe \"{}\"", fullPath);
	setAutorun(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce", L"OpenCreatedNote", noteCmd.c_str());

	shutdownSystem(SHUTDOWN_RESTART, 180);
}

inline void runTask3()
{
	std::print("Выберите действие (1 - Перезагрузка, 2 - Выключение): ");
	int choice = 1;
	std::cin >> choice;

	wchar_t profile[MAX_PATH]{};
	GetEnvironmentVariableW(L"USERPROFILE", profile, MAX_PATH);
	std::wstring desktop = std::format(L"{}\\Desktop\\power_log.txt", profile);
	std::ofstream(desktop, std::ios::app)
		<< (choice == 1 ? "Перезагрузка ОС\n" : "Выключение ОС\n");

	shutdownSystem(choice == 1 ? SHUTDOWN_RESTART : SHUTDOWN_POWEROFF, 180);
}

inline void runTask5()
{
	monitorSysLoad();
}

inline void runTask7()
{
	wchar_t wevtutilCmd[] = L"wevtutil qe System /c:5 /rd:true /f:text";
	runProcess(wevtutilCmd);

	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH);
	std::wstring cmd = std::format(
		L"schtasks /Create /SC MINUTE /MO 5 /TN \"SysLogReaderTask\" /TR \"\\\"{}\\\"\" /F",
		exePath);
	std::println("\nЗадача в планировщик Windows");
	runProcess(cmd.data());
}
