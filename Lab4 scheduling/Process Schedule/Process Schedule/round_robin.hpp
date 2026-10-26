#pragma once
#include "helper.hpp"
#include <print>

class RR : public Process
{
public:
	using Process::Process;
	uint8_t K{};

	RR(std::initializer_list<uint8_t> d, uint8_t K) : Process(d), K(K)
	{
	} 

	constexpr std::vector<uint8_t> quantOverhead()
	{ 
		//A(10), B(15), C(20), D(25) K=10 -> A(10) B(10) C(10) D(10) B(5) C(10) D(10) D(5)

		for (size_t i = 0; i< data.size(); ++i)
		{
			/*uint8_t num = data.at(i);*/
			if (data.at(i) > K)
			{
				data.emplace_back(data.at(i) - K);
				data.at(i) = K;

			}
		}
		return data;
	}
};

void calculateRR(RR& process)
{
	std::println("New order with quant considered: {}", process.quantOverhead());
	std::println("{:<32} {:.5}", "Avg time for process: ", process.calcAvgDuration());

}
