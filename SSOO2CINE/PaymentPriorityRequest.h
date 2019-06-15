#pragma once
#include "PaymentRequest.h"

class PaymentPriorityRequest
{
public:
	PaymentPriorityRequest();
	PaymentPriorityRequest(int Priority, PaymentRequest PayRequest);
	~PaymentPriorityRequest();

	inline PaymentRequest getPaymentRequest()
	{
		return PayRequest;
	}

	bool operator<(const PaymentPriorityRequest &PayRequest) const
	{
		return this->Priority < PayRequest.Priority;
	}

private:
	int Priority;
	PaymentRequest PayRequest;
};

