#include "pch.h"
#include "Stand.h"

Stand::Stand(int StandId, std::queue<int>* ReadyFoodStands, std::mutex * StandsOperationMutex, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentRequestQueueMutex):IsRefilledStand(false),PopcorAmount(MAX_POPCORN_AMOUNT),DrinksAmount(MAX_DRINKS_AMOUNT)
{
	this->StandId = StandId;
	this->ReadyFoodStands = ReadyFoodStands;
	this->StandsOperationMutex = StandsOperationMutex;
	this->ReplenishmentRequestQueue = ReplenishmentRequestQueue;
	this->ReplenishmentRequestQueueMutex = ReplenishmentRequestQueueMutex;
}

Stand::~Stand()
{
}

void Stand::operator()()
{
	FoodAndDrinkRequest Request;

	while (true)
	{
		std::unique_lock<std::mutex> FoodOperationAvailableLock(FoodOperationAvailableMutex);
		cvFoodOperationAvailable.wait(FoodOperationAvailableLock, [this] {return !FoodAndDrinkRequests.empty(); });

		std::lock_guard<std::mutex> lk(RequestsOperationMutex);
		Request = FoodAndDrinkRequests.front();
		FoodAndDrinkRequests.pop();
		lk.~lock_guard();

		if (Request.getNumberOfDrinks() > DrinksAmount || Request.getNumberOfPopcorns() > PopcorAmount)
		{
			ReplenishmentRequest ReplenishRequest((Stand *) this);
			std::lock_guard<std::mutex> lk(*ReplenishmentRequestQueueMutex);
			ReplenishmentRequestQueue->push(ReplenishRequest);
			lk.~lock_guard();

			std::unique_lock<std::mutex> ReplenishOperationCompletedLock(ReplenishOperationCompletedMutex);
			cvReplenishOperationCompleted.wait(ReplenishOperationCompletedLock, [this] {return IsRefilledStand; });
			IsRefilledStand = false;
		}

		//A PAGAR
		DrinksAmount -= Request.getNumberOfDrinks();
		PopcorAmount -= Request.getNumberOfPopcorns();
		Request.RequestCompleted();

		std::lock_guard<std::mutex> StandsOperationLock(*StandsOperationMutex);
		ReadyFoodStands->push(StandId);
	}
}

void Stand::addRequest(FoodAndDrinkRequest FoodDrinkRequest)
{
	std::lock_guard<std::mutex> lk(RequestsOperationMutex);
	FoodAndDrinkRequests.push(FoodDrinkRequest);
}

void Stand::RefillPopcorn(int PopcornAmount)
{
	this->PopcorAmount += PopcorAmount;
}

void Stand::RefillDrinks(int DrinksAmount)
{
	this->DrinksAmount += DrinksAmount;
}

void Stand::RefilledStand()
{
	IsRefilledStand = true;
}
