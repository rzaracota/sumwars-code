#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "PacketEnumerations.h" // Enumerations
#include "GetTime.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h> // Sleep
#define SLEEP(arg) ( Sleep( (arg) ) )
#else
#include <unistd.h> // usleep
#define SLEEP(arg) ( usleep( (arg) *1000 ) )
#endif

static const int DESTINATION_SYSTEM_PORT=60000;
static const int RELAY_SYSTEM_PORT=60001;
static const int SOURCE_SYSTEM_PORT=60002;

int main(void)
{
	RakPeerInterface *localSystem;
	Packet *p;
	int systemType, threadSleepTimer;
	unsigned char byteBlock[4096];
	unsigned long time, quitTime, nextStatsTime, packetsPerSecond, bytesPerPacket, lastSendTime,num,index, bytesInPackets;
	int sendMode;
	int verbosityLevel;
	unsigned long showStatsInterval;
	bool connectionCompleted, incomingConnectionCompleted;
	RakNetStatisticsStruct *rss;

	printf("Loopback performance test.\n");
	printf("This test measures the effective transfer rate of RakNet.\n\n");
	printf("Instructions:\nStart 3 instances of this program.\n");
	printf("Press\n1. for the first instance (destination)\n2. for the second instance (relay)\n3. for the third instance (source).\n");
	printf("When the third instance is started the test will start.\n\n");
	printf("Which instance is this?  Enter 1, 2, or 3: ");
	
	gets((char*)byteBlock);
	systemType=byteBlock[0]-'0'-1;
	if (systemType < 0 || systemType > 2)
	{
		printf("Error, you must enter 1, 2, or 3.\nQuitting.\n");
		return 1;
	}

	localSystem=RakNetworkFactory::GetRakPeerInterface();
	printf("Enter thread sleep timer:\n(0). Regular\n(1). High\n(2). None (single threaded)\n");
	gets((char*)byteBlock);
	if (byteBlock[0]==0)
	{
		printf("Defaulting to regular.\n");
		threadSleepTimer=0;
	}
	else
	{
		if (byteBlock[0]<'0' || byteBlock[0]>'2')
		{
			printf("Error, you must enter 0, 1, or 2\n.Quitting.\n");
			return 1;
		}
		threadSleepTimer=byteBlock[0]-'0';
	}

	if (threadSleepTimer==0)
		threadSleepTimer=30;
	else if (threadSleepTimer==1)
		threadSleepTimer=0;
	else
		threadSleepTimer=-1;
	
	printf("How many seconds do you want to run the test for?\n");
	gets((char*)byteBlock);
	if (byteBlock[0]==0)
	{
		printf("Defaulting to 60 seconds\n");
		quitTime=60;
	}
	else
		quitTime=atoi((char*)byteBlock);

	printf("Enter statistics verbosity level, 0=lowest, 2=highest\n");
	gets((char*)byteBlock);
	if (byteBlock[0]==0)
	{
		printf("Defaulting to verbosity level 1\n");
		verbosityLevel=1;
	}
	else
		verbosityLevel=atoi((char*)byteBlock);

	printf("How frequently to show statistics, in seconds?\n");
	gets((char*)byteBlock);
	if (byteBlock[0]==0)
	{
		printf("Defaulting to 5 seconds\n");
		showStatsInterval=5*1000;
	}
	else
		showStatsInterval=atoi((char*)byteBlock)*1000;

	if (systemType==0)
	{
		printf("Initializing Raknet...\n");
		// Destination.  Accept one connection and wait for further instructions.
		if (localSystem->Initialize(1, DESTINATION_SYSTEM_PORT, threadSleepTimer)==false)
		{
			printf("Failed to initialize RakNet!.\nQuitting\n");
			return 1;
		}	
		localSystem->SetMaximumIncomingConnections(1);
		printf("Initialization complete. Destination system started and waiting...\n");
	}
	else if (systemType==1)
	{
		printf("Enter MTU size to use.  576 for dialup, 1400 for AOL, 1492 otherwise.\n");
		gets((char*)byteBlock);
		if (byteBlock[0]==0)
		{
			printf("Defaulting to 1492.\n");
			localSystem->SetMTUSize(1492);
		}
		else
			localSystem->SetMTUSize(atoi((char*)byteBlock));

		printf("What send mode to use for relays?\n");
		printf("(0). UNRELIABLE\n");
		printf("(1). UNRELIABLE_SEQUENCED\n");
		printf("(2). RELIABLE\n");
		printf("(3). RELIABLE_ORDERED\n");
		printf("(4). RELIABLE_SEQUENCED\n");
		gets((char*)byteBlock);
		if (byteBlock[0]==0)
		{
			printf("Defaulting to RELIABLE\n");
			sendMode=2;
		}
		else
		{
			sendMode=atoi((char*)byteBlock);
			if (sendMode < 0 || sendMode > 4)
			{
				printf("Invalid send mode.  Using UNRELIABLE\n");
				sendMode=0;
			}
		}

		printf("Initializing Raknet...\n");
		// Relay.  Accept one connection, initiate outgoing connection, wait for further instructions.
		if (localSystem->Initialize(2, RELAY_SYSTEM_PORT, threadSleepTimer)==false)
		{
			printf("Failed to initialize RakNet!.\nQuitting\n");
			return 1;
		}
		localSystem->SetMaximumIncomingConnections(1);
		if (localSystem->Connect("127.0.0.1", DESTINATION_SYSTEM_PORT, 0, 0)==false)
		{
			printf("Connect call failed!.\nQuitting\n");
			return 1;
		}

		printf("Initialization complete. Relay system started.\nConnecting to destination and waiting for sender...\n");
	}
	else
	{
		printf("Enter MTU size to use. 576 for dialup, 1400 for AOL, 1492 otherwise.\n");
		gets((char*)byteBlock);
		if (byteBlock[0]==0)
		{
			printf("Defaulting to 1492.\n");
			localSystem->SetMTUSize(1492);
		}
		else
			localSystem->SetMTUSize(atoi((char*)byteBlock));

		printf("How many packets do you wish to send per second?\n");
		gets((char*)byteBlock);
		if (byteBlock[0]==0)
		{
#ifdef _DEBUG
			printf("Defaulting to 100\n");
			packetsPerSecond=100;
#else
			printf("Defaulting to 4000\n");
			packetsPerSecond=4000; // Don't go past MAX_AVERAGE_PACKETS_PER_SECOND
#endif
		}
		else
			packetsPerSecond=atoi((char*)byteBlock);
		printf("How many bytes per packet?\n");
		gets((char*)byteBlock);
		if (byteBlock[0]==0)
		{
			printf("Defaulting to 256\n");
			bytesPerPacket=256;
		}
		else
		{
			bytesPerPacket=atoi((char*)byteBlock);
			if (bytesPerPacket > 4096)
			{
				printf("Increase the array size of byteBlock to send more than 4096 bytes.\n");
				bytesPerPacket=4096;
			}
		}
		
		printf("What send mode?\n");
		printf("(0). UNRELIABLE\n");
		printf("(1). UNRELIABLE_SEQUENCED\n");
		printf("(2). RELIABLE\n");
		printf("(3). RELIABLE_ORDERED\n");
		printf("(4). RELIABLE_SEQUENCED\n");
		gets((char*)byteBlock);
		if (byteBlock[0]==0)
		{
			printf("Defaulting to RELIABLE\n");
			sendMode=2;
		}
		else
		{
			sendMode=atoi((char*)byteBlock);
			if (sendMode < 0 || sendMode > 4)
			{
				printf("Invalid send mode.  Using UNRELIABLE\n");
				sendMode=0;
			}
		}

		printf("Initializing RakNet...\n");
		// Sender.  Initiate outgoing connection to relay.
		if (localSystem->Initialize(1, SOURCE_SYSTEM_PORT, threadSleepTimer)==false)
		{
			printf("Failed to initialize RakNet!.\nQuitting\n");
			return 1;
		}
		if (localSystem->Connect("127.0.0.1", RELAY_SYSTEM_PORT, 0, 0)==false)
		{
			printf("Connect call failed!.\nQuitting\n");
			return 1;
		}

		printf("Initialization complete. Sender system started. Connecting to relay...\n");
	}

	connectionCompleted=false;
	incomingConnectionCompleted=false;
	time = RakNet::GetTime();
	lastSendTime=time;
	nextStatsTime=time+2000; // First stat shows up in 2 seconds
	bytesInPackets=0;

	while (time < quitTime || (connectionCompleted==false && incomingConnectionCompleted==false))
	{
		time = RakNet::GetTime();
		// Parse messages
		while (1)
		{
			p = localSystem->Receive();

			if (p)
			{
				bytesInPackets+=p->length;
				switch (p->data[0])
				{
				case ID_CONNECTION_REQUEST_ACCEPTED:
					printf("%ID_CONNECTION_REQUEST_ACCEPTED.\n");
					connectionCompleted=true;
					// Timer starts when a connection has completed
					if (systemType==1 || systemType==2)
						quitTime=quitTime*1000 + time;
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					// Connection lost normally
					printf("%ID_DISCONNECTION_NOTIFICATION.\n");
			//		connectionCompleted=false;
					break;
				case ID_RECEIVED_STATIC_DATA:
					break;
				case ID_NEW_INCOMING_CONNECTION:
					// Somebody connected.  We have their IP now
					printf("ID_NEW_INCOMING_CONNECTION.\n");
					incomingConnectionCompleted=true;
					// Timer starts when a new connection has come in
					if (systemType==0)
						quitTime=quitTime*1000 + time;
					if (systemType==1 && connectionCompleted==false)
						printf("Warning, relay connection to destination has not completed yet.\n");
					break;

				case ID_CONNECTION_LOST:
					// Couldn't deliver a reliable packet - i.e. the other system was abnormally
					// terminated
					printf("ID_CONNECTION_LOST.\n");
				//	connectionCompleted=false;
					break;
				default:
					// The relay system will relay all data with 255 as the first byte
					if (systemType==1)
					{
						if (p->data[0]==255)
						{
							if (localSystem->Send((char*)p->data, p->length, HIGH_PRIORITY, (PacketReliability)sendMode, 0, p->playerId, true)==false)
							{
								printf("Relay failed!");
							}
						}
						else
							printf("Got packet with ID %u\n", p->data[0]);
					}
						
					break;
				}
			}
			else
				break;

			localSystem->DeallocatePacket(p);
		}

		// Show stats.
		if (time > nextStatsTime && (connectionCompleted || incomingConnectionCompleted))
		{
			printf("\n* First connected system statistics:\n");
			rss=localSystem->GetStatistics(localSystem->GetPlayerIDFromIndex(0));
			StatisticsToString(rss, (char*)byteBlock, verbosityLevel);
			printf("%s", byteBlock);
			if (systemType==1)
			{
				rss=localSystem->GetStatistics(localSystem->GetPlayerIDFromIndex(1));
				if (rss)
				{
					printf("* Second connected system statistics:\n");
					StatisticsToString(rss, (char*)byteBlock, verbosityLevel);
					printf("%s", byteBlock);
				}				
			}

			nextStatsTime = time + showStatsInterval;
		}

		// As the destination, we don't care if the connection is completed.  Do nothing
		// As the relay, we relay packets if the connection is completed.
		// That is done when the packet arrives.
		// As the source, we start sending packets when the connection is completed.
		if (systemType==2 && connectionCompleted)
		{
			
			// Number of packets to send is (float)(packetsPerSecond * (time - lastSendTime)) / 1000.0f;
			num=(packetsPerSecond * (time - lastSendTime)) / 1000;
			byteBlock[0]=255; // Relay all data with an identifier of 255
			for (index=0; index < num; index++)
			{
				localSystem->Send((char*)byteBlock, bytesPerPacket, HIGH_PRIORITY, (PacketReliability)sendMode, 0, UNASSIGNED_PLAYER_ID, true);
			}
            
			lastSendTime+= (1000 * num) / packetsPerSecond;
		}

	}

	printf("Test duration elapsed.  Final Stats:\n");
	printf("\n* First connected system statistics:\n");
	rss=localSystem->GetStatistics(localSystem->GetPlayerIDFromIndex(0));
	StatisticsToString(rss, (char*)byteBlock, 2);
	printf("%s", byteBlock);
	if (systemType==1)
	{
		rss=localSystem->GetStatistics(localSystem->GetPlayerIDFromIndex(1));
		if (rss)
		{
			printf("* Second connected system statistics:\n");
			StatisticsToString(rss, (char*)byteBlock, 2);
			printf("%s", byteBlock);
		}				
	}

	printf("Hit enter to continue.\n");
	char buff[100];
	gets(buff);

	RakNetworkFactory::DestroyRakPeerInterface(localSystem);
	return 0;
}
