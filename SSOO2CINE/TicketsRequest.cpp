#include "pch.h"
#include "TicketsRequest.h"


TicketsRequest::TicketsRequest()
{
}


TicketsRequest::TicketsRequest(int ClientId, unsigned char NumberOfSeats)
{
	this->ClientId = ClientId;
	this->NumberOfSeats = NumberOfSeats;
}

TicketsRequest::~TicketsRequest()
{
}

int TicketsRequest::getClientId()
{
	return ClientId;
}

int TicketsRequest::getNumberOfSeats()
{
	return NumberOfSeats;
}
