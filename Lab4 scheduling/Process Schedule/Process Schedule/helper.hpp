#pragma once
#include <vector>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <string_view>

class Process
{
public: 

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

    constexpr float calcAvg()
    {
        return 0;
    }


    std::vector<uint8_t> data{};
    std::map<std::string_view, uint8_t> map_data{};

    constexpr Process() = default;
    ~Process() = default;
    Process(std::initializer_list<uint8_t> d) : data(d)
    {}

    Process(std::map<std::string_view, uint8_t> m) : map_data(m) {}

};
