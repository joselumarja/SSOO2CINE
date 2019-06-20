#include "pch.h"
#include "PaymentScheduler.h"

PaymentScheduler::PaymentScheduler(std::condition_variable *cvPayTurn, std::condition_variable *cvPayAvailable, std::queue<PaymentRequest> *PaymentRequestQueue, std::priority_queue<PaymentPriorityRequest> *PaymentPriorityRequestQueue, std::mutex *PaymentRequestQueueMutex, std::mutex *PaymentPriorityRequestQueueMutex)
{
	this->cvPayTurn = cvPayTurn;
	this->cvPayAvailable = cvPayAvailable;
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

	std::mutex PaymentRequestAvailableMutex;

	int PaymentTicketsPriorityCount=0;

	while (true)
	{
		std::unique_lock<std::mutex> PaymentRequestAvailableLock(PaymentRequestAvailableMutex);
		cvPayTurn->wait(PaymentRequestAvailableLock, [this] {return !PaymentRequestQueue->empty(); });

		std::unique_lock<std::mutex> PaymentRequestQueueLock(*PaymentRequestQueueMutex);
		PayRequest = PaymentRequestQueue->front();
		PaymentRequestQueue->pop();
		PaymentRequestQueueLock.unlock();

		switch (PayRequest.getOrigin())
		{
		case RequestOrigin::TicketOffice:
			PayPriorityRequest = PaymentPriorityRequest(PaymentTicketsPriorityCount++, PayRequest);
			break;
		case RequestOrigin::FoodAndDrink:
			PayPriorityRequest = PaymentPriorityRequest(PaymentTicketsPriorityCount + PRIORITY_FACTOR, PayRequest);
			break;
		}

		std::unique_lock<std::mutex> PaymentPriorityRequestQueueLock(*PaymentPriorityRequestQueueMutex);
		PaymentPriorityRequestQueue->push(PayPriorityRequest);
		PaymentPriorityRequestQueueLock.unlock();

		cvPayAvailable->notify_one();
	}
}
