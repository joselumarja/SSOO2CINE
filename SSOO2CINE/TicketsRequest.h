#pragma once

class Client;

class TicketsRequest
{
public:
	TicketsRequest();
	TicketsRequest(Client *ClientId, unsigned char NumberOfSeats);
	~TicketsRequest();
	inline int getNumberOfSeats()
	{
		return NumberOfSeats;
	}

	void AcceptOperation();
	void DenyOperation();

private:
	Client *ClientId;
	unsigned char NumberOfSeats;

};

