#pragma once

class Stand;

class ReplenishmentRequest
{
public:
	ReplenishmentRequest();
	ReplenishmentRequest(Stand *StandId);
	~ReplenishmentRequest();
	void Replenish(int PopcornAmount, int DrinksAmount);

private:
	Stand *StandId;
};

