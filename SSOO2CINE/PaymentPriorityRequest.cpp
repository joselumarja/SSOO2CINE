#include "pch.h"
#include "PaymentPriorityRequest.h"


PaymentPriorityRequest::PaymentPriorityRequest()
{
}


PaymentPriorityRequest::PaymentPriorityRequest(int Priority, PaymentRequest PayRequest)
{
	this->Priority = Priority;
	this->PayRequest = PayRequest;
}

PaymentPriorityRequest::~PaymentPriorityRequest()
{
}
