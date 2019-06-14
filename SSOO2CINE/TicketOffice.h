#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "TicketsRequest.h"
#include "Definitions.h"

class TicketOffice
{
public:
	TicketOffice(int OfficeId, std::queue<int> *ReadyTicketOffices, std::mutex *TicketOfficesOperationMutex, int *TakenSeats, std::mutex *SeatsOperationMutex);
	~TicketOffice();
	void operator()();
	void addRequest(TicketsRequest TicketRequest);

private:
	int OfficeId;

	std::queue<int> *ReadyTicketOffices;
	std::mutex *TicketOfficesOperationMutex;

	int *TakenSeats;
	std::mutex *SeatsOperationMutex;

	std::queue<TicketsRequest> TicketsRequests;
	std::mutex RequestsOperationMutex;

	std::mutex TicketOperationAvailableMutex;
	std::condition_variable cvTicketOperationAvailable;
};

