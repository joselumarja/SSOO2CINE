#include "pch.h"
#include "Replenishment.h"

Replenishment::Replenishment(int ReplenishmentId, std::queue<int> *ReadyReplenishment, std::mutex *ReplenishmentOperationMutex)
{
	this->ReplenishmentId = ReplenishmentId;
	this->ReadyReplenishment = ReadyReplenishment;
	this->ReplenishmentOperationMutex = ReplenishmentOperationMutex;
}

Replenishment::~Replenishment()
{
}

void Replenishment::operator()()
{
	ReplenishmentRequest Request;

	std::mutex ReplenishOperationAvailableMutex;
	std::condition_variable cvReplenishOperationAvailable;

	while (true)
	{
		std::unique_lock<std::mutex> ReplenishmentOperationAvailableLock(*ReplenishmentOperationMutex);
		cvReplenishOperationAvailable.wait(ReplenishmentOperationAvailableLock, [this] {return !ReplenishRequests.empty(); });

		Request = ReplenishRequests.front();
		ReplenishRequests.pop();

		Request.Replenish(MAX_POPCORN_AMOUNT, MAX_DRINKS_AMOUNT);

		std::lock_guard<std::mutex> ReplenishmentOperationLock(*ReplenishmentOperationMutex);
		ReadyReplenishment->push(ReplenishmentId);
	}
}

void Replenishment::addRequest(ReplenishmentRequest ReplenishRequest)
{
	ReplenishRequests.push(ReplenishRequest);
}
