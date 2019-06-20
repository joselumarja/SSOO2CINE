#include <chrono>

#include "pch.h"
#include "PaymentGateway.h"
#include "termcolor.h"

PaymentGateway::PaymentGateway(std::condition_variable *cvPayAvailable, std::condition_variable *cvPayResponse, std::priority_queue<PaymentPriorityRequest>* PaymentPriorityRequestQueue, std::mutex * PaymentPriorityRequestQueueMutex, std::mutex *PrintMutex)
{
	this->cvPayAvailable = cvPayAvailable;
	this->cvPayResponse = cvPayResponse;
	this->PaymentPriorityRequestQueue = PaymentPriorityRequestQueue;
	this->PaymentPriorityRequestQueueMutex = PaymentPriorityRequestQueueMutex;
	this->PrintMutex = PrintMutex;
}

PaymentGateway::~PaymentGateway()
{
}

void PaymentGateway::operator()()
{
	PaymentPriorityRequest PayRequest;

	std::mutex PaymentRequestAvailableMutex;

	std::unique_lock<std::mutex> PrintLock(*PrintMutex);
	PrintLock.unlock();

	while (true)
	{
		std::unique_lock<std::mutex> PaymentRequestAvailableLock(PaymentRequestAvailableMutex);
		cvPayAvailable->wait(PaymentRequestAvailableLock, [this] {return !PaymentPriorityRequestQueue->empty(); });

		std::unique_lock<std::mutex> PaymentPriorityRequestQueueLock(*PaymentPriorityRequestQueueMutex);
		PayRequest = PaymentPriorityRequestQueue->top();
		PaymentPriorityRequestQueue->pop();
		PaymentPriorityRequestQueueLock.unlock();

		std::this_thread::sleep_for(std::chrono::seconds(2));

		PrintLock.lock();
		std::cout << termcolor::white << "[PAYMENT GATEWAY] Received "<<std::to_string(PayRequest.getPaymentRequest().getMoney())<<" from " << PayRequest.getPaymentRequest().getOriginInfo() << termcolor::reset << std::endl;
		PrintLock.unlock();

		PayRequest.getPaymentRequest().notifyPaymentAccepted();
		cvPayResponse->notify_all();
	}
}
