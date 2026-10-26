#pragma once
#include "helper.hpp"
#include <utility>
#include <print>

struct SJF: public Process
{
    using Process::Process;
};

struct FCFS: public SJF
{
    using SJF::SJF;
};


struct SRTF: public Process
{
    using Process::Process;
};

struct RR: public Process
{
    using Process::Process;
};

template <typename T>
void calculateSJF(T& process)
{
    if constexpr (std::same_as<T, SJF>)
    {
        std::print("{:<29} {:<30}", "SJF order: ", process.data);
        process.sortData();
        std::println("{:<4} {}", "-> ", process.data);
    }

    else if (std::same_as<T, FCFS>)
    {
        std::println("{:<29} {}", "FCFS order: ", process.data);
    }

    if constexpr (std::same_as<T, SRTF>)
    {
        std::print("{:<29} {:<30}", "SRTF order: ", process.data);
        process.sortData();
        process.quantOverhead(true);
        std::println("{:<4} {}", "-> ", process.data);
    }

    if (auto t = process.calcProcessDellay(); t.has_value())
    {
        std::println("{:<30}{}\n{:<30}{:.4}",
            "Each process delay:", t.value().second,
            "Avg Duration of each process:", t.value().first);
    }
    std::println("{:<30}{:.5}\n", "Avg Process duration:", process.calcAvgExec());

    
}

void calculateRR(RR& process)
{
    std::println("New order with quant considered: {}", process.quantOverhead());
    std::println("{:<32} {:.5}\n", "Avg time for process: ", process.calcAvgExec());
}


