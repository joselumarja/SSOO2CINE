#include "pch.h"
#include "StandsScheduler.h"

StandsScheduler::StandsScheduler(std::vector<Stand>* StandsVector, std::queue<int>* ReadyFoodStands, std::queue<FoodAndDrinkRequest>* FoodAndDrinkRequestQueue, std::mutex * StandsOperationMutex, std::mutex * FoodAndDrinkRequestQueueMutex)
{
	this->StandsVector = StandsVector;
	this->ReadyFoodStands = ReadyFoodStands;
	this->FoodAndDrinkRequestQueue = FoodAndDrinkRequestQueue;
	this->StandsOperationMutex = StandsOperationMutex;
	this->FoodAndDrinkRequestQueueMutex = FoodAndDrinkRequestQueueMutex;
}

StandsScheduler::~StandsScheduler()
{
}

void StandsScheduler::operator()()
{
	std::condition_variable cvAvailableFoodOperation;
	std::mutex AvailableFoodOperationMutex;

	while (true)
	{
		std::unique_lock<std::mutex> AvailableFoodOperationLock(AvailableFoodOperationMutex);
		cvAvailableFoodOperation.wait(AvailableFoodOperationLock, [this] {return !FoodAndDrinkRequestQueue->empty() && !ReadyFoodStands->empty(); });

		std::unique_lock<std::mutex> StandsOperationLock(*StandsOperationMutex, std::defer_lock);
		std::unique_lock<std::mutex> FoodAndDrinkRequestQueueLock(*FoodAndDrinkRequestQueueMutex, std::defer_lock);
		std::lock(StandsOperationLock, FoodAndDrinkRequestQueueLock);

		StandsVector->at(ReadyFoodStands->front()).addRequest(FoodAndDrinkRequestQueue->front());

		ReadyFoodStands->pop();
		FoodAndDrinkRequestQueue->pop();
	}
}
