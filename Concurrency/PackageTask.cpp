#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <functional>
#include <future>

//The class template std::packaged_task wraps any Callable target so that it can be invoked asynchronously.

int add(int x, int y)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "Add function runs in : " << std::this_thread::get_id() << "\n";
	return x + y;
}

void task_thread()
{
	std::packaged_task<int(int, int)> task1(add);
	std::future<int> f1 = task1.get_future();

	std::thread t1(std::move(task1), 5, 6);
	t1.detach();


	std::cout << "Task thread " << f1.get() << "\n";

}

void task_normal()
{
	std::packaged_task<int(int, int)> task1(add);
	std::future<int> f1 = task1.get_future();
	task1(7, 8);
	std::cout << "task normal -" << f1.get() << "\n";

}


void main()
{
	task_thread();
	task_normal();
	std::cout << "Main thread id : " << std::this_thread::get_id() << "\n";
}


