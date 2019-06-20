#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

#include "FoodAndDrinkRequest.h"
#include "ReplenishmentRequest.h"
#include "Definitions.h"
#include "TradeNode.h"
#include "PaymentRequest.h"

class Stand : public TradeNode
{
public:
	Stand(int StandId, std::condition_variable *cvFoodStandTurn, std::condition_variable *cvFoodStandResponse,std::condition_variable *cvReplenishmentTurn,std::condition_variable *cvReplenishmentResponse, std::condition_variable *cvPayTurn, std::condition_variable *cvPayResponse, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *FoodAndDrinkRequestQueueMutex, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentRequestQueueMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex, std::mutex *PrintMutex);
	~Stand();
	void operator()();

	void RefillPopcorn(int PopcornAmount);
	void RefillDrinks(int DrinksAmount);
	void RefilledStand();

	std::string getInfo();

	void PayAccomplished();

private:
	int StandId;
	int PopcorAmount;
	int DrinksAmount;
	bool IsRefilledStand;

	std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue;
	std::mutex *FoodAndDrinkRequestQueueMutex;

	std::condition_variable *cvFoodStandTurn;
	std::condition_variable *cvFoodStandResponse;
	std::condition_variable *cvReplenishmentTurn;
	std::condition_variable *cvReplenishmentResponse;
	std::condition_variable *cvPayTurn;
	std::condition_variable *cvPayResponse;

	std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue;
	std::mutex *ReplenishmentRequestQueueMutex;

	std::queue<PaymentRequest> *PaymentRequestQueue;
	std::mutex *PaymentRequestQueueMutex;

	std::mutex *PrintMutex;

	bool PaymentAccomplished;
};

