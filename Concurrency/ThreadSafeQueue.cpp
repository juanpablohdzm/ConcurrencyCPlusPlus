#include <iostream>
#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>
#include <thread>

template<typename T>
class ThreadSafeQueue
{
	std::mutex m;
	std::condition_variable cv;
	std::queue<std::shared_ptr<T>> queue;

public:
	ThreadSafeQueue()
	{

	}

	ThreadSafeQueue(const ThreadSafeQueue& other)
	{

	}

	void Push(T& value)
	{
		std::lock_guard<std::mutex> lg(m);
		queue.push(std::make_shared<T>(value));
		cv.notify_one();
	}

	std::shared_ptr<T> Pop()
	{
		std::lock_guard<std::mutex> lg(m);
		if (queue.empty())
		{
			return std::shared_ptr<T>();
		}
		else
		{
			std::shared_ptr<T> ref(queue.front());
			queue.pop();
			return ref;
		}

	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lg(m);
		return queue.empty();

	}

	std::shared_ptr<T> Wait_Pop()
	{
		std::unique_lock<std::mutex> lg(m);
		cv.wait(lg, [this]() ->bool {return !queue.empty(); });
		std::shared_ptr<T> ref = queue.front();
		queue.pop();
		return ref;
	}

	size_t Size()
	{
		std::lock_guard<std::mutex> lg(m);
		return queue.size();

	}



};