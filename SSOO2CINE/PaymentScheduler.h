#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

#include "PaymentRequest.h"
#include "PaymentPriorityRequest.h"

class PaymentScheduler
{
public:
	PaymentScheduler(std::condition_variable *cvPayTurn, std::condition_variable *cvPayAvailable, std::queue<PaymentRequest> *PaymentRequestQueue, std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue, std::mutex *PaymentRequestQueueMutex, std::mutex *PaymentPriorityRequestQueueMutex);
	~PaymentScheduler();
	void operator()();

private:

	std::condition_variable *cvPayTurn;
	std::condition_variable *cvPayAvailable;

	std::queue<PaymentRequest> *PaymentRequestQueue;
	std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue;

	std::mutex *PaymentRequestQueueMutex;
	std::mutex *PaymentPriorityRequestQueueMutex;

};

