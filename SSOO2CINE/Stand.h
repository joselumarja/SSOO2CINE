#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "FoodAndDrinkRequest.h"
#include "ReplenishmentRequest.h"
#include "Definitions.h"

class Stand
{
public:
	Stand(int StandId, std::queue<int> *ReadyFoodStands, std::mutex *StandsOperationMutex, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentQueueMutex);
	~Stand();
	void operator()();
	void addRequest(FoodAndDrinkRequest FoodDrinkRequest);
	void RefillPopcorn(int PopcornAmount);
	void RefillDrinks(int DrinksAmount);
	void RefilledStand();

private:
	int StandId;
	int PopcorAmount;
	int DrinksAmount;
	bool IsRefilledStand;

	std::queue<int> *ReadyFoodStands;
	std::mutex *StandsOperationMutex;

	std::queue<FoodAndDrinkRequest> FoodAndDrinkRequests;
	std::mutex RequestsOperationMutex;

	std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue;
	std::mutex *ReplenishmentRequestQueueMutex;

	std::mutex FoodOperationAvailableMutex;
	std::condition_variable cvFoodOperationAvailable;

	std::condition_variable cvReplenishOperationCompleted;
	std::mutex ReplenishOperationCompletedMutex;
};

