#include "pch.h"
#include "TicketOfficesScheduler.h"


TicketOfficesScheduler::TicketOfficesScheduler(std::vector<TicketOffice> *TicketOfficesVector, std::queue<int> *ReadyTicketOffices, std::queue<TicketsRequest> *TicketsRequestQueue, std::mutex *TicketOfficesOperationMutex, std::mutex *TicketsRequestQueueMutex)
{
	this->TicketOfficesVector = TicketOfficesVector;
	this->ReadyTicketOffices = ReadyTicketOffices;
	this->TicketsRequestQueue = TicketsRequestQueue;
	this->TicketOfficesOperationMutex = TicketOfficesOperationMutex;
	this->TicketsRequestQueueMutex = TicketsRequestQueueMutex;
}


TicketOfficesScheduler::~TicketOfficesScheduler()
{
}

void TicketOfficesScheduler::operator()()
{
	while (true)
	{
		std::unique_lock<std::mutex> AvailableTicketOperationLock(AvailableTicketOperationMutex);
		cvAvailableTicketOperation.wait(AvailableTicketOperationLock, [this] {return !TicketsRequestQueue->empty() && !ReadyTicketOffices->empty(); });

		std::unique_lock<std::mutex> TicketOfficesOperationLock(*TicketOfficesOperationMutex, std::defer_lock);
		std::unique_lock<std::mutex> TicketsRequestQueueLock(*TicketsRequestQueueMutex, std::defer_lock);
		std::lock(TicketOfficesOperationLock, TicketsRequestQueueLock);

		TicketOfficesVector->at(ReadyTicketOffices->front()).addRequest(TicketsRequestQueue->front());

		ReadyTicketOffices->pop();
		TicketsRequestQueue->pop();
	}
}
