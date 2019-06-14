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

	while (true)
	{
		std::unique_lock<std::mutex> ReplenishmentOperationAvailableLock(*ReplenishmentOperationMutex);
		cvReplenishOperationAvailable.wait(ReplenishmentOperationAvailableLock, [this] {return !ReplenishRequests.empty(); });

		std::lock_guard<std::mutex> lk(RequestsOperationMutex);
		Request = ReplenishRequests.front();
		ReplenishRequests.pop();
		lk.~lock_guard();

		Request.Replenish(MAX_POPCORN_AMOUNT, MAX_DRINKS_AMOUNT);

		std::lock_guard<std::mutex> ReplenishmentOperationLock(*ReplenishmentOperationMutex);
		ReadyReplenishment->push(ReplenishmentId);
	}
}

void Replenishment::addRequest(ReplenishmentRequest ReplenishRequest)
{
	std::lock_guard<std::mutex> lk(RequestsOperationMutex);
	ReplenishRequests.push(ReplenishRequest);
}
