#include <print>

#include <functional>
#include <vector>
#include "evenTasks.hpp"
#include <utility>
#include <cstdint>
#include <Windows.h>
#include <fileapi.h>
#include <iostream>

enum class Task : uint8_t
{
	TASK1 = 0,
	TASK2,
	TASK3,
	TASK4,
	TASK5,
	TASK6,
	TASK7,
	TASK8,
};

static void funcCal(const Task task)
{
	std::vector<std::function<void()>> funcs{ restart };
	funcs[std::to_underlying(task)]();
}

int main()
{
	using std::println;


	WCHAR data[MAX_PATH] = {}, fSystem[MAX_PATH] = {};
	DWORD serial_num{}, max_val{};
	DWORD f = 0;
	BOOL b = ::GetVolumeInformationW(
		L"С:\\",
		data, MAX_PATH,
		&serial_num, &max_val,
		&f,
		fSystem,
		MAX_PATH);

	std::wcout
		<< L"Bool: " << (b != FALSE)
		<< L", data: " << data
		<< L", serial number: " << serial_num
		<< L", max val: " << max_val
		<< L", fsystem: " << fSystem
		<< std::endl;


  	/*SYSTEM_INFO info;
	::GetNativeSystemInfo(&info);
	println("LP: {}, Page size {}, PMask {}, Min address {}, Max adress {}", 
		info.dwNumberOfProcessors,
		info.dwPageSize, info.dwActiveProcessorMask,
		info.lpMinimumApplicationAddress,
		info.lpMaximumApplicationAddress);*/
}; 

