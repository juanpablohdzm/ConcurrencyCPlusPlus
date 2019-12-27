#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	printf("Hello from foo\n");
}

void bar()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	printf("Hello from bar\n");
}

void run()
{
	std::thread foo_thread(foo);
	std::thread bar_thread(bar);

	bar_thread.detach(); //Detach will not block the calling thread but will avoid using std::thread_terminate which causes exceptions
	printf("Hello from run\n");

	foo_thread.join();

	printf("Hello from run2\n");

}

void other_operation()
{
	printf("Hello from other operation\n");
	throw std::runtime_error("This is a runtime error");

}

void runWithExceptions()
{
	std::thread foo_thread(foo);
	try
	{
		other_operation();
		foo_thread.join();
	}
	catch (const std::exception&)
	{
		foo_thread.join();
	}
}




class thread_guard
{
	std::thread& t;
public:
	explicit thread_guard(std::thread& _t) : t(_t) {};

	~thread_guard()
	{
		if (t.joinable())
		{
			t.join();
		}
	}

	thread_guard(thread_guard& const) = delete;
	thread_guard& operator= (thread_guard& const) = delete;

};

void runWithGuardExpections()
{
	std::thread foo_thread(foo);
	thread_guard guard(foo_thread);
	try
	{
		other_operation();
	}
	catch (const std::exception&)
	{

	}
}


void main()
{
	//run();
	//runWithExceptions();
	runWithGuardExpections();
}

