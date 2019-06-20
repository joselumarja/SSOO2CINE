#include <string>

#include "pch.h"
#include "Client.h"
#include "FoodAndDrinkRequest.h"


FoodAndDrinkRequest::FoodAndDrinkRequest()
{
}

FoodAndDrinkRequest::FoodAndDrinkRequest(unsigned short NumberOfPopcorns, unsigned short NumberOfDrinks)
{
	this->NumberOfPopcorns = NumberOfPopcorns;
	this->NumberOfDrinks = NumberOfDrinks;
}

FoodAndDrinkRequest::~FoodAndDrinkRequest()
{
}

void FoodAndDrinkRequest::setClientPointer(Client * client)
{
	this->ClientId = client;
}

std::string FoodAndDrinkRequest::getClientInfo()
{
	return std::to_string(ClientId->getClientId());
}

void FoodAndDrinkRequest::RequestCompleted()
{
	ClientId->acceptFoodAndDrinkRequest();
}
