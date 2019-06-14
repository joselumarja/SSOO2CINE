#include "pch.h"
#include "TicketOffice.h"

TicketOffice::TicketOffice(int OfficeId, std::queue<int> *ReadyTicketOffices, std::mutex *TicketOfficesOperationMutex, int *TakenSeats, std::mutex *SeatsOperationMutex)
{
	this->OfficeId = OfficeId;
	this->ReadyTicketOffices = ReadyTicketOffices;
	this->TicketOfficesOperationMutex = TicketOfficesOperationMutex;
	this->TakenSeats = TakenSeats;
	this->SeatsOperationMutex = SeatsOperationMutex;
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
			//A PAGAR
			Request.AcceptOperation();
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
