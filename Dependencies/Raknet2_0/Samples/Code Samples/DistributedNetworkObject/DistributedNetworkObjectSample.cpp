// ----------------------------------------------------------------------
// RakNet version 2.0+ (registered)
// DistributedNetworkObject.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) December 8, 2003
// Shows how to implement distributed network objects.  See "DISTRIBUTED OBJECT USER NOTE" in this file for examples
// All the classes and data here are nonsense but illustrate the syntax
// ----------------------------------------------------------------------

#include "Multiplayer.h"
// *** DISTRIBUTED OBJECT USER NOTE #1 ***
// The distributed object system requires "DistributedNetworkObjectHeader.h"
#include "DistributedNetworkObjectHeader.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "RakNetworkFactory.h"
#include "BitStream.h"
#include "Rand.h"
#include <conio.h> // For kbhit
#include <stdlib.h> // atoi
#include <string.h> // strcpy

class Apple;
class Orange;
class Barrel;

Apple *apple;
Orange * orange;
Barrel *barrel;

// *** DISTRIBUTED OBJECT USER NOTE #2 ***
// The basemost class has to derive from DistributedNetworkObject
class Fruit : public DistributedNetworkObject
{
	public:
		struct FruitName
		{
			char name[20];
		} ;

		FruitName fruitName;
		int numberOfGrams;

		Fruit()
		{
			fruitName.name[0]=0;
			numberOfGrams=100;
			// See the header file for DistributedNetworkObject for docs on this.  It is easy to use.
			SynchronizeMemory(0, (char*)&numberOfGrams, sizeof(numberOfGrams), false);
			SynchronizeMemory(1, (char*)&fruitName, sizeof(FruitName), false);
		}

		// Overload this to show a message
		bool OnDistributedObjectCreation(PlayerID senderID)
		{
			printf("Object Created\n");
			return true; // Allow object creation by returning true
		}

		// Overload this to show a message
		void OnDistributedObjectDestruction(PlayerID senderID)
		{
			printf("Object Destroyed\n");
			delete this;
		}
/*
	
	// *** DISTRIBUTED OBJECT USER NOTE #3 ***
	// This is the general format of synchronizing data elements.  Not all data elements have to be synchronized
	// You pass the base class, the copy type, whether the server or the client is the authority, the type of the data member,
	// and the name of the data member.  Because of the way the defines work out we have to enclose the array in a struct rather than
	// just passing an array.  This will probably be fixed in a later version.
	REGISTER_1_DISTRIBUTED_OBJECT_MEMBERS(DistributedNetworkObject,
		DOM_COPY_UNCOMPRESSED, DOM_CLIENT_AUTHORITATIVE, FruitName, fruitName)
		*/
};

// *** DISTRIBUTED OBJECT USER NOTE #4 ***
// We don't have to call REGISTER_DISTRIBUTED_CLASS for class Fruit because we never instantiate it

class Apple : public Fruit
{
public:
	Apple() {printf("Apple created\n");	apple=this;
	SynchronizeMemory(2, (char*)&color, sizeof(color), true);
	}
	~Apple() {printf("Apple deleted\n"); apple=0;	}

	/*
	// *** DISTRIBUTED OBJECT USER NOTE #5 ***
	// Since color only takes low values (0 and 1), we can send it compressed.
	// Note also that we are making this variable server authoritative
	// This macro always has to go in the public section of the class
	// Notice we always pass the base class of this class to the first argument
	REGISTER_1_DISTRIBUTED_OBJECT_MEMBERS(Fruit,
		DOM_COPY_COMPRESSED, DOM_SERVER_AUTHORITATIVE, char, color)
		*/

	char color;
};

class Orange : public Fruit
{
public:
	Orange() {printf("Orange created\n"); orange=this;
	SynchronizeMemory(3, (char*)&radius, sizeof(radius), true);
	SynchronizeMemory(4, (char*)&sweetness, sizeof(sweetness), true);
	}
	~Orange() {printf("Orange deleted\n"); orange=0;	}

	float radius;
	int sweetness;

	/*
	// *** DISTRIBUTED OBJECT USER NOTE #6 ***
	// Here we synch 2 members.  Both of them do automatic interpolation.
	// Automatic interpolation requires the +, -, *, and = operators to be defined
	REGISTER_2_DISTRIBUTED_OBJECT_MEMBERS(Fruit,
		DOM_INTERPOLATE_UNCOMPRESSED, DOM_SERVER_AUTHORITATIVE, float, radius,
		DOM_INTERPOLATE_UNCOMPRESSED, DOM_SERVER_AUTHORITATIVE, int, sweetness	)
		*/
};

class Barrel : public DistributedNetworkObject
{
public:
	Barrel() {printf("Barrel created\n"); barrel=this;}
	~Barrel() {printf("Barrel deleted\n"); barrel=0;	}
	Orange orange;
	Apple apple;
};

int main(void)
{
	RakClientInterface *rakClient=RakNetworkFactory::GetRakClientInterface();
	RakServerInterface *rakServer=RakNetworkFactory::GetRakServerInterface();
	Multiplayer<RakClientInterface> rakClientMultiplayer;
	Multiplayer<RakServerInterface> rakServerMultiplayer;
	bool isServer;

	REGISTER_DISTRIBUTED_CLASS(Apple);
	REGISTER_DISTRIBUTED_CLASS(Orange);
	REGISTER_DISTRIBUTED_CLASS(Barrel);


	apple=0;
	barrel=0;
	orange=0;

	// Crude interface
	puts("Run as (1) Client or (2) Server ?");
	if (getch()=='1')
	{
		// Holds user data
		char ip[30], serverPort[30], clientPort[30];
		// A client
		isServer=false;

		// Get our input
		puts("Enter the client port to listen on");
		gets(clientPort);
		if (clientPort[0]==0)
			strcpy(clientPort, "0");
		puts("Enter IP to connect to");
		gets(ip);
		if (ip[0]==0)
			strcpy(ip, "127.0.0.1");
		puts("Enter the port to connect to");
		gets(serverPort);
		if (serverPort[0]==0)
			strcpy(serverPort, "60000");

		// Connecting the client is very simple.  0 means we don't care about
		// a connectionValidationInteger, and false for low priority threads
		bool b = rakClient->Connect(ip, atoi(serverPort), atoi(clientPort), 0, 30);
		
		if (b)
			puts("Attempting connection");
		else
		{
			puts("Bad connection attempt.  Terminating.");
			exit(1);
		}
		
		// *** DISTRIBUTED OBJECT USER NOTE #7 ***
		// To use distributed objects we have to register our network instance with the distributed object manager
		DistributedNetworkObjectManager::Instance()->RegisterRakClientInterface(rakClient);
	}
	else
	{
		// Holds user data
		char portstring[30];

		// A server
		isServer=true;
		puts("Enter the server port to listen on");
		gets(portstring);
		if (portstring[0]==0)
			strcpy(portstring, "60000");
		puts("Starting server.");
		// Starting the server is very simple.  2 players allowed.
		// 0 means we don't care about a connectionValidationInteger, and false
		// for low priority threads
		bool b = rakServer->Start(2, 0, 30, atoi(portstring));
		if (b)
			puts("Server started, waiting for connections.");
		else
		{ 
			puts("Server failed to start.  Terminating.");
			exit(1);
		}
		
		// *** DISTRIBUTED OBJECT USER NOTE #7 ***
		// To use distributed objects we have to register our network instance with the distributed object manager
		DistributedNetworkObjectManager::Instance()->RegisterRakServerInterface(rakServer);
	}

//	printf("Create (new) (A)pple (O)range (B)arrel.\n");
	printf("Create (new) (B)arrel of fruit.\n");
	printf("Change (C)olor of apple.\n");
	printf("Change (R)adius of orange.\n");
	printf("Change (S)weetness of orange.\n");
	printf("Change (L)abel on apple.\n");
	printf("Change (N)ame of orange.\n");
	printf("Change (G)rams of apple.\n");
	printf("(E)at (delete) Apple\n");
	printf("(P)eel (delete) Orange\n");
	printf("Hit space to show all values\n");
	printf("(Q)uit.\n");
	
	char ch=0;
	while (1)
	{
		rakClientMultiplayer.ProcessPackets(rakClient);
		rakServerMultiplayer.ProcessPackets(rakServer);
		if (kbhit())
			ch=getch();

		/*
		if (ch=='a')
		{
			if (apple==0)
			{
				printf("Creating apple.\n");
				apple = new Apple;
			}
			else
				printf("Apple already exists.\n");
		}
		else if (ch=='o')
		{
			if (orange==0)
			{
				printf("Creating orange.\n");
				orange = new Orange;
			}
			else
				printf("Orange already exists.\n");
		}
		else */if (ch=='b')
		{
			if (barrel==0)
			{
				printf("Creating barrel.\n");
				barrel = new Barrel;
			}
			else
				printf("Barrel already exists.\n");
		}
		else if (ch=='c' && apple)
		{
			apple->color=(char)(randomMT() % 10);
			printf("Apple color changed to %i\n", apple->color);
		}
		else if (ch=='g' && apple)
		{
			apple->numberOfGrams=randomMT() % 1000;
			printf("Apple grams changed to %i\n", apple->numberOfGrams);
		}
		else if (ch=='r' && orange)
		{
			orange->radius=frandomMT();
			printf("Orange radius changed to %f\n", orange->radius);
		}
		else if (ch=='s' && orange)
		{
			orange->sweetness=randomMT();
			printf("Orange sweetness changed to %i\n", orange->sweetness);
		}
		else if (ch=='l' && apple)
		{
			printf("Enter name of apple: ");
			gets(apple->fruitName.name);
		}
		else if (ch=='n' && orange)
		{
			printf("Enter name of orange: ");
			gets(orange->fruitName.name);
		}
		else if (ch=='e' && apple)
		{
			// *** DISTRIBUTED OBJECT USER NOTE #8 ***
			// If the client deletes an object it will not automatically propagate.  To propagate a deletion, you must call DestroyObjectOnNetwork
			// The server could can either delete the object directly or call DestroyObjectOnNetwork
			apple->DestroyObjectOnNetwork();
		}
		else if (ch=='p' && orange)
		{
			// *** DISTRIBUTED OBJECT USER NOTE #8 ***
			// If the client deletes an object it will not automatically propagate.  To propagate a deletion, you must call DestroyObjectOnNetwork
			// The server could can either delete the object directly or call DestroyObjectOnNetwork
			orange->DestroyObjectOnNetwork();
		}
		else if (ch==' ')
		{
			if (apple)
				printf("Apple: name = %s, color=%i grams=%i owner=%i:%i\n", apple->fruitName.name, apple->color, apple->numberOfGrams, apple->GetClientOwnerID().binaryAddress, apple->GetClientOwnerID().port);
			else
				printf("No apple\n");

			if (orange)
				printf("Orange: name = %s, radius=%f, sweetness=%i grams=%i owner=%i:%i\n", orange->fruitName.name, orange->radius, orange->sweetness, orange->numberOfGrams, orange->GetClientOwnerID().binaryAddress, orange->GetClientOwnerID().port);
			else
				printf("No orange\n");
		}
		else if (ch=='q')
			break;

		ch=0;

		// *** DISTRIBUTED OBJECT USER NOTE #9 ***
		// We have to update all distributed objects in our update cycle
		if (apple)
			apple->UpdateDistributedObject("Apple");
		if (orange)
			orange->UpdateDistributedObject("Orange");
		if (barrel)
			barrel->UpdateDistributedObject("Barrel");
	}

	RakNetworkFactory::DestroyRakClientInterface(rakClient);
	RakNetworkFactory::DestroyRakServerInterface(rakServer);

	return 0;
}
