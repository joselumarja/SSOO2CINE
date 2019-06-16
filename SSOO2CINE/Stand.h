#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "FoodAndDrinkRequest.h"
#include "ReplenishmentRequest.h"
#include "Definitions.h"
#include "TradeNode.h"
#include "PaymentRequest.h"

class Stand : public TradeNode
{
public:
	Stand(int StandId, std::queue<int> *ReadyFoodStands, std::mutex *StandsOperationMutex, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentQueueMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex);
	~Stand();
	void operator()();

	void addRequest(FoodAndDrinkRequest FoodDrinkRequest);

	void RefillPopcorn(int PopcornAmount);
	void RefillDrinks(int DrinksAmount);
	void RefilledStand();

	void PayAccomplished();

private:
	int StandId;
	int PopcorAmount;
	int DrinksAmount;
	bool IsRefilledStand;

	std::queue<int> *ReadyFoodStands;
	std::mutex *StandsOperationMutex;

	std::queue<FoodAndDrinkRequest> FoodAndDrinkRequests;

	std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue;
	std::mutex *ReplenishmentRequestQueueMutex;

	std::queue<PaymentRequest> *PaymentRequestQueue;
	std::mutex *PaymentRequestQueueMutex;

	bool PaymentAccomplished;
};

