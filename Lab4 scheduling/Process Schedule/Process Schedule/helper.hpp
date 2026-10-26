#pragma once
#include <vector>
#include <cstdint>
#include <initializer_list>
#include <string_view>
#include <format>
#include <optional>
#include <numeric>
#include <algorithm>

struct Unit
{
    std::string_view name{};
    uint8_t time{};
    uint8_t priority{};

    constexpr operator uint8_t() const
    {
        return time;
    }

    constexpr auto operator<=>(const Unit& other) const
    {
        if (priority != other.priority)
        {
            return other.priority <=> priority;
        }
        return time <=> other.time;
    }
};

template <>
struct std::formatter<Unit, char>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <class FormatContext>
    auto format(const Unit& u, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}({})", u.name, u.time);
    }
};

class Process
{
public: 

    constexpr float calcAvgProcessTime(const std::vector<Unit>& sep_time)
    {
        return std::accumulate(sep_time.begin(), sep_time.end(), 0.0f) / sep_time.size();
    }
    void sortData()
    {
        std::sort(data.begin(), data.end());
    }

    constexpr float calcAvgExec()
    {
        float sum = 0.0;
        for (size_t i = 0; i < data.size(); ++i)
        {
            sum += float(data[i]) * (data.size() - i);

        }
        sum /= data.size();
        return sum;
    }

    constexpr std::vector<Unit> quantOverhead(bool is_srtf = false)
    {
        //A(10), B(15), C(20), D(25) K=10 -> A(10) B(10) C(10) D(10) B(5) C(10) D(10) D(5)
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data.at(i).time > K)
            {
                Unit rem{ data.at(i).name, static_cast<uint8_t>(data.at(i).time - K) };
                data.at(i).time = K;

                if (is_srtf)
                    data.insert(data.begin() + i + 1, rem);
                else
                    data.emplace_back(rem);
            }
        }
        return data;
    }

    constexpr std::optional<std::pair<float, std::vector<uint8_t>>> calcProcessDellay()
    {
        if (data.empty())
            return std::nullopt;

        uint8_t prev = 0;

        for (auto& num : data)
        {
            prev += num.time;
            process_times.emplace_back(prev);
        }
        return std::pair<float, std::vector<uint8_t>>(calcAvgProcessTime(data), process_times);

    }

    std::vector<Unit> data{};
    std::vector<uint8_t> process_times{};
    uint8_t K{};

    constexpr Process() = default;
    constexpr Process(std::initializer_list<Unit> d) : data(d)
    {}
    constexpr Process(std::initializer_list<Unit> d, uint8_t K) : data(d), K(K) {}
    
    ~Process() = default;
};
