#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

#include "TicketsRequest.h"
#include "Definitions.h"
#include "TradeNode.h"
#include "PaymentRequest.h"

class TicketOffice:public TradeNode
{
public:
	TicketOffice(int OfficeId, std::condition_variable *cvTicketOfficeTurn, std::condition_variable *cvTicketOfficeResponse, std::condition_variable *cvPayTurn, std::condition_variable *cvPayResponse, std::queue<TicketsRequest> *TicketsRequestQueue, std::mutex *TicketsRequestQueueMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex, int *TakenSeats, std::mutex *SeatsOperationMutex, std::mutex *PrintMutex);
	~TicketOffice();
	void operator()();

	std::string getInfo();

	void PayAccomplished();

private:
	int OfficeId;

	std::condition_variable *cvTicketOfficeTurn;
	std::condition_variable *cvTicketOfficeResponse;

	std::condition_variable *cvPayTurn;
	std::condition_variable *cvPayResponse;

	std::queue<TicketsRequest> *TicketsRequestQueue;
	std::mutex *TicketsRequestQueueMutex;

	std::queue<PaymentRequest> *PaymentRequestQueue;
	std::mutex *PaymentRequestQueueMutex;

	int *TakenSeats;
	std::mutex *SeatsOperationMutex;

	std::mutex *PrintMutex;

	bool PaymentAccomplished;
};

