#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

#include "Replenishment.h"
#include "ReplenishmentRequest.h"

class ReplenishmentScheduler
{
public:
	ReplenishmentScheduler(std::vector<Replenishment> *ReplenishmentVector, std::queue<int> *ReadyReplenishment, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentOperationMutex, std::mutex *ReplenishmentRequestQueueMutex);
	~ReplenishmentScheduler();
	void operator()();

private:
	std::queue<int> *ReadyReplenishment;
	std::vector<Replenishment> *ReplenishmentVector;
	std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue;

	std::mutex *ReplenishmentOperationMutex;
	std::mutex *ReplenishmentRequestQueueMutex;

	std::condition_variable cvAvailableReplenishmentOperation;
	std::mutex AvailableReplenishmentOperationMutex;
};

