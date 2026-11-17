#pragma once
#include "helper.hpp"
#include <fileapi.h>
#include <shellapi.h>
#include <iostream>
#include <fstream>
#include <print>
#include <winreg.h>
#include <minwinbase.h>
#include <Windows.h>
#include <synchapi.h>

inline void restart()
{
	/* 
	2)	Создайте программу, которая добавляет ваше приложение в автозагрузку операционной системы, 
		после перезагружает компьютер :
		• Для Windows : добавьте запись в реестр для автоматического запуска.
	*/
	addToAutorun();
	shutdownSystem(SHUTDOWN_RESTART, 1800);
}

inline void sysLoad()
{
	/*
	* 4) Создайте программу, которая отображает текущую загрузку процессора 
		 и свободное место в оперативной памяти каждые 5 секунд, 
		 а также добавьте ее в автозагрузку.
	*/
	addToAutorun();
	monitorSysLoad();
}

inline void openCMD()
{
	/*
	* 6) Создайте программу, которая запускает несколько системных команд (например, ping и tracert) 
		 одновременно и отображает результат их выполнения, 
		 а также сохраняет результат в файлах.
	*/
	SECURITY_ATTRIBUTES sa{ .nLength = sizeof(SECURITY_ATTRIBUTES), .bInheritHandle = TRUE };
	HANDLE pingFile = CreateFileW(L"ping.txt", GENERIC_WRITE, FILE_SHARE_READ, &sa,
									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	HANDLE tracertFile = CreateFileW(L"tracert.txt", GENERIC_WRITE, FILE_SHARE_READ, 
									&sa, CREATE_ALWAYS, 
									FILE_ATTRIBUTE_NORMAL, nullptr);

	wchar_t pingCmd   [] = L"ping -n 4 8.8.8.8";
	wchar_t tracertCmd[] = L"tracert -d 8.8.8.8";
	HANDLE procs[]{ startProcess(pingCmd, pingFile), startProcess(tracertCmd, tracertFile) };
	WaitForMultipleObjects(2, procs, TRUE, INFINITE);

	for (HANDLE h : { procs[0], procs[1], pingFile, tracertFile })
		CloseHandle(h);

	std::println("/ ping.txt \\");
	std::cout << std::ifstream("ping.txt").rdbuf();
	std::println("\n/ tracert.txt \\");
	std::cout << std::ifstream("tracert.txt").rdbuf();
}

inline void netConf()
{
	/*
	* 
	8) Напишите программу, которая добавляет правило в брандмауэр, 
	   чтобы разрешить входящие соединения на порту 8080. Windows (с помощью netsh) 
	   и реализуйте проверку работоспособности портов и IP-адресов с помощью команд ping, tracert и ipconfig.
	*/

	wchar_t showRule[] = L"netsh advfirewall firewall show rule name=\"Lab5 TCP 8080\"";
	if (runProcess(showRule, nullptr))
	{
		SHELLEXECUTEINFOW addRule{
			.cbSize = sizeof(SHELLEXECUTEINFOW),
			.fMask = SEE_MASK_NOCLOSEPROCESS,
			.lpVerb = L"runas",
			.lpFile = L"netsh",
			.lpParameters = L"advfirewall firewall add rule name=\"Lab5 TCP 8080\" dir=in action=allow protocol=TCP localport=8080",
			.nShow = SW_HIDE
		};
		ShellExecuteExW(&addRule);
		if (addRule.hProcess)
		{
			WaitForSingleObject(addRule.hProcess, INFINITE);
			CloseHandle(addRule.hProcess);
		}
	}

	wchar_t ipconfig[] = L"ipconfig";
	wchar_t ping    [] = L"ping -n 4 8.8.8.8";
	wchar_t tracert [] = L"tracert -d 8.8.8.8";

	for (wchar_t* cmd : { showRule, ipconfig, ping, tracert })
		runProcess(cmd);
}
