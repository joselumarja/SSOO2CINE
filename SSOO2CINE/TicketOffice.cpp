#include "pch.h"
#include "TicketOffice.h"

TicketOffice::TicketOffice(int OfficeId, std::queue<int> *ReadyTicketOffices, std::mutex *TicketOfficesOperationMutex, int *TakenSeats, std::mutex *SeatsOperationMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex)
{
	this->OfficeId = OfficeId;
	this->ReadyTicketOffices = ReadyTicketOffices;
	this->TicketOfficesOperationMutex = TicketOfficesOperationMutex;
	this->TakenSeats = TakenSeats;
	this->SeatsOperationMutex = SeatsOperationMutex;
	this->PaymentRequestQueue = PaymentRequestQueue;
	this->PaymentRequestQueueMutex = PaymentRequestQueueMutex;
	PaymentAccomplished = false;
}


TicketOffice::~TicketOffice()
{
}

void TicketOffice::operator()()
{
	TicketsRequest Request;

	while (true)
	{
		std::unique_lock<std::mutex> TicketOperationAvailableLock(TicketOperationAvailableMutex);
		cvTicketOperationAvailable.wait(TicketOperationAvailableLock, [this] {return !TicketsRequests.empty(); });

		std::lock_guard<std::mutex> lkRequests(RequestsOperationMutex);
		Request = TicketsRequests.front();
		TicketsRequests.pop();
		lkRequests.~lock_guard();

		std::lock_guard<std::mutex> lkSeats(*SeatsOperationMutex);
		if (Request.getNumberOfSeats() + *TakenSeats > NUMBER_OF_SEATS)
		{
			Request.DenyOperation();
		}
		else
		{
			PaymentRequest PayRequest = PaymentRequest(RequestOrigin::TicketOffice, (TradeNode*)this, Request.getNumberOfSeats()*TICKET_PRICE);
			std::lock_guard<std::mutex> PaymentRequestQueueLock(*PaymentRequestQueueMutex);
			PaymentRequestQueue->push(PayRequest);
			PaymentRequestQueueLock.~lock_guard();

			std::unique_lock<std::mutex> PaymentAccomplishedLock(PaymentAccomplishedMutex);
			cvPaymentAccomplished.wait(PaymentAccomplishedLock, [this] {return PaymentAccomplished; });
			Request.AcceptOperation();
			PaymentAccomplished = false;
		}
		lkSeats.~lock_guard();

		std::lock_guard<std::mutex> TicketOfficesOperationLock(*TicketOfficesOperationMutex);
		ReadyTicketOffices->push(OfficeId);

	}
}

void TicketOffice::addRequest(TicketsRequest TicketRequest)
{
	std::lock_guard<std::mutex> lk(RequestsOperationMutex);
	TicketsRequests.push(TicketRequest);
}

void TicketOffice::PayAcomplished()
{
	PaymentAccomplished = true;
}
