#include "pch.h"
#include "ReplenishmentScheduler.h"

ReplenishmentScheduler::ReplenishmentScheduler(std::vector<Replenishment>* ReplenishmentVector, std::queue<int>* ReadyReplenishment, std::queue<ReplenishmentRequest>* ReplenishmentRequestQueue, std::mutex * ReplenishmentOperationMutex, std::mutex * ReplenishmentRequestQueueMutex)
{
	this->ReplenishmentVector = ReplenishmentVector;
	this->ReadyReplenishment = ReadyReplenishment;
	this->ReplenishmentRequestQueue = ReplenishmentRequestQueue;
	this->ReplenishmentOperationMutex = ReplenishmentOperationMutex;
	this->ReplenishmentRequestQueueMutex = ReplenishmentRequestQueueMutex;
}

ReplenishmentScheduler::~ReplenishmentScheduler()
{
}

void ReplenishmentScheduler::operator()()
{
	std::condition_variable cvAvailableReplenishmentOperation;
	std::mutex AvailableReplenishmentOperationMutex;

	while (true)
	{
		std::unique_lock<std::mutex> AvailableReplenishmentOperationLock(AvailableReplenishmentOperationMutex);
		cvAvailableReplenishmentOperation.wait(AvailableReplenishmentOperationLock, [this] {return !ReplenishmentRequestQueue->empty() && !ReadyReplenishment->empty(); });

		std::unique_lock<std::mutex> ReplenishmentOperationLock(*ReplenishmentOperationMutex,std::defer_lock);
		std::unique_lock<std::mutex> ReplenishmentRequestQueueLock(*ReplenishmentRequestQueueMutex, std::defer_lock);
		std::lock(ReplenishmentOperationLock, ReplenishmentRequestQueueLock);

		ReplenishmentVector->at(ReadyReplenishment->front()).addRequest(ReplenishmentRequestQueue->front());

		ReadyReplenishment->pop();
		ReplenishmentRequestQueue->pop();
	}
}
