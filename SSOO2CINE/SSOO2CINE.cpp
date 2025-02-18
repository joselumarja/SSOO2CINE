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
#include <condition_variable>

#include "PaymentRequest.h"
#include "TicketsRequest.h"
#include "PaymentRequest.h"
#include "PaymentPriorityRequest.h"
#include "FoodAndDrinkRequest.h"
#include "ReplenishmentRequest.h"

#include "Client.h"
#include "Stand.h"
#include "TicketOffice.h"
#include "Replenishment.h"
#include "PaymentGateway.h"
#include "PaymentScheduler.h"

#include "termcolor.h"

std::vector<std::thread> ClientsVector;

std::vector<std::thread> TicketOfficesVectorThreads;
std::vector<std::thread> StandsVectorThreads;
std::vector<std::thread> ReplenishmentVectorThreads;
std::thread PaymentGatewayThread;
std::thread PaymentSchedulerThread;

std::condition_variable cvTicketOfficeTurn;
std::condition_variable cvTicketOfficeResponse;

std::condition_variable cvFoodStandTurn;
std::condition_variable cvFoodStandResponse;

std::condition_variable cvReplenishmentTurn;
std::condition_variable cvReplenishmentResponse;

std::condition_variable cvPayTurn;
std::condition_variable cvPayAvailable;
std::condition_variable cvPayResponse;

std::vector<Stand> StandsVector;
std::vector<TicketOffice> TicketOfficesVector;
std::vector<Replenishment> ReplenishmentVector;

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

std::mutex PrintMutex;

/*Client thread generation*/
void randomGenerateClients();

void waitClientsFinalization();

void parseArgv(int argc, char *argv[], unsigned short &NumberOfTicketOffices, unsigned short &NumberOfStands, unsigned short &NumberOfReplenishment);
void checkArgumentValues(unsigned short &NumberOfTicketOffices, unsigned short &NumberOfStands, unsigned short &NumberOfReplenishment);
void initializeDataStructures();

void launchServices(unsigned short &NumberOfTicketOffices, unsigned short &NumberOfStands, unsigned short &NumberOfReplenishment);

int main(int argc, char *argv[])
{	
	unsigned short NumberOfTicketOffices, NumberOfStands, NumberOfReplenishment;

	parseArgv(argc, argv, NumberOfTicketOffices, NumberOfStands, NumberOfReplenishment);
	checkArgumentValues(NumberOfTicketOffices, NumberOfStands, NumberOfReplenishment);
	initializeDataStructures();

	launchServices(NumberOfTicketOffices, NumberOfStands, NumberOfReplenishment);

	randomGenerateClients();
	waitClientsFinalization();

	std::cout << termcolor::white << "[LAUNCHER] Total taken seats: " << std::to_string(TakenSeats) << termcolor::reset << std::endl;

	return EXIT_SUCCESS;
}
void parseArgv(int argc, char *argv[], unsigned short &NumberOfTicketOffices, unsigned short &NumberOfStands, unsigned short &NumberOfReplenishment)
{
	if (argc != 4)
	{
		std::cerr << termcolor::red << "[LAUNCHER] Fail, Wrong Argumnet Number" << termcolor::reset << std::endl;
		exit(EXIT_FAILURE);
	}

	NumberOfTicketOffices = std::atoi(argv[1]);
	NumberOfStands = std::atoi(argv[2]);
	NumberOfReplenishment = std::atoi(argv[3]);
}
void checkArgumentValues(unsigned short &NumberOfTicketOffices, unsigned short &NumberOfStands, unsigned short &NumberOfReplenishment)
{
	unsigned short CPUThreads = std::thread::hardware_concurrency();

	if (NumberOfTicketOffices > CPUThreads)
	{
		std::cout << termcolor::yellow << "[LAUNCHER] " << std::to_string(NumberOfTicketOffices) << " ticket offices is greather than the number of cores" << termcolor::reset << std::endl;
		NumberOfTicketOffices = CPUThreads;
	}

	if (NumberOfStands > CPUThreads)
	{
		std::cout << termcolor::yellow << "[LAUNCHER] " << std::to_string(NumberOfStands) << " stands is greather than the number of cores" << termcolor::reset << std::endl;
		NumberOfStands = CPUThreads;
	}

	if (NumberOfReplenishment > CPUThreads)
	{
		std::cout << termcolor::yellow <<"[LAUNCHER] "<< std::to_string(NumberOfReplenishment) << " replenishment is greather than the number of cores" << termcolor::reset << std::endl;
		NumberOfReplenishment = CPUThreads;
	}

}

void initializeDataStructures()
{
	 ClientsVector= std::vector<std::thread>();

	TicketOfficesVectorThreads= std::vector<std::thread>();
	StandsVectorThreads= std::vector<std::thread>();
	ReplenishmentVectorThreads= std::vector<std::thread>();

	StandsVector= std::vector<Stand>();
	TicketOfficesVector= std::vector<TicketOffice>();
	ReplenishmentVector=std::vector<Replenishment>();

	TicketsRequestQueue= std::queue<TicketsRequest>();
	FoodAndDrinkRequestQueue= std::queue<FoodAndDrinkRequest>();
	ReplenishmentRequestQueue= std::queue<ReplenishmentRequest>();
	PaymentRequestQueue= std::queue<PaymentRequest>();

	PaymentPriorityRequestQueue= std::priority_queue<PaymentPriorityRequest>();
}

void launchServices(unsigned short &NumberOfTicketOffices, unsigned short &NumberOfStands, unsigned short &NumberOfReplenishment)
{
	std::cout << termcolor::white << "[LAUNCHER] Launching payment gateway" << termcolor::reset << std::endl;
	PaymentGatewayThread=std::thread(PaymentGateway(&cvPayAvailable,&cvPayResponse,&PaymentPriorityRequestQueue,&PaymentPriorityRequestQueueMutex,&PrintMutex));
	PaymentSchedulerThread = std::thread(PaymentScheduler(&cvPayTurn, &cvPayAvailable, &PaymentRequestQueue, &PaymentPriorityRequestQueue, &PaymentPriorityRequestQueueMutex, &PaymentPriorityRequestQueueMutex));

	std::cout << termcolor::white <<"[LAUNCHER] Launching "<< std::to_string(NumberOfReplenishment) << " replenishment threads" << termcolor::reset << std::endl;
	for (int i = 0; i < NumberOfReplenishment; i++)
	{
		Replenishment replenishment(i,&cvReplenishmentTurn,&cvReplenishmentResponse,&ReplenishmentRequestQueue,&ReplenishmentRequestQueueMutex,&PrintMutex);
		ReplenishmentVector.push_back(replenishment);
		ReplenishmentVectorThreads.push_back(std::thread(replenishment));
	}

	std::cout << termcolor::white << "[LAUNCHER] Launching " << std::to_string(NumberOfStands) << " stand threads" << termcolor::reset << std::endl;
	for (int i = 0; i < NumberOfStands; i++)
	{
		Stand stand(i,&cvFoodStandTurn,&cvFoodStandResponse,&cvReplenishmentTurn,&cvReplenishmentResponse,&cvPayTurn,&cvPayResponse,&FoodAndDrinkRequestQueue,&FoodAndDrinkRequestQueueMutex,&ReplenishmentRequestQueue,&ReplenishmentRequestQueueMutex,&PaymentRequestQueue,&PaymentRequestQueueMutex,&PrintMutex);
		StandsVector.push_back(stand);
		StandsVectorThreads.push_back(std::thread(stand));
	}

	std::cout << termcolor::white << "[LAUNCHER] Launching " << std::to_string(NumberOfTicketOffices) << " ticket offices threads" << termcolor::reset << std::endl;
	for (int i = 0; i < NumberOfTicketOffices; i++)
	{
		TicketOffice TOffice(i,&cvTicketOfficeTurn,&cvTicketOfficeResponse,&cvPayTurn,&cvPayResponse,&TicketsRequestQueue,&TicketsRequestQueueMutex,&PaymentRequestQueue,&PaymentRequestQueueMutex,&TakenSeats,&SeatsOperationMutex,&PrintMutex);
		TicketOfficesVector.push_back(TOffice);
		TicketOfficesVectorThreads.push_back(std::thread(TOffice));
	}
}

void randomGenerateClients() 
{
	for (int i = 0; i < NUMBER_OF_CLIENTS; i++)
	{
		unsigned short NumberOfPersons = (rand() % 10)+1;
		TicketsRequest TRequest(NumberOfPersons);
		FoodAndDrinkRequest FAndDRequest(NumberOfPersons, NumberOfPersons);
		Client client(i,TRequest,FAndDRequest, &cvTicketOfficeTurn, &cvTicketOfficeResponse, &cvFoodStandTurn, &cvFoodStandResponse, &TicketsRequestQueue, &FoodAndDrinkRequestQueue, &TicketsRequestQueueMutex, &FoodAndDrinkRequestQueueMutex, &PrintMutex);
		ClientsVector.push_back(std::thread(client));
		std::this_thread::sleep_for(std::chrono::seconds(rand()%3));
	}
}

void waitClientsFinalization()
{
	for (unsigned int i = 0; i < ClientsVector.size(); i++)
	{
		if (ClientsVector.at(i).joinable())
		{
			ClientsVector.at(i).join();
		}
	}
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
