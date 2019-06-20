#include "pch.h"
#include "TicketOffice.h"
#include "termcolor.h"

TicketOffice::TicketOffice(int OfficeId, std::condition_variable *cvTicketOfficeTurn, std::condition_variable *cvTicketOfficeResponse, std::condition_variable *cvPayTurn, std::condition_variable *cvPayResponse, std::queue<TicketsRequest> *TicketsRequestQueue, std::mutex *TicketsRequestQueueMutex, std::queue<PaymentRequest> *PaymentRequestQueue, std::mutex *PaymentRequestQueueMutex, int *TakenSeats, std::mutex *SeatsOperationMutex, std::mutex *PrintMutex)
{
	this->OfficeId = OfficeId;
	this->cvTicketOfficeTurn = cvTicketOfficeTurn;
	this->cvTicketOfficeResponse = cvTicketOfficeResponse;
	this->cvPayTurn = cvPayTurn;
	this->cvPayResponse = cvPayResponse;
	this->TicketsRequestQueue = TicketsRequestQueue;
	this->TicketsRequestQueueMutex = TicketsRequestQueueMutex;
	this->TakenSeats = TakenSeats;
	this->SeatsOperationMutex = SeatsOperationMutex;
	this->PaymentRequestQueue = PaymentRequestQueue;
	this->PaymentRequestQueueMutex = PaymentRequestQueueMutex;
	this->PrintMutex = PrintMutex;
	PaymentAccomplished = false;
}


TicketOffice::~TicketOffice()
{
}

void TicketOffice::operator()()
{
	TicketsRequest Request;

	std::mutex TicketOperationAvailableMutex;
	std::mutex PaymentAccomplishedMutex;

	std::unique_lock<std::mutex> PrintLock(*PrintMutex);
	PrintLock.unlock();

	while (true)
	{
		std::unique_lock<std::mutex> TicketOperationAvailableLock(TicketOperationAvailableMutex);
		cvTicketOfficeTurn->wait(TicketOperationAvailableLock, [this] {return !TicketsRequestQueue->empty(); });

		std::unique_lock<std::mutex> TicketsRequestQueueLock(*TicketsRequestQueueMutex);
		Request = TicketsRequestQueue->front();
		TicketsRequestQueue->pop();
		TicketsRequestQueueLock.unlock();

		PrintLock.lock();
		std::cout << termcolor::magenta << "[TICKET OFFICE " << std::to_string(OfficeId) << "] Attending Client: " << Request.getClientInfo() << termcolor::reset << std::endl;
		PrintLock.unlock();

		std::unique_lock<std::mutex> lkSeats(*SeatsOperationMutex);
		if (Request.getNumberOfSeats() + *TakenSeats > NUMBER_OF_SEATS)
		{
			PrintLock.lock();
			std::cout << termcolor::red << "[TICKET OFFICE " << std::to_string(OfficeId) << "] Not Enought Tickets Client: " << Request.getClientInfo() << termcolor::reset << std::endl;
			PrintLock.unlock();

			Request.DenyOperation();
		}
		else
		{
			PaymentRequest PayRequest = PaymentRequest(RequestOrigin::TicketOffice, (TradeNode*)this, Request.getNumberOfSeats()*TICKET_PRICE);

			PrintLock.lock();
			std::cout << termcolor::magenta << "[TICKET OFFICE " << std::to_string(OfficeId) << "] Sending Payment Request" << termcolor::reset << std::endl;
			PrintLock.unlock();

			std::unique_lock<std::mutex> PaymentRequestQueueLock(*PaymentRequestQueueMutex);
			PaymentRequestQueue->push(PayRequest);
			PaymentRequestQueueLock.unlock();

			cvPayTurn->notify_one();

			std::unique_lock<std::mutex> PaymentAccomplishedLock(PaymentAccomplishedMutex);
			cvPayResponse->wait(PaymentAccomplishedLock, [this] {return PaymentAccomplished; });

			PrintLock.lock();
			std::cout << termcolor::magenta << "[TICKET OFFICE " << std::to_string(OfficeId) << "] Client: " << Request.getClientInfo() << " Attended" << termcolor::reset << std::endl;
			PrintLock.unlock();

			Request.AcceptOperation();
			PaymentAccomplished = false;
		}
		lkSeats.unlock();

		cvTicketOfficeResponse->notify_all();

	}
}

std::string TicketOffice::getInfo()
{
	return std::string("Ticket Office "+std::to_string(OfficeId));
}

void TicketOffice::PayAccomplished()
{
	PaymentAccomplished = true;
}
