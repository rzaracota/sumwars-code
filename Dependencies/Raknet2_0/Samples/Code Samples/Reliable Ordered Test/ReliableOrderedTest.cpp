#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "GetTime.h"
#include "PacketEnumerations.h"
#include "BitStream.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include "Rand.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h> // Sleep
#else
#include <unistd.h> // usleep
#endif

int main(void)
{
	RakPeerInterface *sender, *receiver;
	unsigned int packetNumber[32], receivedPacketNumber, receivedTime;
	char str[256];
	char ip[32];
	unsigned int sendInterval, nextSend, currentTime, quitTime;
	unsigned short remotePort, localPort;
	unsigned char streamNumber;
	RakNet::BitStream bitStream;
	Packet *packet;

	for (int i=0; i < 32; i++)
		packetNumber[i]=0;
	
	printf("Act as (s)ender or (r)eceiver?\n");
	gets(str);
	if (str[0]==0)
		return 1;

	if (str[0]=='s' || str[0]=='S')
	{
		sender = RakNetworkFactory::GetRakPeerInterface();
		receiver = 0;

		printf("Enter number of ms to pass between sends: ");
		gets(str);
		if (str[0]==0)
			sendInterval=30;
		else
			sendInterval=atoi(str);

		printf("Enter remote IP: ");
		gets(ip);
		if (ip[0]==0)
			strcpy(ip, "127.0.0.1");
		
		printf("Enter remote port: ");
		gets(str);
		if (str[0]==0)
			strcpy(str, "60000");
		remotePort=atoi(str);

		printf("Enter local port: ");
		gets(str);
		if (str[0]==0)
			strcpy(str, "0");
		localPort=atoi(str);


		printf("Connecting...\n");
		sender->Initialize(1, localPort, 0);
		sender->Connect(ip, remotePort, 0, 0);
	}
	else
	{
		receiver = RakNetworkFactory::GetRakPeerInterface();
		sender=0;

		printf("Enter local port: ");
		gets(str);
		if (str[0]==0)
			strcpy(str, "60000");
		localPort=atoi(str);

		printf("Waiting for connections...\n");
		receiver->Initialize(32, localPort, 0);
		receiver->SetMaximumIncomingConnections(32);
	}

	
	printf("How long to run this test for, in seconds?\n");
	gets(str);
	if (str[0]==0)
		strcpy(str, "60");
	
	currentTime = RakNet::GetTime();
	quitTime = atoi(str) * 1000 + currentTime;

	nextSend=currentTime;

	printf("Test running.\n");

	while (currentTime < quitTime)
	{
		if (sender)
		{
			packet = sender->Receive();
			while (packet)
			{
				// PARSE TYPES
				switch(packet->data[0])
				{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("ID_DISCONNECTION_NOTIFICATION\n");
					break;
				case ID_CONNECTION_LOST:
					printf("ID_CONNECTION_LOST\n");
					break;
				case ID_REMOTE_PORT_REFUSED:
					printf("ID_REMOTE_PORT_REFUSED\n");
					break;
				}

				sender->DeallocatePacket(packet);
				packet = sender->Receive();
			}
			
			while (currentTime > nextSend)
			{
				streamNumber=0;
			//	streamNumber = randomMT() % 32;
				// Do the send
				bitStream.Reset();
				bitStream.Write((unsigned char) (ID_RESERVED9+1));
				bitStream.Write(streamNumber);
				bitStream.Write(packetNumber[streamNumber]++);
				bitStream.Write(currentTime);
				// Send on a random priority with a random stream
				if (sender->Send(&bitStream, (PacketPriority)(HIGH_PRIORITY + (randomMT() % 2)), RELIABLE_ORDERED,streamNumber, UNASSIGNED_PLAYER_ID, true)==false)
					packetNumber[streamNumber]--; // Didn't finish connecting yet?

				nextSend+=sendInterval;
			}
		}
		else
		{
			packet = receiver->Receive();
			while (packet)
			{
				switch(packet->data[0])
				{
				case ID_NEW_INCOMING_CONNECTION:
					printf("ID_NEW_INCOMING_CONNECTION\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("ID_DISCONNECTION_NOTIFICATION\n");
					break;
				case ID_CONNECTION_LOST:
					printf("ID_CONNECTION_LOST\n");
					break;
				case ID_REMOTE_PORT_REFUSED:
					printf("ID_REMOTE_PORT_REFUSED\n");
					break;
				case ID_RESERVED9+1:
					bitStream.Reset();
					bitStream.Write((char*)packet->data, packet->length);
					bitStream.IgnoreBits(8); // Ignore ID_RESERVED9+1
					bitStream.Read(streamNumber);
					bitStream.Read(receivedPacketNumber);					
					bitStream.Read(receivedTime);

					if (receivedPacketNumber!=packetNumber[streamNumber])
						printf("ERROR! Expecting %i got %i (channel %i). ",packetNumber[streamNumber], receivedPacketNumber, streamNumber);
					else
						printf("User got %i (channel %i). ", packetNumber[streamNumber], streamNumber);

					printf("Sent=%u Received=%u Diff=%i.\n", receivedTime, currentTime, (int)currentTime - (int) receivedTime);

					packetNumber[streamNumber]++;
					break;
				}

				
				receiver->DeallocatePacket(packet);
				packet = receiver->Receive();
			}
		}

		// DO NOT COMMENT OUT THIS SLEEP!
		// This sleep keeps RakNet responsive
#ifdef _WIN32
		Sleep(0);
#else
		usleep(0);
#endif
		currentTime=RakNet::GetTime();
	}

	printf("Press any key to continue\n");
	gets(str);

	if (sender)
		RakNetworkFactory::DestroyRakPeerInterface(sender);
	if (receiver)
		RakNetworkFactory::DestroyRakPeerInterface(receiver);

	return 1;
}