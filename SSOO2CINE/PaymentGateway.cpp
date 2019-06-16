#include "pch.h"
#include "PaymentGateway.h"

PaymentGateway::PaymentGateway(std::priority_queue<PaymentPriorityRequest>* PaymentPriorityRequestQueue, std::mutex * PaymentPriorityRequestQueueMutex)
{
	this->PaymentPriorityRequestQueue = PaymentPriorityRequestQueue;
	this->PaymentPriorityRequestQueueMutex = PaymentPriorityRequestQueueMutex;
}

PaymentGateway::~PaymentGateway()
{
}

void PaymentGateway::operator()()
{
	PaymentPriorityRequest PayRequest;

	std::condition_variable cvPaymentRequestAvailable;
	std::mutex PaymentRequestAvailableMutex;

	while (true)
	{
		std::unique_lock<std::mutex> PaymentRequestAvailableLock(PaymentRequestAvailableMutex);
		cvPaymentRequestAvailable.wait(PaymentRequestAvailableLock, [this] {return !PaymentPriorityRequestQueue->empty(); });

		std::lock_guard<std::mutex> PaymentPriorityRequestQueueLock(*PaymentPriorityRequestQueueMutex);
		PayRequest = PaymentPriorityRequestQueue->top();
		PaymentPriorityRequestQueue->pop();
		PaymentPriorityRequestQueueLock.~lock_guard();

		PayRequest.getPaymentRequest().notifyPaymentAccepted();
	}
}
