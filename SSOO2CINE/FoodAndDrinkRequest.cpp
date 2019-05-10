#include "pch.h"
#include "FoodAndDrinkRequest.h"


FoodAndDrinkRequest::FoodAndDrinkRequest()
{
}

FoodAndDrinkRequest::FoodAndDrinkRequest(unsigned char NumberOfPopcorns, unsigned char NumberOfDrinks)
{
	this->NumberOfPopcorns = NumberOfPopcorns;
	this->NumberOfDrinks = NumberOfDrinks;
}


FoodAndDrinkRequest::~FoodAndDrinkRequest()
{
}

unsigned char FoodAndDrinkRequest::getNumberOfDrinks()
{
	return NumberOfDrinks;
}

unsigned char FoodAndDrinkRequest::getNumberOfPopcorns()
{
	return NumberOfPopcorns;
}
