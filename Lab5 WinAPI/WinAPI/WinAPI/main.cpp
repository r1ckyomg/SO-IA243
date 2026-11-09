#include <print>
#include <Windows.h>
#include <winreg.h>
#include <libloaderapi.h>

#include <tchar.h>
#include <cstdlib>

int main()
{
	TCHAR exepath[MAX_PATH];
	DWORD len = GetModuleFileName(0, exepath, MAX_PATH);
	HKEY hKey;
	LONG lnRes = RegOpenKeyEx(
		HKEY_CURRENT_USER,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, KEY_WRITE,
		&hKey
	);

	if (ERROR_SUCCESS == lnRes)
	{
		lnRes = RegSetValueEx(
			hKey,
			L"WinAPI lab",
			0,
			REG_SZ,
			reinterpret_cast<const BYTE*>(exepath),
			(len + 1) * sizeof(TCHAR)
		);
		RegCloseKey(hKey);
	}
	std::println("{}", lnRes == ERROR_SUCCESS ? 0 : 1);
	std::system("shutdown /r /t 3500");

	/*
	2) Создайте программу, которая добавляет ваше приложение в автозагрузку операционной системы, а затем перезагружает компьютер:
	• Для Windows: добавьте запись в реестр для автоматического запуска. 
	*/

	/*
	Создайте программу, которая отображает текущую загрузку процессора и свободное место в оперативной памяти каждые 5 секунд, а также добавьте ее в автозагрузку. 
	*/
	/*
	Создайте программу, которая запускает несколько системных команд (например, ping и tracert) одновременно и отображает результат их выполнения, а также сохраняет результат в файлах.
	*/
	
	
	/*
	* 8) Напишите программу, которая добавляет правило в брандмауэр, чтобы разрешить входящие соединения на порту 8080. Windows (с помощью netsh) и реализуйте проверку работоспособности портов и IP-адресов с помощью команд ping, tracert и ipconfig.
	*/
}; 

