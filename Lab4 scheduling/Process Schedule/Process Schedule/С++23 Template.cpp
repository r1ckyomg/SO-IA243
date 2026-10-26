#include <print>
#include "sjf.hpp"
#include "round_robin.hpp"
#include <concepts>
#include <map>
#include <string_view>
#include <vector>
#include <cstdint>

template<typename T>
void calc (T&& process) 
{
    if constexpr (std::same_as <T, SJF>)
    {
        calculateSJF(process);
    }
    if constexpr (std::same_as<T, RR>)
    {
        calculateRR(process);
    }
};

int main()
{
    using std::println;
    /*
        2. Алгоритм «Самый короткий процесс первым» (Shortest Job First – SJF)
        **Входные данные**: A(7), B(2), C(4), D(1)
        **Входные данные**: P1(5), P2(3), P3(1), P4(2)
        .
        **Входные данные**: X(6), Y(4), Z(3), W(2)
        **Входные данные**: T1(8), T2(3), T3(6), T4(1)
        **Входные данные**: M(9), N(5), O(2), P(4)
    *
    */

    std::println("SJF\n{}", std::string(50, '='));
    //calc(SJF{ 7,2,4,1 });
    calc(SJF{ 5,3,1,2 });
    calc(SJF{ 6,4,3,2 });
    calc(SJF{ 8,3,6,1 });
    calc(SJF{ 9,5,2,4 });

    //SJF({ "A", {7} }, { "B", {2} }, { "C", {4} });

    calc(SJF({ {"A", 7}, {"B", 2}, {"C", 4}, {"D", 1} }));

    /*
    *
        4. Алгоритм «Round Robin» (Round Robin – RR)
        **Входные данные**: A(10), B(15), C(20), D(25). K = 10
        **Входные данные**: P1(12), P2(8), P3(16), P4(6). K = 7
        **Входные данные**: X(9), Y(14), Z(11), W(13). K = 5
        **Входные данные**: T1(20), T2(12), T3(9), T4(16). K = 8
        **Входные данные**: M(18), N(22), O(13), P(17). K = 9
    *
    */

    std::map<std::string_view, std::vector<uint8_t>> data{ {"A", {10}}, {"B", {15}}, {"C", {20}}, {"D", {25}} };
    
    std::println("\nRound-Robin\n{}", std::string(50, '='));
    calc(RR{ { 10,15,20,25 },  10 });
    calc(RR{ { 12,8,16,6   },   7 });
    calc(RR{ { 9,14,11,13  },   5 });
    calc(RR{ { 20,12,9,16  },   8 });
    calc(RR{ { 18,22,13,17 },   9 });

};