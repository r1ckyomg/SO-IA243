#pragma once
#include "helper.hpp"
#include <utility>
#include <print>

enum class Algo : uint8_t
{
    FCFS, 
    SJF, 
    SRTF, 
    RR, 
    Priority,
    SPN,
};

template <Algo A>
struct Task : public Process
{
    constexpr Task(std::initializer_list<Unit> d, uint8_t K = 8) : Process(d, K)
    {
    }
};
using FCFS = Task<Algo::FCFS>;
using SJF = Task<Algo::SJF>;
using SRTF = Task<Algo::SRTF>;
using RR = Task<Algo::RR>;
using Priority = Task<Algo::Priority>;
using SPN = Task<Algo::SPN>;


struct User : public Process
{
    uint8_t share{};
    size_t idx{};
    constexpr User(uint8_t share, std::initializer_list<Unit> d, uint8_t K = 8)
        : Process(d, K), share(share)
    {
        quantOverhead();
    }
};

struct FSS : public Process
{
    std::vector<User> users{};
    FSS(std::initializer_list<User> u) : users(u)
    {
    }
    std::vector<Unit> schedule()
    {
        uint8_t min_share = 100;
        for (const auto& u : users)
            min_share = std::min(min_share, u.share);
        data.clear();
        bool active = true;
        while (active)
        {
            active = false;
            for (auto& u : users)
            {
                uint8_t steps = u.share / min_share;
                for (uint8_t s = 0; s < steps && u.idx < u.data.size(); ++s)
                {
                    data.emplace_back(u.data[u.idx++]);
                    active = true;
                }
            }
        }
        return data;
    }
};

template <typename T>
constexpr void calculateAlgo(T&& process)
{
    if constexpr (std::same_as<T, FSS>)
    {
        std::println("Fair-share order: {}\n", process.schedule());
        std::println("{:<32} {:.5}\n", "Avg time for process: ", process.calcAvgExec());
    }
    else if constexpr (std::same_as<T, RR>)
    {
        std::println("New order with quant considered: {}", process.quantOverhead());
        std::println("{:<32} {:.5}\n", "Avg time for process: ", process.calcAvgExec());
    }
    else
    {
        if constexpr (std::same_as<T, SJF> || std::same_as<T, Priority>)
        {
            std::print("{:<29} {:<30}", std::same_as<T, Priority> ? "Priority order: " : "SJF order: ", process.data);
            process.sortData();
            std::println("{:<4} {}", "-> ", process.data);
        }
        else if constexpr (std::same_as<T, FCFS>)
        {
            std::println("{:<29} {}", "FCFS order: ", process.data);
        }
        else if constexpr (std::same_as<T, SRTF> || std::same_as<T, SPN>)
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
}



