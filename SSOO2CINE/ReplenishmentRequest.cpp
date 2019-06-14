#include "pch.h"
#include "ReplenishmentRequest.h"
#include "Stand.h"

ReplenishmentRequest::ReplenishmentRequest()
{
}

ReplenishmentRequest::ReplenishmentRequest(Stand * StandId)
{
	this->StandId = StandId;
}

ReplenishmentRequest::~ReplenishmentRequest()
{
}

void ReplenishmentRequest::Replenish(int PopcornAmount, int DrinksAmount)
{
	StandId->RefillPopcorn(PopcornAmount);
	StandId->RefillDrinks(DrinksAmount);
	StandId->RefilledStand();
}
