#include "pch.h"
#include "Client.h"
#include "TicketsRequest.h"

TicketsRequest::TicketsRequest()
{
}

TicketsRequest::TicketsRequest(Client *ClientId, unsigned char NumberOfSeats)
{
	this->ClientId = ClientId;
	this->NumberOfSeats = NumberOfSeats;
}

TicketsRequest::~TicketsRequest()
{
}

void TicketsRequest::AcceptOperation()
{
	ClientId->acceptTicketRequest();
}

void TicketsRequest::DenyOperation()
{
	ClientId->denyTicketRequest();
}
