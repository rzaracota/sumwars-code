// ----------------------------------------------------------------------
// Filename Compression.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) September 25, 2003
// Demonstrates how to use compression with the standard chat example.
// The first time you run this you won't have frequency tables, so you must run it twice.
// The first time to generate a table, the second time to use the last table generated
// ----------------------------------------------------------------------
#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"
#include "Multiplayer.h"
#include "NetworkTypes.h"
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

// Holds enumeration data
const int MOTD_MAXIMUM_LENGTH=50; // Characters allocated for the message of the day, used for enumerations
const int SERVER_NAME_MAXIMUM_LENGTH=40; // Characters allocated for the server name

struct EnumerationDataStruct
{
	char MOTD[MOTD_MAXIMUM_LENGTH];  // (Message of the day) Optional, replace with whatever and/or add more fields
	char serverName[SERVER_NAME_MAXIMUM_LENGTH];
};

const int SCDS_MAXIMUM_NAME_LENGTH=40;

struct StaticClientDataStruct
{
	unsigned char typeId;  // ID_SET_CLIENT_DATA
	char name[SCDS_MAXIMUM_NAME_LENGTH];
};

template <class InterfaceType>
class ClientMultiplayer : public Multiplayer<InterfaceType>
{
	void ProcessUnhandledPacket(Packet *packet, unsigned char packetIdentifier, InterfaceType *interfaceType)
	{
		// It's a client, so just show the message
		printf("%s\n", packet->data);
	}
};

template <class InterfaceType>
class ServerMultiplayer : public Multiplayer<InterfaceType>
{
	void ProcessUnhandledPacket(Packet *packet, unsigned char packetIdentifier, InterfaceType *interfaceType)
	{
		char message[200];
		// The server knows the static data of all clients, so we can prefix the message
		// With the name data
		printf("%s: %s\n", ((StaticClientDataStruct*)(interfaceType->GetStaticClientData(packet->playerId)->GetData()))->name, packet->data);

		// Relay the message.  We prefix the name for other clients.  This demonstrates
		// That messages can be changed on the server before being broadcast
		// Sending is the same as before
		sprintf(message, "%s: %s", ((StaticClientDataStruct*)(interfaceType->GetStaticClientData(packet->playerId)->GetData()))->name, packet->data);
		interfaceType->Send(message, (const int)strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->playerId, true);
	}
};

int main(void)
{
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	FILE *serverToClientFrequencyTableFilePointer, *clientToServerFrequencyTableFilePointer;
	char serverToClientFrequencyTableFilename[100], clientToServerFrequencyTableFilename[100];
	char buff[256];

	RakClientInterface *rakClient=RakNetworkFactory::GetRakClientInterface();
	RakServerInterface *rakServer=RakNetworkFactory::GetRakServerInterface();
	ClientMultiplayer<RakClientInterface> rakClientMultiplayer;
	ServerMultiplayer<RakServerInterface> rakServerMultiplayer;

	// Frequency table tracking takes a bit of CPU power under high loads so is off by default.  We need to turn it on
	// So RakNet will track data frequencies which we can then use to create the compression tables
	rakServer->SetTrackFrequencyTable(true);
	rakClient->SetTrackFrequencyTable(true);

	int i = rakServer->GetNumberOfAddresses();

	// Just so we can remember where the packet came from
	bool isServer;

	// We are generating compression layers for both the client and the server just to demonstrate it
	// in practice you would only do one or the other depending on whether you wanted to run as a client or as a server
	// Note that both the client and the server both need both frequency tables and they must be the same
	puts("Enter the filename holding the server to client frequency table:");
	gets(serverToClientFrequencyTableFilename);
	serverToClientFrequencyTableFilePointer = fopen(serverToClientFrequencyTableFilename, "rb");
	if (serverToClientFrequencyTableFilePointer==0)
	{
		printf("Can't open %s\n", serverToClientFrequencyTableFilename);
	}
	else
	{
		unsigned int frequencyData[256];
		int numRead;
		numRead=(int)fread(frequencyData, sizeof(unsigned int), 256, serverToClientFrequencyTableFilePointer);
        if (numRead != 256)
			puts("Error reading data");
		else
		{
			rakClient->GenerateCompressionLayer(frequencyData, true); // server to client is input for the client so the last parameter is true
			rakServer->GenerateCompressionLayer(frequencyData, false); // server to client is output for the server so the last parameter is false
			puts("Compression layer generated for server to client data");
		}

		fclose(serverToClientFrequencyTableFilePointer);
	}

	puts("Enter the filename holding the client to server frequency table:");
	gets(clientToServerFrequencyTableFilename);
	clientToServerFrequencyTableFilePointer = fopen(clientToServerFrequencyTableFilename, "rb");
	if (clientToServerFrequencyTableFilePointer==0)
	{
		printf("Can't open %s\n", clientToServerFrequencyTableFilename);
	}
	else
	{
		unsigned int frequencyData[256];
		int numRead;
		numRead=(int)fread(frequencyData, sizeof(unsigned int), 256, clientToServerFrequencyTableFilePointer);
		if (numRead != 256)
			puts("Error reading data");
		else
		{
			rakClient->GenerateCompressionLayer(frequencyData, false); // client to server is output for the client so the last parameter is false
			rakServer->GenerateCompressionLayer(frequencyData, true); // client to server is input for the server so the last parameter is true
			puts("Compression layer generated for server to client data");
		}

		fclose(clientToServerFrequencyTableFilePointer);
	}


	// Crude interface
	puts("Run as (1) Client or (2) Server ?");
	if ( gets(buff)&&(buff[0]=='1') )
	{
		// Holds user data
		char ip[30], serverPort[30], clientPort[30];
		StaticClientDataStruct staticClientData;

		// A client
		isServer=false;

		// Get our input
		puts("Enter the client port to listen on.");
		gets(clientPort);
		puts("Enter your name (up to 40 characters)");
		gets(staticClientData.name);
		// Note this passes by value, because otherwise people could
		// get access to and damage our internal data
		// UNASSIGNED_PLAYER_ID is to specify changing our own data, rather than our copy of another clients data
		rakClient->SetStaticClientData(UNASSIGNED_PLAYER_ID, (char*)&staticClientData, sizeof(StaticClientDataStruct));
		puts("Enter IP to connect to");
		gets(ip);
		puts("Enter the port to connect to");
		gets(serverPort);

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
	}
	else
	{
		// Holds user data
		char portstring[30];
		EnumerationDataStruct enumerationDataStruct;

		// A server
		isServer=true;
		puts("Enter the server port to listen on");
		gets(portstring);
		puts("Enter the server name  (up to 40 characters)");
		gets(enumerationDataStruct.serverName);
		puts("Enter the server message of the day  (up to 50 characters)");
		gets(enumerationDataStruct.MOTD);
		// Note this passes by value, because otherwise people could get access
		// to and damage our internal data
		rakServer->SetStaticServerData((char*)&enumerationDataStruct, sizeof(EnumerationDataStruct));
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
	}

	puts("Type 'quit' to quit or type to talk.");
	char message[400];

	// Loop for input
	while (1)
	{
		if (kbhit())
		{
			// Notice what is not here: something to keep our network running.  It's
			// fine to block on gets or anything we want
			// Because the network engine was painstakingly written using threads.
			gets(message);

			if (strcmp(message, "quit")==0)
			{
				puts("Quitting.");
				break;
			}

			// Message now holds what we want to broadcast
			if (isServer)
			{
				char message2[420];
				// Append Server: to the message so clients know that it ORIGINATED from the server
				// All messages to all clients come from the server either directly or by being
				// relayed from other cilents
				strcpy(message2, "Server: ");
				strcat(message2, message);

				// message2 is the data to send
				// strlen(message2)+1 is to send the null terminator
				// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
				// RELIABLE_ORDERED means make sure the message arrives in the right order
				// We arbitrarily pick 0 for the ordering stream
				// UNASSIGNED_PLAYER_ID means don't exclude anyone from the broadcast
				// true means broadcast the message to everyone connected
				rakServer->Send(message2, (const int)strlen(message2)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
			}
			else
			{
				// message is the data to send
				// strlen(message)+1 is to send the null terminator
				// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
				// RELIABLE_ORDERED means make sure the message arrives in the right order
				rakClient->Send(message, (const int)strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
			}
		}
		rakClientMultiplayer.ProcessPackets(rakClient);
		rakServerMultiplayer.ProcessPackets(rakServer);
	}

	// Output statistics
	printf("Server: Compression ratio=%f. Decompression ratio=%f\n", rakServer->GetCompressionRatio(), rakServer->GetDecompressionRatio());
	printf("Client: Compression ratio=%f. Decompression ratio=%f\n", rakClient->GetCompressionRatio(), rakClient->GetDecompressionRatio());

	rakServer->Disconnect(100);
	rakClient->Disconnect(100);

	// Output the frequency table generated by this run
	if (isServer)
		puts("Enter the filename to use to save the server to client frequency table:");
	else
		puts("Enter the filename to use to save the client to server frequency table:");

	scanf("%s", serverToClientFrequencyTableFilename);
	serverToClientFrequencyTableFilePointer = fopen(serverToClientFrequencyTableFilename, "wb");
	if (serverToClientFrequencyTableFilePointer==0)
	{
		printf("Can't open %s\n", serverToClientFrequencyTableFilename);
	}
	else
	{
		unsigned int frequencyData[256];

		// Get the frequency table generated during the run
		if (isServer)
			rakServer->GetSendFrequencyTable(frequencyData);
		else
			rakClient->GetSendFrequencyTable(frequencyData);

		fwrite(frequencyData, sizeof(unsigned int), 256, serverToClientFrequencyTableFilePointer);
		fclose(serverToClientFrequencyTableFilePointer);
	}

	// We're done with the network
	RakNetworkFactory::DestroyRakServerInterface(rakServer);
	RakNetworkFactory::DestroyRakClientInterface(rakClient);

	return 0;
}
