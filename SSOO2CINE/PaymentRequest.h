#pragma once
#include <string>

#include "Definitions.h"

class TradeNode;

enum class RequestOrigin
{
	TicketOffice,
	FoodAndDrink
};

class PaymentRequest
{
public:
	PaymentRequest();
	PaymentRequest(RequestOrigin Origin,TradeNode *TradeOrigin, double Money);
	~PaymentRequest();

	void notifyPaymentAccepted();

	inline const RequestOrigin getOrigin()
	{
		return Origin;
	}
	inline const double getMoney()
	{
		return Money;
	}

	std::string getOriginInfo();
	
private:
	RequestOrigin Origin;
	TradeNode *TradeOrigin;
	double Money;

};

