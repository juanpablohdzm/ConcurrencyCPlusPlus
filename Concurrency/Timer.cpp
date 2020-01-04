#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

template<typename Func> 
class Timer
{
private:
	Timer();

	int seconds;

	std::thread t;
	std::condition_variable cv;

	std::promise<void> promise;
	std::future<void> f;

	Func callback;

public:
	Timer(int seconds) : seconds(seconds) 
	{
		f = promise.get_future();
	};

	std::condition_variable GetConditionVariable() const { return cv; }

	void Start()
	{
		t = std::thread([&]()
			{
				int timeout = seconds;
				while (f.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout && timeout > 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					timeout--;
					printf("Second %d \n",timeout);
				}				
				callback();
			});
	}

	void Stop()
	{
		promise.set_value();
	}

	void SetFunction(Func& function) { this->function = function; }

	~Timer()
	{
		t.join();
	}


};


void main()
{

	Timer<std::function<void(void)>> timer(5);
	timer.SetFunction([]() {std::printf("Hello from function\n"); });
	timer.Start();

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	Timer<std::function<void(void)>> timer2(5);
	timer2.SetFunction([]() {std::printf("Hello from function\n"); });
	timer2.Start();
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	timer2.Stop();

	getchar();
}