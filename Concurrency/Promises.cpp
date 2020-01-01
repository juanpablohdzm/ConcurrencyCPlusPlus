#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <stdexcept>
#include <chrono>

void print_int(std::future<int>& f)
{
	std::cout << "Waiting for value from print thread \n";
	int x=  f.get();
	std::cout << "value: " << x << std::endl;
}

void main()
{
	std::promise<int> prom;
	std::future<int> f = prom.get_future();

	std::thread print_thread(print_int, std::ref(f));

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "Setting the value in main thread \n";
	prom.set_value(10);

	print_thread.join();
}