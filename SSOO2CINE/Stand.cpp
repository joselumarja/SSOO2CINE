#include "pch.h"
#include "Stand.h"

Stand::Stand(int StandId, std::queue<int>* ReadyFoodStands, std::mutex * StandsOperationMutex, std::queue<ReplenishmentRequest> *ReplenishmentRequestQueue, std::mutex *ReplenishmentRequestQueueMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex)
{
	this->StandId = StandId;
	this->ReadyFoodStands = ReadyFoodStands;
	this->StandsOperationMutex = StandsOperationMutex;
	this->ReplenishmentRequestQueue = ReplenishmentRequestQueue;
	this->ReplenishmentRequestQueueMutex = ReplenishmentRequestQueueMutex;
	this->PaymentRequestQueue = PaymentRequestQueue;
	this->PaymentRequestQueueMutex = PaymentRequestQueueMutex;
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

		PaymentRequest PayRequest = PaymentRequest(RequestOrigin::FoodAndDrink, (TradeNode*)this,(double) Request.getNumberOfDrinks()*DRINK_PRICE+Request.getNumberOfPopcorns()*POPCORN_PRICE);
		std::lock_guard<std::mutex> PaymentRequestQueueLock(*PaymentRequestQueueMutex);
		PaymentRequestQueue->push(PayRequest);
		PaymentRequestQueueLock.~lock_guard();

		std::unique_lock<std::mutex> PaymentAccomplishedLock(PaymentAccomplishedMutex);
		cvPaymentAccomplished.wait(PaymentAccomplishedLock, [this] {return PaymentAccomplished; });
		DrinksAmount -= Request.getNumberOfDrinks();
		PopcorAmount -= Request.getNumberOfPopcorns();
		Request.RequestCompleted();
		PaymentAccomplished = false;

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

void Stand::PayAccomplished()
{
	PaymentAccomplished = true;
}
