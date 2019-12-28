#include <thread>
#include <iostream>
#include <chrono>

void foo()
{
	printf("Foo");
}

void bar()
{

	printf("Bar");
}

void main()
{

	std::thread t1(foo);

	std::thread t2 = std::move(t1);

	t1 = std::thread(bar); //Implicit move operation, in other cases we have seen that this is the same as copy constructor. 
	//Remember that in this case t1 is null because we used move and passed it to t2, otherwise we would make thread t1 an orphan causing an exception. 

	t1.join();
	t2.join();
}