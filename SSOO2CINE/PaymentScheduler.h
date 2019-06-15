#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

#include "PaymentRequest.h"
#include "PaymentPriorityRequest.h"

class PaymentScheduler
{
public:
	PaymentScheduler(std::queue<PaymentRequest> *PaymentRequestQueue, std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue, std::mutex *PaymentRequestQueueMutex, std::mutex *PaymentPriorityRequestQueueMutex);
	~PaymentScheduler();
	void operator()();

private:
	std::queue<PaymentRequest> *PaymentRequestQueue;
	std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue;

	std::mutex *PaymentRequestQueueMutex;
	std::mutex *PaymentPriorityRequestQueueMutex;

	std::condition_variable cvPaymentRequestAvailable;
	std::mutex PaymentRequestAvailableMutex;
};

