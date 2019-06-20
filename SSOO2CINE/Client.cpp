#include <string>
#include <chrono>
#include <math.h>

#include "pch.h"
#include "Client.h"
#include "termcolor.h"

Client::Client()
{
}

Client::Client(int ClientId, TicketsRequest TicketRequest, FoodAndDrinkRequest FoodDrinkRequest, std::condition_variable *cvTicketOfficeTurn, std::condition_variable *cvTicketOfficeResponse, std::condition_variable *cvFoodStandTurn, std::condition_variable *cvFoodStandResponse, std::queue<TicketsRequest> *TicketRequestQueue, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *TicketsRequestQueueMutex, std::mutex *FoodAndDrinkRequestQueueMutex, std::mutex *PrintMutex)
{
	this->ClientId = ClientId;
	this->TicketRequest = TicketRequest;
	this->FoodDrinkRequest = FoodDrinkRequest;
	this->cvFoodStandTurn = cvFoodStandTurn;
	this->cvFoodStandResponse = cvFoodStandResponse;
	this->cvTicketOfficeTurn = cvTicketOfficeTurn;
	this->cvTicketOfficeResponse = cvTicketOfficeResponse;
	this->TicketRequestQueue = TicketRequestQueue;
	this->FoodAndDrinkRequestQueue = FoodAndDrinkRequestQueue;
	this->TicketsRequestQueueMutex = TicketsRequestQueueMutex;
	this->FoodAndDrinkRequestQueueMutex = FoodAndDrinkRequestQueueMutex;
	this->PrintMutex = PrintMutex;
	TicketsReadyOperation = false;
	TicketsAcceptedOperation = false;
	FoodAndDrinkReadyOperation = false;
}


Client::~Client()
{
}

void Client::setClientPointer()
{
	this->TicketRequest.setClientPointer((Client*)this);
	this->FoodDrinkRequest.setClientPointer((Client*)this);
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
	setClientPointer();
	std::unique_lock<std::mutex> PrintLock(*PrintMutex);
	PrintLock.unlock();

	std::mutex TicketReadyOperationMutex;
	std::mutex FoodAndDrinkReadyOperationMutex;

	PrintLock.lock();
	std::cout << termcolor::green << "[CLIENT " << std::to_string(ClientId) << "] Waiting in Ticket Queue" << termcolor::reset << std::endl;
	PrintLock.unlock();

	std::unique_lock<std::mutex> TicketsRequestQueueLock(*TicketsRequestQueueMutex);
	TicketRequestQueue->push(TicketRequest);
	TicketsRequestQueueLock.unlock();

	cvTicketOfficeTurn->notify_one();

	std::unique_lock<std::mutex> TicketReadyOperationLock(TicketReadyOperationMutex);
	cvTicketOfficeResponse->wait(TicketReadyOperationLock, [this] {return TicketsReadyOperation; });

	if (!TicketsAcceptedOperation)
	{
		PrintLock.lock();
		std::cout << termcolor::green << "[CLIENT " << std::to_string(ClientId) << "] Ticket Request Denied" << termcolor::reset << std::endl;
		PrintLock.unlock();
		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds((rand() % 10) + 5));

	std::unique_lock<std::mutex> FoodAndDrinkRequestQueueLock(*FoodAndDrinkRequestQueueMutex);
	FoodAndDrinkRequestQueue->push(FoodDrinkRequest);
	FoodAndDrinkRequestQueueLock.unlock();

	cvFoodStandTurn->notify_one();

	PrintLock.lock();
	std::cout << termcolor::green << "[CLIENT " << std::to_string(ClientId) << "] Waiting in Stand Queue" << termcolor::reset << std::endl;
	PrintLock.unlock();

	std::unique_lock<std::mutex> FoodAndDrinkReadyOperationLock(FoodAndDrinkReadyOperationMutex);
	cvFoodStandResponse->wait(FoodAndDrinkReadyOperationLock, [this] {return FoodAndDrinkReadyOperation; });

	PrintLock.lock();
	std::cout << termcolor::green << "[CLIENT " << std::to_string(ClientId) << "] Inside Cinema" << termcolor::reset << std::endl;
	PrintLock.unlock();
}
