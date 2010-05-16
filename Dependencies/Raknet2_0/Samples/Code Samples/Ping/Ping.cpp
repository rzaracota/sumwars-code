// ----------------------------------------------------------------------
// RakNet version 1.0
// Filename ChatExample.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) January 24, 2003
// Very basic chat engine example
// ----------------------------------------------------------------------
#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"
#include "NetworkTypes.h"
#include "GetTime.h"
#include "BitStream.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <conio.h>
#else
#include "../Unix/kbhit.h"
#endif

int main(void)
{
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	RakClientInterface *client=RakNetworkFactory::GetRakClientInterface();
	RakServerInterface *server=RakNetworkFactory::GetRakServerInterface();

	int i = server->GetNumberOfAddresses();

	// Holds packets
	Packet* p;

	// Record the first client that connects to us so we can pass it to the ping function
	PlayerID clientID=UNASSIGNED_PLAYER_ID;


	char portstring[30];

	// A server
	puts("Enter the server port to listen on");
	gets(portstring);
	if (portstring[0]==0)
		strcpy(portstring,"60000");

	// Enumeration data
	puts("Enter offline ping response data (for return by a LAN discovery for example)");
	puts("Hit enter for none.");
	char enumData[512];
	gets(enumData);
	if (enumData[0])
		server->SetOfflinePingResponse(enumData, strlen(enumData)+1);

	puts("Starting server.");

	// Test to make sure this works when security is on
	server->InitializeSecurity(0,0);

	// The server has to be started to respond to pings.
	bool b = server->Start(2, 0, 30, atoi(portstring));
	if (b)
		puts("Server started, waiting for connections.");
	else
	{ 
		puts("Server failed to start.  Terminating.");
		exit(1);
	}

	puts("'q' to quit, any other key to send a ping from the client.");
	char buff[256];

	// Loop for input
	while (1)
	{
		if (kbhit())
		{
			// Holds user data
			char ip[30], serverPort[30], clientPort[30];


			if (gets(buff)&&(buff[0]=='q'))
				break;
			else
			{

				// Get our input
				puts("Enter the client port to listen on, or 0");
				gets(clientPort);
				if (clientPort[0]==0)
					strcpy(clientPort, "0");
				puts("Enter IP to ping");
				gets(ip);
				if (ip[0]==0)
					strcpy(ip, "127.0.0.1");
				puts("Enter the port to ping");
				gets(serverPort);
				if (serverPort[0]==0)
					strcpy(serverPort, "60000");

				// Connecting the client is very simple.  0 means we don't care about
				// a connectionValidationInteger, and false for low priority threads
				client->PingServer(ip, atoi(serverPort), atoi(clientPort), false);

				puts("Pinging");
			}
		}

		// Get a packet from either the server or the client
		p = server->Receive();

		if (p==0)
			p = client->Receive();

		if (p==0)
			continue;


		// Check if this is a network message packet
		switch (p->data[0])
		{
			
			case ID_PONG:
				unsigned int time, dataLength;
				RakNet::BitStream pong( (char*)p->data+1, sizeof(unsigned int), false);
				pong.Read(time);
				dataLength = p->length - sizeof(unsigned char) - sizeof(unsigned int);
				printf("ID_PONG from PlayerID:%u:%u.\nPing is %i\nData is %i bytes long.\n",p->playerId.binaryAddress, p->playerId.port, time, dataLength);
				if (dataLength > 0)
					printf("Data is %s\n", p->data+sizeof(unsigned char)+sizeof(unsigned int));
				break;
			
			// In this sample since the client is not running a game we can save CPU cycles by
			// Stopping the network threads after receiving the pong.
			client->Disconnect(100);
		}

		// We're done with the packet
		server->DeallocatePacket(p);
	}

	// We're done with the network
	RakNetworkFactory::DestroyRakServerInterface(server);
	RakNetworkFactory::DestroyRakClientInterface(client);

	return 0;
}
