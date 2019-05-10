#pragma once
#include "Definitions.h"

class TicketsRequest
{
public:
	TicketsRequest();
	TicketsRequest(int ClientId, unsigned char NumberOfSeats);
	~TicketsRequest();
	int getClientId();
	int getNumberOfSeats();

private:
	int ClientId;
	unsigned char NumberOfSeats;

};

