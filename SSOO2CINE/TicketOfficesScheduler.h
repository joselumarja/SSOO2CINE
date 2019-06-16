#pragma once
#include <queue>
#include <condition_variable>
#include "TicketOffice.h"
#include "TicketsRequest.h"

class TicketOfficesScheduler
{
public:
	TicketOfficesScheduler(std::vector<TicketOffice> *TicketOfficesVector, std::queue<int> *ReadyTicketOffices, std::queue<TicketsRequest> *TicketsRequestQueue, std::mutex *TicketOfficesOperationMutex, std::mutex *TicketsRequestQueueMutex);
	~TicketOfficesScheduler();
	void operator()();

private:
	std::queue<int> *ReadyTicketOffices;
	std::vector<TicketOffice> *TicketOfficesVector;
	std::queue<TicketsRequest> *TicketsRequestQueue;

	std::mutex *TicketOfficesOperationMutex;
	std::mutex *TicketsRequestQueueMutex;

};

