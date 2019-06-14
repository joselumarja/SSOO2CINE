#pragma once
#include <mutex>
#include <shared_mutex>

class CounterSemaphore
{
public:
	
	CounterSemaphore() 
	{
		Count = 0;
	}

	inline CounterSemaphore(int Count)
	{
		if (Count <= 0)
		{
			WaitingMutex.lock();
		}
		
		this->Count = Count;
	}

	inline void notify()
	{
		Mutex.lock();
		if (++Count <= 0)
		{
			Mutex.unlock();
			WaitingMutex.unlock();
			Mutex.lock();
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
			Mutex.lock();
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

