#include <thread>
#include <iostream>
#include <chrono>
#include <list>
#include <numeric>
#include <string>
#include <algorithm>
#include <functional>
#include <mutex>


std::list<int> my_list;
std::mutex m;

void add2(const int& x)
{
	std::lock_guard<std::mutex> lg(m);
	my_list.push_front(x);
}

void add(const int& x)
{
	m.lock();
	my_list.push_front(x);
	m.unlock();
}

void size()
{
	m.lock();
	int size = my_list.size();
	m.unlock();
	std::cout << "size of the list is :" << size << std::endl;
}



class Bank
{
	inline static std::mutex guard;
	int total= 0;
public:
	void deposit(const int& amount)
	{
		guard.lock();
		total += amount;
		guard.unlock();
	}

	void withdraw(const int& amount)
	{
		if (total - amount < 0)
			return;
		guard.lock();
		total -= amount;
		guard.unlock();
	}

	void print() { std::cout << "Total" << total << std::endl; }
};

void main()
{

	std::thread t1([]() 
		{
			for (size_t i = 0; i < 10; i++)
			{
				add2(i);
			}
		});

	std::thread t2([]()
		{
			for (size_t i = 10; i < 20; i++)
			{
				add2(i);
			}
		});

	t1.join();
	std::cout << "Join T1" << std::endl;
	t2.join();
	std::cout << "Join T2" << std::endl;

	for (int item : my_list)
	{
		std::cout << item << std::endl;
	}

	Bank b;

	std::thread t3([](Bank& b)
		{
			for (size_t i = 0; i < 100; i++)
			{
				b.deposit(100);
			}
		},std::ref(b));

	std::thread t4([](Bank& b)
		{
			for (size_t i = 0; i < 100; i++)
			{
				b.withdraw(100);
			}
		}, std::ref(b));

	t3.join();
	t4.join();

	b.print();

	
}