#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <functional>
#include <future>


void printing(std::string message)
{
	std::cout << message << std::this_thread::get_id() << std::endl;
}

int addition(int x, int y)
{
	printing("Addition runs on: ");
	return x + y;
}

int substract(int x, int y)
{
	printing("Subtract runs on: ");
	return x - y;
}

void SimpleExampleOfFuture()
{
	std::cout << "Thread ID :" << std::this_thread::get_id() << std::endl;

	std::future<void> f = std::async(std::launch::deferred, []() {std::cout << "Thread ID :" << std::this_thread::get_id() << std::endl;  }); // Launch::deferred will only be called when future.get is called, also it will run on the same thread. 
	f.get();

	std::future<void> a = std::async(std::launch::async, []() {std::cout << "Thread ID :" << std::this_thread::get_id() << std::endl;  }); // Launch::async will run on a different thread. 

	std::future<void> f1 = std::async(std::launch::async, printing, "Main function: ");
	std::future<int> f2 = std::async(std::launch::deferred, addition, 1, 2);
	std::future<int> f3 = std::async(std::launch::deferred | std::launch::async, substract, 1, 2); // The compiler will decide which one to use.

	std::cout << "value from add: " << f2.get() << std::endl;
	std::cout << "value from substract: " << f3.get() << std::endl;
}


int MIN_ELEMENT_COUNT = 1000;

template<typename iterator>
int parallel_accumulate(iterator begin, iterator end)
{
	long length = std::distance(begin, end);

	//At least runs 1000 elements
	if (length <= MIN_ELEMENT_COUNT)
	{
		std::cout << std::this_thread::get_id() << std::endl;
		return std::accumulate(begin, end, 0);
	}

	iterator mid = begin;
	std::advance(mid, (length + 1) / 2);

	//Recursive all to parallel_accumulate
	std::future<int> f1 = std::async(std::launch::deferred | std::launch::async, parallel_accumulate<iterator>, mid, end);
	int sum = parallel_accumulate<iterator>(begin, mid);

	return sum + f1.get();
}

void ParallelAccumulateExample()
{
	const int size = 8000;
	int* my_array = new int[size];

	for (size_t i = 0; i < size; i++)
	{
		my_array[i] = i;
	}

	int rer_val = parallel_accumulate<int*>(my_array, my_array + size);
	std::cout << "Accumulated value : " << rer_val << std::endl;
	std::cout << "Accumulated value (normal) :" << std::accumulate(my_array, my_array + size, 0);

}




void main()
{
	//SimpleExampleOfFuture();
	ParallelAccumulateExample();
}