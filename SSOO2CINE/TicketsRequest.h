#pragma once
#include <string>

class Client;

class TicketsRequest
{
public:
	TicketsRequest();
	TicketsRequest(unsigned short NumberOfSeats);
	~TicketsRequest();
	inline short getNumberOfSeats()
	{
		return NumberOfSeats;
	}

	void setClientPointer(Client *client);

	void AcceptOperation();
	void DenyOperation();

	std::string getClientInfo();

private:
	Client *ClientId;
	unsigned short NumberOfSeats;

};

