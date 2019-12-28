#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <functional>


void sequential_accumulate_test()
{
	std::vector<int> v{ 1,2,3,4,5,6,7,8,9,10 };
	int sum = std::accumulate(v.begin(), v.end(), 0);
	int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

	auto dash_fold = [](std::string a, int b)
	{
		return std::move(a) + "-" + std::to_string(b);
	};

	std::string s = std::accumulate(std::next(v.begin()), v.end(), std::to_string(v[0]), dash_fold);

	std::cout << "Sum - " << sum << std::endl;
	std::cout << "Product - " << product << std::endl;
	std::cout << "Dash fold - " << s << std::endl;
}

#define  MIN_BLOCK_SIZE 1000

template<typename iterator, typename T>
void accumulate(iterator start, iterator end, T& ref)
{
	ref = std::accumulate(start, end, 0);
}

template<typename iterator, typename T>
T parrallel_accumulate(iterator start, iterator end, T& ref)
{
	int input_size = std::distance(start, end);
	int allowed_threads_by_elements = (input_size) / MIN_BLOCK_SIZE;

	int allowed_threads_by_hardware = std::thread::hardware_concurrency();

	int num_threads = std::min(allowed_threads_by_elements, allowed_threads_by_hardware);

	int block_size = (input_size + 1) / num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);

	iterator last;

	for (size_t i= 0; i< num_threads - 1; i++)
	{
		last = start;
		std::advance(last, block_size);
		threads[i] = std::thread(accumulate<iterator, T>, start, last, std::ref(results[i]));

		start = last;
	}

	results[num_threads - 1] = std::accumulate(start, end, 0);

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(), results.end(), ref);
}

void main()
{
	//sequential_accumulate_test();

	const int size = 8000;
	int*  my_array = new int[size];
	int ref = 0;

	srand(0);

	for (size_t i = 0; i < size; i++)
	{
		my_array[i] = rand() % 10;
	}

	int rer_val = parrallel_accumulate<int*, int>(my_array, my_array + size, ref);
	std::cout << "Accumulated value : " << rer_val << std::endl;
}