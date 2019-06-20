#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "PaymentPriorityRequest.h"

class PaymentGateway
{
public:
	PaymentGateway(std::condition_variable *cvPayAvailable, std::condition_variable *cvPayResponse, std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue, std::mutex *PaymentPriorityRequestQueueMutex, std::mutex *PrintMutex);
	~PaymentGateway();
	void operator()();

private:

	std::condition_variable *cvPayAvailable;
	std::condition_variable *cvPayResponse;

	std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue;
	std::mutex *PaymentPriorityRequestQueueMutex;

	std::mutex *PrintMutex;
};

