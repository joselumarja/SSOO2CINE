#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "ReplenishmentRequest.h"
#include "Definitions.h"

class Replenishment
{
public:
	Replenishment(int ReplenishmentId, std::queue<int> *ReadyReplenishment, std::mutex *ReplenishmentOperationMutex);
	~Replenishment();
	void operator()();
	void addRequest(ReplenishmentRequest ReplenishRequest);

private:
	int ReplenishmentId;

	std::queue<int> *ReadyReplenishment;
	std::mutex *ReplenishmentOperationMutex;

	std::queue<ReplenishmentRequest> ReplenishRequests;
	std::mutex RequestsOperationMutex;

	std::mutex ReplenishOperationAvailableMutex;
	std::condition_variable cvReplenishOperationAvailable;
};

