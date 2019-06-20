#pragma once
#include <string>

class Stand;

class ReplenishmentRequest
{
public:
	ReplenishmentRequest();
	ReplenishmentRequest(Stand *StandId);
	~ReplenishmentRequest();
	void Replenish(int PopcornAmount, int DrinksAmount);

	std::string getStandInfo();

private:
	Stand *StandId;
};

