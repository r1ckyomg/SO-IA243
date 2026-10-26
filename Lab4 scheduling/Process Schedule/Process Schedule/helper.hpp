#pragma once
#include <vector>
#include <cstdint>

class Process
{
public: 

    Process() = default;
    ~Process() = default;
    constexpr Process(std::initializer_list<uint8_t> d) : data(d)
    {
    }

    std::vector<uint8_t> data{};

    constexpr float calcAvgDuration()
    {
        float sum = 0.0;
        for (size_t i = 0; i < data.size(); ++i)
        {
            sum += float(data[i]) * (data.size() - i);

        }
        sum /= data.size();
        return sum;
    }
};
