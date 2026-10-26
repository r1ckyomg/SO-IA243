#pragma once
#include "helper.hpp"
#include <utility>
#include <print>
#include <optional>
#include <numeric>
#include <algorithm>
#include <cstdint>
#include <vector> 
#include <map>
#include <string_view>

class SJF : public Process
{
public:

    constexpr float calcAvgProcess(const std::vector<uint8_t>& sep_time)
    {
        return std::accumulate(sep_time.begin(), sep_time.end(), 0.0f) / sep_time.size();
    }

    constexpr std::optional<std::pair<float, std::vector<uint8_t>>> processDellay()
    {
        if (data.empty())
            return std::nullopt;

        process_time.clear();
        uint8_t prev = 0;

        for (auto num : data)
        {
            prev += num;
            process_time.emplace_back(prev);
        }
        std::pair<float, std::vector<uint8_t>> tmp(calcAvgProcess(data), process_time);
        return tmp;
    }

    std::vector<uint8_t> process_time{};
    using Process::Process;
    SJF(std::map<std::string_view, uint8_t> m) : Process(m) {}

};

void calculateSJF(SJF& process)
{
    std::print("{:<29} {}", "SJF order: ", process.data);
    std::sort(process.data.begin(), process.data.end());
    std::println("{:>5} {}", "-> ", process.data);

    if (auto t = process.processDellay(); t.has_value())
    {
        std::println("{:<30}{}\n{:<30}{}",
            "Each process delay:", t.value().second,
            "Avg Duration of each process:", t.value().first);
    }

    std::println("{:<30}{}\n", "Avg Process duration:", process.calcAvgDuration());
}