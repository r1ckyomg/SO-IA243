#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

using namespace std;

inline void runTask1() {
    const char* fileName = "restart_note.txt";

    ofstream(fileName, ios::app).close();

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = {};
    string cmd = string("notepad.exe ") + fileName;

    if (!CreateProcessA(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cerr << "Ошибка запуска Notepad\n";
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    cout << "Код выхода: " << exitCode << "\n";

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    char fullPath[MAX_PATH];
    GetFullPathNameA(fileName, MAX_PATH, fullPath, NULL);
    string regCmd = string("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce /v OpenCreatedNote /d \"notepad.exe \\\"") + fullPath + "\\\"\" /f >nul";
    system(regCmd.c_str());

    system("shutdown /r /t 60");
}

inline void runTask3() {
    cout << "Выберите действие (1 - Перезагрузка, 2 - Выключение): ";
    int choice = 1;
    cin >> choice;

    string desktop = string(getenv("USERPROFILE")) + "\\Desktop\\power_log.txt";
    ofstream(desktop, ios::app)
        << (choice == 1 ? "Перезагрузка ОС\n" : "Выключение ОС\n");

    system(choice == 1 ? "shutdown /r /t 0" : "shutdown /s /t 0");
}

inline ULONGLONG ftToU64(const FILETIME& ft) {
    return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

inline void runTask5() {
    FILETIME prevIdle, prevKernel, prevUser;
    GetSystemTimes(&prevIdle, &prevKernel, &prevUser);

    cout << "Мониторинг CPU и RAM\n";

    while (true) {
        Sleep(5000);

        FILETIME idle, kernel, user;
        GetSystemTimes(&idle, &kernel, &user);

        ULONGLONG idleDiff = ftToU64(idle) - ftToU64(prevIdle);
        ULONGLONG totalDiff = (ftToU64(kernel) - ftToU64(prevKernel)) +
                              (ftToU64(user) - ftToU64(prevUser));
        double cpuLoad = totalDiff ? (1.0 - static_cast<double>(idleDiff) / totalDiff) * 100.0 : 0.0;

        prevIdle = idle;
        prevKernel = kernel;
        prevUser = user;

        MEMORYSTATUSEX mem = { sizeof(mem) };
        GlobalMemoryStatusEx(&mem);
        ULONGLONG freeRamMB = mem.ullAvailPhys / (1024 * 1024);

        cout << fixed << setprecision(1)
             << "Загрузка CPU: " << cpuLoad << "% | "
             << "Свободно RAM: " << freeRamMB << " МБ\n";
    }
}

inline void runTask7() {
    FILE* pipe = _popen("wevtutil qe System /c:5 /rd:true /f:text", "r");
    if (!pipe) return;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        cout << buffer;
    }
    _pclose(pipe);

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    string cmd = string("schtasks /Create /SC MINUTE /MO 5 /TN \"SysLogReaderTask\" /TR \"\\\"")
                + exePath + "\\\"\" /F";
    cout << "\nЗадача в планировщик Windows\n";
    system(cmd.c_str());
}
