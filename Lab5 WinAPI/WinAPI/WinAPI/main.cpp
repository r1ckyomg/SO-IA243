#include "evenTasks.hpp"
#include "oddTasks.hpp"
#include <array>
#include <utility>
#include <cstdint>


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
	constexpr std::array tasks{ runTask1, restart, runTask3, sysLoad,
							runTask5, openCMD, runTask7, netConf };
	
	tasks[std::to_underlying(task)]();
}

//#define CPULOG

int main()
{
	for (uint8_t i = 0; i <= std::to_underlying(Task::TASK8); ++i)
	{
#ifndef CPULOG 
		if (i == std::to_underlying(Task::TASK5) || i == std::to_underlying(Task::TASK4))
			continue;
#endif
		funcCal(static_cast<Task>(i));
	}
};
