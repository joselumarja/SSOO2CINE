#include "pch.h"
#include "Client.h"
#include "TicketsRequest.h"

TicketsRequest::TicketsRequest()
{
}

TicketsRequest::TicketsRequest(unsigned short NumberOfSeats)
{
	this->NumberOfSeats = NumberOfSeats;
}

TicketsRequest::~TicketsRequest()
{
}

void TicketsRequest::setClientPointer(Client * client)
{
	this->ClientId = client;
}

void TicketsRequest::AcceptOperation()
{
	ClientId->acceptTicketRequest();
}

void TicketsRequest::DenyOperation()
{
	ClientId->denyTicketRequest();
}

std::string TicketsRequest::getClientInfo()
{
	return std::to_string(ClientId->getClientId());
}
