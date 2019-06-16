#pragma once
#include <queue>
#include <condition_variable>
#include "Stand.h"
#include "FoodAndDrinkRequest.h"

class StandsScheduler
{
public:
	StandsScheduler(std::vector<Stand> *StandsVector, std::queue<int> *ReadyFoodStands, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *StandsOperationMutex, std::mutex *FoodAndDrinkRequestQueueMutex);
	~StandsScheduler();
	void operator()();

private:
	std::queue<int> *ReadyFoodStands;
	std::vector<Stand> *StandsVector;
	std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue;

	std::mutex *StandsOperationMutex;
	std::mutex *FoodAndDrinkRequestQueueMutex;

};

