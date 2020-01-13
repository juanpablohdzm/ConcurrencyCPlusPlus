#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <stdexcept>
#include <chrono>

void print_result(std::shared_future<int>& fut)
{
	std::cout << fut.get() << " - valid future \n";
}

void main()
{
	std::promise<int> prom;
	std::shared_future<int> fut(prom.get_future());

	std::thread th1(print_result, std::ref(fut));
	std::thread th2(print_result, std::ref(fut));

	prom.set_value(5);

	th1.join();
	th2.join();
}