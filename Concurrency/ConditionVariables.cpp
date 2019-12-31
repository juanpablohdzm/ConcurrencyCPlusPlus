#include <thread>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <chrono>


bool IsArrived = false;
int TotalDistance = 10;
int CurrentDistance = 0;
std::condition_variable cv;
std::mutex m;
std::mutex m2;

void KeepMoving()
{
	while (true)
	{
		CurrentDistance++;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		//if (CurrentDistance == TotalDistance) // You can either do the check here or let each thread handle it's own condition to unlock
			cv.notify_all();

		printf("Moving\n");
	}
}

void ConditionVariableCheck()
{
	std::unique_lock<std::mutex> ul(m); // unique_lock can be manually unlocked, unlike counter part lock_guard.
	cv.wait(ul,[]()->bool {return TotalDistance == CurrentDistance; }); // in this case cv is unlocking the ul if the lambda function returns true. If there was no lambda expression as soon as the condition variable was notified it would unlock the unique_lock.
	printf("Finally I am here\n");
}

void SecondCondtionVariableCheck()
{
	std::unique_lock<std::mutex> ul(m2); 
	cv.wait(ul, []()->bool {return TotalDistance == CurrentDistance; }); // If you have multiple events you can't use the same mutex as lock_guard, the thread won't wait until the mutex is released.
	printf("Finally I am here 2\n");

}

std::mutex f;
void IGotFirst(char c)
{
	std::lock_guard<std::mutex> l(f);
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "I got first: " << c << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

void main()
{
	std::thread t(IGotFirst, 't');
	std::thread r(IGotFirst, 'r');

	t.join();
	r.join();


	std::thread driver_thread(KeepMoving);
	std::thread person(ConditionVariableCheck);
	std::thread person2(SecondCondtionVariableCheck);

	driver_thread.join();
	person.join();
	person2.join();
}