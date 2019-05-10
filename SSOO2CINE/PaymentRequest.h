#pragma once
#include <chrono>

#include "Definitions.h"

enum class RequestOrigin
{
	TicketOffice,
	FoodAndDrink
};

/*class MorePriority
{
public:
	bool operator()(const PaymentRequest &lhs, const PaymentRequest &rhs) const
	{
		return &lhs < &rhs;
	}
};*/

//bool operator<(const PaymentRequest & PR1, const PaymentRequest & PR2);

class PaymentRequest
{
public:
	PaymentRequest();
	PaymentRequest(RequestOrigin Origin, float Money);
	PaymentRequest(RequestOrigin Origin, unsigned char StandNumber, float Money);
	~PaymentRequest();
	const bool operator<(const PaymentRequest & PR);
	const RequestOrigin getOrigin();
	const unsigned char getStandNumber();
	const float getMoney();
	const __time64_t getTime();
	
private:
	__time64_t TimeStamp;
	RequestOrigin Origin;
	unsigned char StandNumber;
	float Money;

};

