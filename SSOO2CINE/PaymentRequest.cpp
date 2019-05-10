#include "pch.h"
#include "PaymentRequest.h"
#include <time.h>

PaymentRequest::PaymentRequest()
{
}

PaymentRequest::PaymentRequest(RequestOrigin Origin, float Money)
{

	auto Time = std::chrono::system_clock::now();

	this->Origin = Origin;

	if (Origin == RequestOrigin::FoodAndDrink)
	{
		Time += std::chrono::seconds(5);
	}

	TimeStamp = std::chrono::system_clock::to_time_t(Time);
	this->Money = Money;
}

PaymentRequest::PaymentRequest(RequestOrigin Origin, unsigned char StandNumber, float Money)
{
	auto Time = std::chrono::system_clock::now();

	this->Origin = Origin;

	if (Origin == RequestOrigin::FoodAndDrink)
	{
		Time += std::chrono::seconds(5);
	}

	TimeStamp = std::chrono::system_clock::to_time_t(Time);
	this->Money = Money;
	this->StandNumber = StandNumber;
}

PaymentRequest::~PaymentRequest()
{
}

const bool PaymentRequest::operator<(const PaymentRequest & PR)
{
	return TimeStamp < PR.TimeStamp;
}

const RequestOrigin PaymentRequest::getOrigin()
{
	return Origin;
}

const unsigned char PaymentRequest::getStandNumber()
{
	return StandNumber;
}

const float PaymentRequest::getMoney()
{
	return Money;
}

const __time64_t PaymentRequest::getTime()
{
	return TimeStamp;
}

/*bool operator<(const PaymentRequest & PR1, const PaymentRequest & PR2)
{
	return PR1 < PR2;
}*/
