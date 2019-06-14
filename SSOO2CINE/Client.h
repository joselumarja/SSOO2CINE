#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "TicketsRequest.h"
#include "FoodAndDrinkRequest.h"

class Client
{
public:
	Client(int ClientId, std::queue<TicketsRequest> *TicketRequestQueue, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *TicketsRequestQueueMutex, std::mutex *FoodAndDrinkRequestQueueMutex);
	~Client();
	void setRequests(TicketsRequest TicketRequest, FoodAndDrinkRequest FoodDrinkRequest);

	void acceptTicketRequest();
	void denyTicketRequest();
	void acceptFoodAndDrinkRequest();

	void operator()();

private:
	int ClientId;
	TicketsRequest TicketRequest;
	FoodAndDrinkRequest FoodDrinkRequest;

	std::queue<TicketsRequest> *TicketRequestQueue;
	std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue;

	std::mutex *TicketsRequestQueueMutex;
	std::mutex *FoodAndDrinkRequestQueueMutex;

	std::condition_variable cvTicketReadyOperation;
	std::condition_variable cvFoodAndDrinkReadyOperation;

	std::mutex TicketReadyOperationMutex;
	std::mutex FoodAndDrinkReadyOperationMutex;

	bool TicketsReadyOperation;
	bool TicketsAcceptedOperation;
	bool FoodAndDrinkReadyOperation;
};

