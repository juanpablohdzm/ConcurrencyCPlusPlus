#include <thread>
#include <chrono>
#include <iostream>

void foo(int x, int y)
{
	printf("X - %d, Y - %d \n", x, y);
}

void foo2(int& x, int& y)
{
	while (true)
	{
		x ++;
		printf("X - %d, Y - %d \n", x, y);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void function2(int& x)
{
	while (true)
	{
		try
		{
			printf("X - %d", x);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		catch (const std::exception&)
		{

		}
	}
}


void function1()
{
	int x = 3;
	std::thread t(function2, std::ref(x));
	t.detach(); 
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	//To this point the thread created in this function will cause a memory access violation because X has been destroyed, but function2 is still trying to access it
}

void main()
{
	int x = 9;
	std::cout << "Main thread value of X \n" << x << std::endl;

	//std::thread t1(foo, 1, 2);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "After t1\n" << x << std::endl;
	//t1.join();

	//std::thread t2(foo2, std::ref(x), std::ref(x)); //We need to explicitly say we want this variable to pass by reference. 
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "Main thread value of X \n" << x << std::endl;
	x = 7;
	std::cout << "Main thread value of X \n" << x << std::endl;
	//t2.join();

	std::thread t3(function1);
	t3.join();


}