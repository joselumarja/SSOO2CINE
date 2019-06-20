#pragma once
#include <string>

class TradeNode
{
public:
	TradeNode();
	~TradeNode();
	virtual void PayAccomplished() = 0;
	virtual std::string getInfo() = 0;
};

