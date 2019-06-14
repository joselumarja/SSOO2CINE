#include "pch.h"
#include "Client.h"



Client::Client(int ClientId, std::queue<TicketsRequest> *TicketRequestQueue, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *TicketsRequestQueueMutex, std::mutex *FoodAndDrinkRequestQueueMutex)
{
	this->ClientId = ClientId;
	this->TicketRequestQueue = TicketRequestQueue;
	this->FoodAndDrinkRequestQueue = FoodAndDrinkRequestQueue;
	this->TicketsRequestQueueMutex = TicketsRequestQueueMutex;
	this->FoodAndDrinkRequestQueueMutex = FoodAndDrinkRequestQueueMutex;
	TicketsReadyOperation = false;
	TicketsAcceptedOperation = false;
	FoodAndDrinkReadyOperation = false;
}


Client::~Client()
{
}

void Client::setRequests(TicketsRequest TicketRequest, FoodAndDrinkRequest FoodDrinkRequest)
{
	this->TicketRequest = TicketRequest;
	this->FoodDrinkRequest = FoodDrinkRequest;
}

void Client::acceptTicketRequest()
{
	TicketsAcceptedOperation = true;
	TicketsReadyOperation = true;
}

void Client::denyTicketRequest()
{
	TicketsAcceptedOperation = false;
	TicketsReadyOperation = true;
}

void Client::acceptFoodAndDrinkRequest()
{
	FoodAndDrinkReadyOperation = true;
}

void Client::operator()()
{
	std::lock_guard<std::mutex> TicketsRequestQueueLock(*TicketsRequestQueueMutex);
	TicketRequestQueue->push(TicketRequest);
	TicketsRequestQueueLock.~lock_guard();

	std::unique_lock<std::mutex> TicketReadyOperationLock(TicketReadyOperationMutex);
	cvTicketReadyOperation.wait(TicketReadyOperationLock, [this] {return TicketsReadyOperation; });

	if (!TicketsAcceptedOperation)
	{

		return;
	}

	std::lock_guard<std::mutex> FoodAndDrinkRequestQueueLock(*FoodAndDrinkRequestQueueMutex);
	FoodAndDrinkRequestQueue->push(FoodDrinkRequest);
	FoodAndDrinkRequestQueueLock.~lock_guard();

	std::unique_lock<std::mutex> FoodAndDrinkReadyOperationLock(FoodAndDrinkReadyOperationMutex);
	cvFoodAndDrinkReadyOperation.wait(FoodAndDrinkReadyOperationLock, [this] {return FoodAndDrinkReadyOperation; });

}
