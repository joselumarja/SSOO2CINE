#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "TicketsRequest.h"
#include "FoodAndDrinkRequest.h"

class Client
{
public:
	Client();
	Client(int ClientId, TicketsRequest TicketRequest, FoodAndDrinkRequest FoodDrinkRequest, std::condition_variable *cvTicketOfficeTurn, std::condition_variable *cvTicketOfficeResponse, std::condition_variable *cvFoodStandTurn, std::condition_variable *cvFoodStandResponse, std::queue<TicketsRequest> *TicketRequestQueue, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *TicketsRequestQueueMutex, std::mutex *FoodAndDrinkRequestQueueMutex, std::mutex *PrintMutex);
	~Client();
	void setClientPointer();

	void acceptTicketRequest();
	void denyTicketRequest();
	void acceptFoodAndDrinkRequest();

	inline int getClientId()
	{
		return ClientId;
	}

	void operator()();

private:
	int ClientId;
	TicketsRequest TicketRequest;
	FoodAndDrinkRequest FoodDrinkRequest;

	std::condition_variable *cvTicketOfficeTurn;
	std::condition_variable *cvTicketOfficeResponse;

	std::condition_variable *cvFoodStandTurn;
	std::condition_variable *cvFoodStandResponse;

	std::queue<TicketsRequest> *TicketRequestQueue;
	std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue;

	std::mutex *TicketsRequestQueueMutex;
	std::mutex *FoodAndDrinkRequestQueueMutex;

	std::mutex *PrintMutex;

	bool TicketsReadyOperation;
	bool TicketsAcceptedOperation;
	bool FoodAndDrinkReadyOperation;
};

