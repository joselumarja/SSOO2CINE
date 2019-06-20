#include <chrono>

#include "pch.h"
#include "Stand.h"
#include "termcolor.h"

Stand::Stand(int StandId, std::condition_variable *cvFoodStandTurn, std::condition_variable *cvFoodStandResponse, std::condition_variable *cvReplenishmentTurn, std::condition_variable *cvReplenishmentResponse, std::condition_variable *cvPayTurn, std::condition_variable *cvPayResponse, std::queue<FoodAndDrinkRequest> *FoodAndDrinkRequestQueue, std::mutex *FoodAndDrinkRequestQueueMutex, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentRequestQueueMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex, std::mutex *PrintMutex)
{
	this->StandId = StandId;
	this->cvFoodStandTurn = cvFoodStandTurn;
	this->cvFoodStandResponse = cvFoodStandResponse;
	this->cvReplenishmentTurn = cvReplenishmentTurn;
	this->cvReplenishmentResponse = cvReplenishmentResponse;
	this->cvPayTurn = cvPayTurn;
	this->cvPayResponse = cvPayResponse;
	this->FoodAndDrinkRequestQueue = FoodAndDrinkRequestQueue;
	this->FoodAndDrinkRequestQueueMutex = FoodAndDrinkRequestQueueMutex;
	this->ReplenishmentRequestQueue = ReplenishmentRequestQueue;
	this->ReplenishmentRequestQueueMutex = ReplenishmentRequestQueueMutex;
	this->PaymentRequestQueue = PaymentRequestQueue;
	this->PaymentRequestQueueMutex = PaymentRequestQueueMutex;
	this->PrintMutex = PrintMutex;
	IsRefilledStand = false;
	PaymentAccomplished = false;
	PopcorAmount = MAX_POPCORN_AMOUNT;
	DrinksAmount=MAX_DRINKS_AMOUNT;
}

Stand::~Stand()
{
}

void Stand::operator()()
{
	FoodAndDrinkRequest Request;

	std::mutex FoodOperationAvailableMutex;
	std::mutex ReplenishOperationCompletedMutex;
	std::mutex PaymentAccomplishedMutex;

	std::unique_lock<std::mutex> PrintLock(*PrintMutex);
	PrintLock.unlock();

	while (true)
	{
		std::unique_lock<std::mutex> FoodOperationAvailableLock(FoodOperationAvailableMutex);
		cvFoodStandTurn->wait(FoodOperationAvailableLock, [this] {return !FoodAndDrinkRequestQueue->empty(); });

		std::unique_lock<std::mutex> FoodAndDrinkRequestLock(*FoodAndDrinkRequestQueueMutex);
		Request = FoodAndDrinkRequestQueue->front();
		FoodAndDrinkRequestQueue->pop();
		FoodAndDrinkRequestLock.unlock();

		PrintLock.lock();
		std::cout << termcolor::blue << "[STAND " << std::to_string(StandId) << "] Attending Client: " << Request.getClientInfo() << termcolor::reset << std::endl;
		PrintLock.unlock();

		if (Request.getNumberOfDrinks() > DrinksAmount || Request.getNumberOfPopcorns() > PopcorAmount)
		{
			PrintLock.lock();
			std::cout << termcolor::yellow << "[STAND " << std::to_string(StandId) << "] Stockout" << termcolor::reset << std::endl;
			PrintLock.unlock();

			ReplenishmentRequest ReplenishRequest((Stand *) this);
			std::unique_lock<std::mutex> lk(*ReplenishmentRequestQueueMutex);
			ReplenishmentRequestQueue->push(ReplenishRequest);
			lk.unlock();

			cvReplenishmentTurn->notify_one();

			std::unique_lock<std::mutex> ReplenishOperationCompletedLock(ReplenishOperationCompletedMutex);
			cvReplenishmentResponse->wait(ReplenishOperationCompletedLock, [this] {return IsRefilledStand; });

			PrintLock.lock();
			std::cout << termcolor::yellow << "[STAND " << std::to_string(StandId) << "] Stock Replenish" << termcolor::reset << std::endl;
			PrintLock.unlock();

			IsRefilledStand = false;
		}

		std::this_thread::sleep_for(std::chrono::seconds(Request.getNumberOfDrinks()));

		PaymentRequest PayRequest = PaymentRequest(RequestOrigin::FoodAndDrink, (TradeNode*)this,(double) Request.getNumberOfDrinks()*DRINK_PRICE+Request.getNumberOfPopcorns()*POPCORN_PRICE);
		std::unique_lock<std::mutex> PaymentRequestQueueLock(*PaymentRequestQueueMutex);
		PaymentRequestQueue->push(PayRequest);
		PaymentRequestQueueLock.unlock();

		cvPayTurn->notify_one();

		PrintLock.lock();
		std::cout << termcolor::blue << "[STAND " << std::to_string(StandId) << "] Sending Payment Request" << termcolor::reset << std::endl;
		PrintLock.unlock();
		
		std::unique_lock<std::mutex> PaymentAccomplishedLock(PaymentAccomplishedMutex);
		cvPayResponse->wait(PaymentAccomplishedLock, [this] {return PaymentAccomplished; });
		DrinksAmount -= Request.getNumberOfDrinks();
		PopcorAmount -= Request.getNumberOfPopcorns();

		PrintLock.lock();
		std::cout << termcolor::blue << "[STAND " << std::to_string(StandId) << "] Client: " << Request.getClientInfo()<<" Attended" << termcolor::reset << std::endl;
		PrintLock.unlock();

		Request.RequestCompleted();

		cvFoodStandResponse->notify_all();

		PaymentAccomplished = false;

	}
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

std::string Stand::getInfo()
{
	return std::string("Stand "+std::to_string(StandId));
}

void Stand::PayAccomplished()
{
	PaymentAccomplished = true;
}
