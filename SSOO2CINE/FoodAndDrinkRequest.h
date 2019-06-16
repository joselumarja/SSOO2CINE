#pragma once

class Client;

class FoodAndDrinkRequest
{
public:
	FoodAndDrinkRequest();
	FoodAndDrinkRequest(Client *ClientId, unsigned short NumberOfPopcorns, unsigned short NumberOfDrinks);
	~FoodAndDrinkRequest();

	inline unsigned short getNumberOfDrinks()
	{
		return NumberOfDrinks;
	}
	inline unsigned short getNumberOfPopcorns()
	{
		return NumberOfPopcorns;
	}

	void RequestCompleted();

private:
	Client *ClientId;
	unsigned short NumberOfPopcorns;
	unsigned short NumberOfDrinks;
};

