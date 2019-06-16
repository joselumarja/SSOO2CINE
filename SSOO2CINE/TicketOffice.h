#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

#include "TicketsRequest.h"
#include "Definitions.h"
#include "TradeNode.h"
#include "PaymentRequest.h"

class TicketOffice:public TradeNode
{
public:
	TicketOffice(int OfficeId, std::queue<int> *ReadyTicketOffices, std::mutex *TicketOfficesOperationMutex, int *TakenSeats, std::mutex *SeatsOperationMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex);
	~TicketOffice();
	void operator()();

	void addRequest(TicketsRequest TicketRequest);

	void PayAccomplished();

private:
	int OfficeId;

	std::queue<int> *ReadyTicketOffices;
	std::mutex *TicketOfficesOperationMutex;

	int *TakenSeats;
	std::mutex *SeatsOperationMutex;

	std::queue<PaymentRequest> *PaymentRequestQueue;
	std::mutex *PaymentRequestQueueMutex;

	std::queue<TicketsRequest> TicketsRequests;

	bool PaymentAccomplished;
};

