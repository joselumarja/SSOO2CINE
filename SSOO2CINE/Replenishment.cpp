#include <string>
#include <chrono>

#include "pch.h"
#include "Replenishment.h"
#include "termcolor.h"

Replenishment::Replenishment(int ReplenishmentId, std::condition_variable *cvReplenishmentTurn, std::condition_variable *cvReplenishmentResponse, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentRequestQueueMutex, std::mutex *PrintMutex)
{
	this->ReplenishmentId = ReplenishmentId;
	this->cvReplenishmentTurn = cvReplenishmentTurn;
	this->cvReplenishmentResponse = cvReplenishmentResponse;
	this->ReplenishmentRequestQueue = ReplenishmentRequestQueue;
	this->ReplenishmentRequestQueueMutex = ReplenishmentRequestQueueMutex;
	this->PrintMutex = PrintMutex;
}

Replenishment::~Replenishment()
{
}

void Replenishment::operator()()
{
	ReplenishmentRequest Request;

	std::mutex ReplenishOperationAvailableMutex;
	
	std::unique_lock<std::mutex> PrintLock(*PrintMutex);
	PrintLock.unlock();

	while (true)
	{
		std::unique_lock<std::mutex> ReplenishmentOperationAvailableLock(ReplenishOperationAvailableMutex);
		cvReplenishmentTurn->wait(ReplenishmentOperationAvailableLock, [this] {return !ReplenishmentRequestQueue->empty(); });
		
		std::unique_lock<std::mutex> ReplenishmentRequestQueueLock(*ReplenishmentRequestQueueMutex);
		Request = ReplenishmentRequestQueue->front();
		ReplenishmentRequestQueue->pop();
		ReplenishmentRequestQueueLock.unlock();

		PrintLock.lock();
		std::cout << termcolor::cyan << "[REPLENISHMENT " << std::to_string(ReplenishmentId) << "] Attending: " << Request.getStandInfo() << termcolor::reset << std::endl;
		PrintLock.unlock();

		std::this_thread::sleep_for(std::chrono::seconds(10));

		PrintLock.lock();
		std::cout << termcolor::cyan << "[REPLENISHMENT " << std::to_string(ReplenishmentId) << "] " << Request.getStandInfo()<<" Attended" << termcolor::reset << std::endl;
		PrintLock.unlock();

		Request.Replenish(MAX_POPCORN_AMOUNT, MAX_DRINKS_AMOUNT);
		cvReplenishmentResponse->notify_all();
	}
}