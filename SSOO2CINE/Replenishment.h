#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "ReplenishmentRequest.h"
#include "Definitions.h"

class Replenishment
{
public:
	Replenishment(int ReplenishmentId, std::condition_variable *cvReplenishmentTurn, std::condition_variable *cvReplenishmentResponse, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentRequestQueueMutex, std::mutex *PrintMutex);
	~Replenishment();
	void operator()();

private:
	int ReplenishmentId;

	std::condition_variable *cvReplenishmentTurn;
	std::condition_variable *cvReplenishmentResponse;

	std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue;
	std::mutex *ReplenishmentRequestQueueMutex;

	std::mutex *PrintMutex;
};

