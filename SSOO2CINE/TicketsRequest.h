#pragma once

class Client;

class TicketsRequest
{
public:
	TicketsRequest();
	TicketsRequest(Client *ClientId, unsigned short NumberOfSeats);
	~TicketsRequest();
	inline short getNumberOfSeats()
	{
		return NumberOfSeats;
	}

	void AcceptOperation();
	void DenyOperation();

private:
	Client *ClientId;
	unsigned short NumberOfSeats;

};

