#pragma once

class Client;

class FoodAndDrinkRequest
{
public:
	FoodAndDrinkRequest();
	FoodAndDrinkRequest(Client *ClientId, unsigned char NumberOfPopcorns, unsigned char NumberOfDrinks);
	~FoodAndDrinkRequest();

	inline unsigned char getNumberOfDrinks()
	{
		return NumberOfDrinks;
	}
	inline unsigned char getNumberOfPopcorns()
	{
		return NumberOfPopcorns;
	}

	void RequestCompleted();

private:
	Client *ClientId;
	unsigned char NumberOfPopcorns;
	unsigned char NumberOfDrinks;
};

