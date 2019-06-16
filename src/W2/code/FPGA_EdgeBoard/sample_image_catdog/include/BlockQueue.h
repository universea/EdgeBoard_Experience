#pragma once
#include <mutex>
#include <condition_variable>
#include <deque>
#include <iostream>
using namespace std;

template <typename T>
class BlockingQueue
{
private:

	BlockingQueue(const BlockingQueue& rhs);
	BlockingQueue& operator =(const BlockingQueue& rhs);
	mutable mutex _mutex;
	condition_variable _condvar;
	deque<T> _queue;
	bool isShutDown = false;
public:
	BlockingQueue()
		: _mutex()
		  , _condvar()
		  , _queue()
	{
	}
	~BlockingQueue()
	{
		cout << "blocking queue release" << endl;
	}
	void ShutDown()
	{
		
		isShutDown = true;
		_condvar.notify_all();
		_queue.clear();
	}
	bool IsShutDown()
	{
		
		return isShutDown;
	}
	void Put(const T task)
	{
		lock_guard<mutex> lock(_mutex);
		if (!isShutDown)
		{
			{
				_queue.push_back(task);
			}
			_condvar.notify_all();
		}
	}

	T Take()
	{
		unique_lock<mutex> lock(_mutex);
		if(_queue.size()<=0)
		_condvar.wait(lock);
		if (isShutDown||_queue.empty())
		{
			throw exception();
		}
		T front(_queue.front());
		_queue.pop_front();
		return front;
	}

	size_t Size() const
	{
		lock_guard<mutex> lock(_mutex);
		return _queue.size();
	}
};
