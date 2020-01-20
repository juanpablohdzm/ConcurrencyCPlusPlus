#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <stdexcept>
#include <chrono>
#include <list>
#include <algorithm>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{

	//recursive  condition
	if (input.size < 2)
	{
		return input;
	}

	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T pivot = *result.begin();

	std::list<T>::iterator divide_point = std::partition(input.begin(), input.end()), [&](T const& value)->bool 
	{
		return value < pivot;
	});

	std::list<T> lower_list;
	lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

	std::list<T> new_lower(sequential_quick_sort(std::move(lower_list)));
	std::list<T> new_upper(sequential_quick_sort(std::move(input)));

	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_upper);

	return result;
}

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{

	//recursive  condition
	if (input.size < 2)
	{
		return input;
	}

	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T pivot = *result.begin();

	std::list<T>::iterator divide_point = std::partition(input.begin(), input.end()), [&](T const& value)->bool
	{
		return value < pivot;
	});

	std::list<T> lower_list;
	lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

	std::list<T> new_lower(sequential_quick_sort(std::move(lower_list)));
	
	std::future<std::list<T>> new_upper_future(std::async(std::launch::async | std::launch::deferred, parallel_quick_sort<T>, std::move(input)));

	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_upper_future.get());

	return result;
}