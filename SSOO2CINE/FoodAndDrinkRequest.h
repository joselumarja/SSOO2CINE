#pragma once

class Client;

class FoodAndDrinkRequest
{
public:
	FoodAndDrinkRequest();
	FoodAndDrinkRequest(unsigned short NumberOfPopcorns, unsigned short NumberOfDrinks);
	~FoodAndDrinkRequest();

	inline unsigned short getNumberOfDrinks()
	{
		return NumberOfDrinks;
	}
	inline unsigned short getNumberOfPopcorns()
	{
		return NumberOfPopcorns;
	}

	void setClientPointer(Client *client);

	std::string getClientInfo();

	void RequestCompleted();

private:
	Client *ClientId;
	unsigned short NumberOfPopcorns;
	unsigned short NumberOfDrinks;
};

