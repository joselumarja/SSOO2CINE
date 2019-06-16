#include "pch.h"
#include "PaymentScheduler.h"

PaymentScheduler::PaymentScheduler(std::queue<PaymentRequest>* PaymentRequestQueue, std::priority_queue<PaymentPriorityRequest>* PaymentPriorityRequestQueue, std::mutex * PaymentRequestQueueMutex, std::mutex * PaymentPriorityRequestQueueMutex)
{
	this->PaymentRequestQueue = PaymentRequestQueue;
	this->PaymentPriorityRequestQueue = PaymentPriorityRequestQueue;
	this->PaymentRequestQueueMutex = PaymentRequestQueueMutex;
	this->PaymentPriorityRequestQueueMutex = PaymentPriorityRequestQueueMutex;
}

PaymentScheduler::~PaymentScheduler()
{
}

void PaymentScheduler::operator()()
{
	PaymentRequest PayRequest;
	PaymentPriorityRequest PayPriorityRequest;

	std::condition_variable cvPaymentRequestAvailable;
	std::mutex PaymentRequestAvailableMutex;

	int PaymentTicketsPriorityCount=0;

	while (true)
	{
		std::unique_lock<std::mutex> PaymentRequestAvailableLock(PaymentRequestAvailableMutex);
		cvPaymentRequestAvailable.wait(PaymentRequestAvailableLock, [this] {return !PaymentRequestQueue->empty(); });

		std::lock_guard<std::mutex> PaymentRequestQueueLock(*PaymentRequestQueueMutex);
		PayRequest = PaymentRequestQueue->front();
		PaymentRequestQueue->pop();
		PaymentRequestQueueLock.~lock_guard();

		switch (PayRequest.getOrigin())
		{
		case RequestOrigin::TicketOffice:
			PayPriorityRequest = PaymentPriorityRequest(PaymentTicketsPriorityCount++, PayRequest);
			break;
		case RequestOrigin::FoodAndDrink:
			PayPriorityRequest = PaymentPriorityRequest(PaymentTicketsPriorityCount + PRIORITY_FACTOR, PayRequest);
			break;
		}

		std::lock_guard<std::mutex> PaymentPriorityRequestQueueLock(*PaymentPriorityRequestQueueMutex);
		PaymentPriorityRequestQueue->push(PayPriorityRequest);
	}
}
