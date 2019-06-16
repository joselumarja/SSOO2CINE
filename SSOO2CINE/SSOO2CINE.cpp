// SSOO2CINE.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <queue>
#include <math.h>
#include <vector>
#include <mutex>
#include <thread>
#include <string>

#include "PaymentRequest.h"
#include "TicketsRequest.h"
#include "PaymentRequest.h"
#include "PaymentPriorityRequest.h"
#include "FoodAndDrinkRequest.h"
#include "ReplenishmentRequest.h"

#include "PrintColorText.h"
#include "Stand.h"
#include "TicketOffice.h"
#include "Replenishment.h"


std::vector<std::thread> ClientsVector;
std::vector<Stand> StandsVector;
std::vector<TicketOffice> TicketOfficesVector;
std::vector<Replenishment> ReplenishmentVector;

std::mutex StandsOperationMutex;
std::mutex TicketOfficesOperationMutex;
std::mutex ReplenishmentOperationMutex;

std::queue<int> ReadyTicketOffices;
std::queue<int> ReadyFoodStands;
std::queue<int> ReadyReplenishment;

std::queue<TicketsRequest> TicketsRequestQueue;
std::queue<FoodAndDrinkRequest> FoodAndDrinkRequestQueue;
std::queue<ReplenishmentRequest> ReplenishmentRequestQueue;
std::queue<PaymentRequest> PaymentRequestQueue;

std::mutex TicketsRequestQueueMutex;
std::mutex FoodAndDrinkRequestQueueMutex;
std::mutex ReplenishmentRequestQueueMutex;
std::mutex PaymentRequestQueueMutex;

std::priority_queue<PaymentPriorityRequest> PaymentPriorityRequestQueue;

std::mutex PaymentPriorityRequestQueueMutex;

std::mutex SeatsOperationMutex;
int TakenSeats = 0;
float Earnings = 0.f;

unsigned int NumberOfThreads = std::thread::hardware_concurrency();

PrintColorText PColor;

/*Client thread generation*/
//void randomGenerateClients();

int main()
{	
	return EXIT_SUCCESS;
}

/*void TicketOffice()
{
	while (true)
	{
		EntranceToCinemaCount.wait();
		TicketTurn.unlock();
		TicketRequestReady.lock();
		PayTurn.lock();
		if (TakenSeats + TicketRequestBuffer.getNumberOfSeats() <= NUMBER_OF_SEATS)
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			TakenSeats += TicketRequestBuffer.getNumberOfSeats();
			PaymentRequestBuffer=PaymentRequest(RequestOrigin::TicketOffice, (float) TicketRequestBuffer.getNumberOfSeats()*TICKET_PRICE);
			PaymentRequestReady.unlock();
			TicketResolution = true;
			PaymentReady.lock();
		}
		else
		{
			TicketResolution = false;
		}
		PayTurn.unlock();
		GetTicketResolution.unlock();
	}
}

void FoodAndDrinkStands()
{
	while (true)
	{
		EntranceToFoodStandCount.wait();
		FreeFoodStand.wait();

		FoodStandsUpdate.lock();
		for (int i = 0; i < NUMBER_OF_STANDS; i++)
		{
			if (FoodStands[i])
			{
				NumberOfStandBuffer = i;
				FoodStands[i] = false;
				break;
			}
		}

		StandRequestReady.unlock();
		FoodStandsUpdate.unlock();
	}
}

void replenishment()
{
	while (true)
	{
		ReplenishmentCount.wait();
		ReplenishmentTurn.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(10));
		PColor.printColorText("Replenish products in Stand: " + std::to_string(NumberOfStandToReplenish) + "\n", ColorText::RedTextColor);
		StandsStock[NumberOfStandToReplenish].Drinks = MAX_DRINKS_AMOUNT;
		StandsStock[NumberOfStandToReplenish].Popcorn = MAX_POPCORN_AMOUNT;
		ReplenishReady.unlock();
	}
}

void Stand(int Id)
{
	while (true)
	{
		StandLock[Id].lock();
		if (FoodAndDrinkRequestBuffer[Id].getNumberOfPopcorns() > StandsStock[Id].Popcorn || FoodAndDrinkRequestBuffer[Id].getNumberOfDrinks() > StandsStock[Id].Drinks)
		{
			ReplenishmentCount.notify();
			ReplenishmentTurn.lock();
			NumberOfStandToReplenish = Id;
			ReplenishReady.lock();
		}

		std::this_thread::sleep_for(std::chrono::seconds(FoodAndDrinkRequestBuffer[Id].getNumberOfDrinks() + FoodAndDrinkRequestBuffer[Id].getNumberOfPopcorns()));

		PayTurn.lock();
	
		PaymentRequestBuffer = PaymentRequest(RequestOrigin::FoodAndDrink, Id,(float) (FoodAndDrinkRequestBuffer[Id].getNumberOfDrinks()*DRINK_PRICE)+(FoodAndDrinkRequestBuffer[Id].getNumberOfPopcorns()*POPCORN_PRICE));
		PaymentRequestReady.unlock();

		StandsStock[Id].Drinks -= FoodAndDrinkRequestBuffer[Id].getNumberOfDrinks();
		StandsStock[Id].Popcorn -= FoodAndDrinkRequestBuffer[Id].getNumberOfPopcorns();

		PaymentReady.lock();
		PayTurn.unlock();

		FoodStandsUpdate.lock();
		FoodStands[Id] = true;
		FreeFoodStand.notify();
		FoodStandsUpdate.unlock();

	}
}

void PaymentGateway()
{
	while (true)
	{
		PaymentRequestReady.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		Earnings+=PaymentRequestBuffer.getMoney();
		if (PaymentRequestBuffer.getOrigin() == RequestOrigin::TicketOffice)
		{
			PColor.printColorText("Earning: " + std::to_string(PaymentRequestBuffer.getMoney()) + " from ticket office\n", ColorText::WhiteTextColor);
		}
		else
		{
			PColor.printColorText("Earning: " + std::to_string(PaymentRequestBuffer.getMoney()) + " from Stand "+std::to_string(PaymentRequestBuffer.getStandNumber())+"\n", ColorText::WhiteTextColor);
		}
		PaymentReady.unlock();
	}

}
void randomGenerateClients() 
{
	for (int i = 0; i < MAX_NUMBER_OF_CLIENTS; i++)
	{
		short NumberOfPersons = (rand() % 10)+1;
		ClientsVector.push_back(std::thread(Client, i, TicketsRequest(i, NumberOfPersons),FoodAndDrinkRequest(NumberOfPersons,NumberOfPersons)));
		std::this_thread::sleep_for(std::chrono::seconds(rand()%3));
		EntranceToCinemaCount.notify();
	}
}

void Client(int ClientId,TicketsRequest TicketRequest,FoodAndDrinkRequest FoodDrinkRequest)
{
	PColor.printColorText("Client: " + std::to_string(ClientId) + " waiting in ticket queue\n", ColorText::YellowTextColor);
	TicketTurn.lock();

	PColor.printColorText("Client: " + std::to_string(ClientId) + " buying tickets: "+std::to_string(TicketRequest.getNumberOfSeats())+"\n", ColorText::YellowTextColor);
	TicketRequestBuffer = TicketRequest;
	TicketRequestReady.unlock();
	GetTicketResolution.lock();
	if (!TicketResolution)
	{
		PColor.printColorText("Client: " + std::to_string(ClientId) + " invalid request, not enought tickets\n", ColorText::RedTextColor);
		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds(rand() % 4));
	PColor.printColorText("Client: " + std::to_string(ClientId) + " waiting in food queue\n", ColorText::LightBlueTextColor);

	EntranceToFoodStandCount.notify();
	StandRequestReady.lock();

	unsigned char NumberOfStand = NumberOfStandBuffer;

	PColor.printColorText("Client: " + std::to_string(ClientId) + " Food Stand "+std::to_string(NumberOfStandBuffer)+" Popcorn: "+std::to_string(FoodDrinkRequest.getNumberOfPopcorns())+" Drinks: "+std::to_string(FoodDrinkRequest.getNumberOfDrinks())+"\n", ColorText::LightBlueTextColor);
	FoodAndDrinkRequestBuffer[NumberOfStand] = FoodDrinkRequest;
	StandLock[NumberOfStand].unlock();
	PColor.printColorText("Client: " + std::to_string(ClientId) + " inside cinema\n", ColorText::GreenTextColor);

}
*/
// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
