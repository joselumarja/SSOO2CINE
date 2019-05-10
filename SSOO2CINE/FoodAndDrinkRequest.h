#pragma once
class FoodAndDrinkRequest
{
public:
	FoodAndDrinkRequest();
	FoodAndDrinkRequest(unsigned char NumberOfPopcorns, unsigned char NumberOfDrinks);
	~FoodAndDrinkRequest();
	unsigned char getNumberOfDrinks();
	unsigned char getNumberOfPopcorns();

private:
	unsigned char NumberOfPopcorns;
	unsigned char NumberOfDrinks;
};

