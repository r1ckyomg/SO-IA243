#include "evenTasks.hpp"
#include <print>

#include <functional>
#include <vector>
#include <utility>
#include <cstdint>
#include <Windows.h>
#include <fileapi.h>
#include <iostream>
#include <VersionHelpers.h>

enum class Task : uint8_t
{
	TASK2 = 0,
	TASK4,
	TASK6,
	TASK8,
};

static void funcCal(const Task task)
{
	std::vector<std::function<void()>> funcs{ restart, sysLoad, openCMD, netConf };
	funcs[std::to_underlying(task)]();
}


int main()
{
	using std::println;
	funcCal(Task::TASK2);

	

}; 

