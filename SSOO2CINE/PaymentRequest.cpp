#include "pch.h"
#include "PaymentRequest.h"
#include "TradeNode.h"

PaymentRequest::PaymentRequest()
{
}

PaymentRequest::PaymentRequest(RequestOrigin Origin, TradeNode * TradeOrigin, double Money)
{
	this->Origin = Origin;
	this->TradeOrigin = TradeOrigin;
	this->Money = Money;
}

PaymentRequest::~PaymentRequest()
{
}

void PaymentRequest::notifyPaymentAccepted()
{
	TradeOrigin->PayAcomplished();
}
