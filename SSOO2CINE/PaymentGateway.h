#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "PaymentPriorityRequest.h"

class PaymentGateway
{
public:
	PaymentGateway(std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue, std::mutex *PaymentPriorityRequestQueueMutex);
	~PaymentGateway();
	void operator()();

private:
	std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue;
	std::mutex *PaymentPriorityRequestQueueMutex;

	std::condition_variable cvPaymentRequestAvailable;
	std::mutex PaymentRequestAvailableMutex;
};

