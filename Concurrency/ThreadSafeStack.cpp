#include <thread>
#include <iostream>
#include <stack>
#include <mutex>
#include <memory>

using namespace std;

template<typename T>
class thread_safe_stack
{
	stack<T> stk;
	mutex m;

public:
	void push(T element)
	{
		lock_guard<mutex> lg(m);
		stk.push(make_shared<T>(element));
	}

	shared_ptr<T> pop()
	{
		lock_guard<mutex> lg(m);
		if (stk.empty())
		{
			throw runtime_error("stack is empty");
		}

		shared_ptr<T> res(stk.top());
		stk.pop();
		return res;
	}

	void pop(T& value)
	{
		lock_guard<mutex> lg(m);
		if (stk.empty()) throw runtime_error("stack is empty");

		value = *(stk.top().get());
		stk.pop();

	}

	bool empty()
	{
		lock_guard<mutex> lg(m);
		return stk.empty();
	}

	size_t size()
	{
		lock_guard<mutex> lg(m);
		return stk.size();
	}
};