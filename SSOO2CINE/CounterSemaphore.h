#pragma once
#include <mutex>
#include <shared_mutex>

class CounterSemaphore
{
public:
	
	CounterSemaphore() 
	{
		Count = 0;
		WaitingMutex.lock();
	}

	inline CounterSemaphore(int Count)
	{
		if (Count <= 0)
		{
			WaitingMutex.lock();
		}
		
		this->Count = Count;
	}

	~CounterSemaphore() {};

	inline void notify()
	{
		Mutex.lock();
		if (++Count <= 0)
		{
			Mutex.unlock();
			WaitingMutex.unlock();
		}

		Mutex.unlock();
	}

	inline void wait()
	{
		Mutex.lock();
		if (--Count < 0)
		{
			Mutex.unlock();
			WaitingMutex.lock();
		}
		Mutex.unlock();
	}

	int getCount()
	{
		return Count;
	}

private:
	std::shared_mutex Mutex;
	std::shared_mutex WaitingMutex;
	int Count;
};

