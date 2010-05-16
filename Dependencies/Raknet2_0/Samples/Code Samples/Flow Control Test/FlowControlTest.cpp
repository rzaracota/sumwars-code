#include "RakNetworkFactory.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "GetTime.h"
#include "PacketEnumerations.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <conio.h>
#else
#include "../Unix/kbhit.h"
#endif

int main(void)
{
	RakServerInterface *rakServer;
	RakClientInterface *rakClient;
	char ch;
	char str[255], remoteIP[255];
	char randomData[8192];
	int localPort, remotePort;
	int packetSize;
	int sendinterval;
	int time;
	Packet *p;
	unsigned int lastPacketReceipt, lastNotification, lastSend;
	#ifndef _WIN32
	char buff[256];
	#endif
	
	memset(randomData, 255, sizeof(randomData));

	printf("Start relay (s)erver or start (c)lient?\n");
#ifndef _WIN32
	gets(buff);
	ch=buff[0];
#else
	ch=getch();
#endif
	if (ch=='s' || ch=='S')
	{
		printf("Acting as server.\n");
		rakServer=RakNetworkFactory::GetRakServerInterface();
		rakClient=0;
	}
	else
	{
		printf("Acting as client.\n");
		rakClient=RakNetworkFactory::GetRakClientInterface();
		rakServer=0;
	}

	printf("Enter local port: ");
	gets(str);
	if (str[0]==0)
	{
		if (rakServer)
			localPort=60000;
		else
			localPort=0;
	}
	else
		localPort=atoi(str);

	if (rakServer)
	{
		printf("(H)igh priority thread or (R)egular?\n");
#ifndef _WIN32
		gets(buff);
		ch=buff[0];
#else
		ch=getch();
#endif
		if (ch=='h' || ch=='H')
		{
			rakServer->Start(100, 0, 0, localPort);
			printf("Server started under high priority\n");
		}
		else
		{
			rakServer->Start(100, 0, 30, localPort);
			printf("Server started under regular priority\n");
		}
	}
	else
	{
		printf("Enter remote IP: ");
		gets(remoteIP);
		if (remoteIP[0]==0)
			strcpy(remoteIP, "127.0.0.1");
		printf("Enter remote port: ");
		gets(str);
		if (str[0]==0)
			remotePort=60000;
		else
			remotePort=atoi(str);
		printf("(H)igh priority thread or (R)egular?\n");
#ifndef _WIN32
		gets(buff);
		ch=buff[0];
#else
		ch=getch();
#endif
		if (ch=='h' || ch=='H')
		{
			rakClient->Connect(remoteIP, remotePort, localPort, 0, 0);
			printf("Client started under high priority\n");
		}
		else
		{
			rakClient->Connect(remoteIP, remotePort, localPort, 0, 30);
			printf("Client started under regular priority.  Attempting connection...\n");
		}
	}

	printf("Entering loop.\nHit 'h' for help.\nHit 'q' to quit\n'i' to increase send interval\n'd' to decrease send interval\n'+' to increase packet size\n'-' to decrease packet size.\nSpace to show current statistics\n");

	sendinterval=128;
	packetSize=64;
	lastPacketReceipt=lastNotification=RakNet::GetTime();
	lastSend=0;

	while (1)
	{
		time=RakNet::GetTime();

		if (kbhit())
		{
#ifndef _WIN32
			gets(buff);
			ch=buff[0];
#else
			ch=getch();
#endif
			if (ch=='q')
			{
				printf("Quitting\n");
				break;
			}
			else if (ch=='i')
			{
				sendinterval*=2;				
				printf("Send interval is now %i\n", sendinterval);
			}
			else if (ch=='d')
			{
				if (sendinterval>1)
					sendinterval/=2;
				printf("Send interval is now %i\n", sendinterval);
			}
			if (ch=='h')
				printf("Hit 'h' for help.\nHit 'q' to quit\n'i' to increase send interval\n'd' to decrease send interval\n'+' to increase packet size\n'-' to decrease packet size.\nSpace to show current statistics\n");
			else if (ch=='+')
			{
				if (packetSize < 8192)
					packetSize*=2;
				printf("Packet size is now %i\n", packetSize);
			}
			else if (ch=='-')
			{
				if (packetSize>1)
					packetSize/=2;
				printf("Packet size is now %i\n", packetSize);
			}
			else if (ch==' ')
			{
				if (rakServer)
				{
					StatisticsToString(rakServer->GetStatistics(rakServer->GetPlayerIDFromIndex(0)), randomData, 1);
					printf("%s", randomData);
				}
				else
				{
					StatisticsToString(rakClient->GetStatistics(), randomData, 1);
					printf("%s", randomData);
				}

				printf("Send interval is %i\n", sendinterval);
				printf("Packet size is %i\n", packetSize);
				printf("\n");
			}

			
			ch=0;
		}

		// get packets
		if (rakServer)
			p = rakServer->Receive();
		else
			p=rakClient->Receive();

		if (p)
		{
			lastPacketReceipt=RakNet::GetTime();

			switch (p->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;

			case ID_NEW_INCOMING_CONNECTION:
				// Somebody connected.  We have their IP now
				printf("ID_NEW_INCOMING_CONNECTION\n");
				break;

			case ID_RECEIVED_STATIC_DATA:
				// Got static data
				printf("ID_RECEIVED_STATIC_DATA\n");
				break;

			case ID_MODIFIED_PACKET:
				// Cheater!
				printf("ID_MODIFIED_PACKET\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;
			default:
				// Relay
				if (rakServer)
					rakServer->Send((char*)p->data, p->length, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->playerId, true);
			}

			if (rakServer)
				rakServer->DeallocatePacket(p);
			else
				rakClient->DeallocatePacket(p);
		}

		if ((rakServer && rakServer->GetConnectedPlayers()>0) ||
			(rakClient && rakClient->IsConnected()))
		{
			// Do sends
			if ((int)lastSend + sendinterval < time)
			{
				if (rakServer)
				{
					rakServer->Send((char*)randomData, packetSize, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_PLAYER_ID, true);
				}
				else if (rakClient)
				{
					rakClient->Send((char*)randomData, packetSize, HIGH_PRIORITY, RELIABLE_ORDERED, 0);
				}

				lastSend=time;
			}		

			if (lastPacketReceipt + 500 < (unsigned int)time && lastNotification + 500 < (unsigned int)time)
			{
				lastNotification=time;
				printf("Warning: No packets for %i ms.  Possibly a spike.\n", time - lastPacketReceipt);
			}
		}
	}

	if (rakServer)
		RakNetworkFactory::DestroyRakServerInterface(rakServer);
	else
		RakNetworkFactory::DestroyRakClientInterface(rakClient);

	return 1;
}
