#include "pch.h"
#include "Client.h"
#include "FoodAndDrinkRequest.h"


FoodAndDrinkRequest::FoodAndDrinkRequest()
{
}

FoodAndDrinkRequest::FoodAndDrinkRequest(Client *ClientId, unsigned short NumberOfPopcorns, unsigned short NumberOfDrinks)
{
	this->ClientId = ClientId;
	this->NumberOfPopcorns = NumberOfPopcorns;
	this->NumberOfDrinks = NumberOfDrinks;
}

FoodAndDrinkRequest::~FoodAndDrinkRequest()
{
}

void FoodAndDrinkRequest::RequestCompleted()
{
	ClientId->acceptFoodAndDrinkRequest();
}
