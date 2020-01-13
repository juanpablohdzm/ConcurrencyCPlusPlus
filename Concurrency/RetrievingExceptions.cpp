#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <stdexcept>
#include <chrono>

void calculate_square_root(std::promise<int>& prom)
{
	int x = 1;
	std::cout << "Enter value\n";
	try
	{
		std::cin >> x;
		if (x < 0)
		{
			throw std::logic_error("Negative number");
		}
		prom.set_value(std::sqrt(x));
	}
	catch (std::exception& e)
	{
		prom.set_exception(std::current_exception());
	}
	
}

void print_result(std::future<int>& fut)
{
	try
	{
		int x = fut.get();
		std::cout << "Value: " << x << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "Exception caught " << e.what() << std::endl;
	}
	
}

void main()
{
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();

	std::thread printing_thread(print_result, std::ref(fut));
	std::thread calculation_thread(calculate_square_root, std::ref(prom));

	printing_thread.join();
	calculation_thread.join();
}