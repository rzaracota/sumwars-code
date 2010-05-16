// This is my own internal test program for the master client but serves as a good example.
// Right now it is hardcoded to connect to 127.0.0.1.  You would run the master server first, then run this.

#include "MasterCommon.h"
#include "MasterClient.h"
#include "StringCompressor.h"
#include "BitStream.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <conio.h>
#else
#include "../Unix/kbhit.h"
#endif

#define READCHAR(arg) gets(arg); ch=arg[0];

// remove this
#include "PacketEnumerations.h"

int main(void)
{
	BitStream bitStream;
	char str[256];
	char ch;
	MasterClient masterClient;
	RakPeerInterface *testGameServer;
	unsigned long serverListSize, index;
	bool identiferFound;
	const char *outputString;
	int outputInt;

	// Create a fake game
	testGameServer = RakNetworkFactory::GetRakPeerInterface();
	testGameServer->Initialize(8, 60003, 0);
	testGameServer->SetMaximumIncomingConnections(8);

	if (masterClient.Connect("127.0.0.1", 60000, 60001,60003))
		printf("Master client connecting...\n");
	else
		printf("Master client failed to start or connect.\n");

	printf("(Q)uit\n(q)uery master server\n(l)ist server\n(d)elist server\n(a)dd rule\n(r)emove rule\n(p)ing server list\n(c)onnect using NAT punch-through with master server, bypassing most NATs\n(SPACE) print server list\n");
	char buff[256];
	while (1)
	{
		if (kbhit())
		{
			READCHAR(buff);
			if (ch=='Q')
				break;
			if (ch=='q')
			{
				masterClient.ClearQueryRules();
				printf("Enter query key 1/2 or enter for none: ");
				gets(str);
				masterClient.AddQueryRule(str);
				printf("Enter query key 2/2 or enter for none: ");
				gets(str);
				masterClient.AddQueryRule(str);
				masterClient.QueryMasterServer();
				printf("Server queried.  Press space to see server list.\n");
			}
			else if (ch=='l')
			{
				printf("Uploading game server.  Query to see it.\n");
				masterClient.ListServer();
			}
			else if (ch=='d')
			{
				printf("Server delisted.  Query to update our own list.\n");
				masterClient.DelistServer();
			}
			else if (ch=='a')
			{
				printf("Adding sample rules.  Query to update our own list.\n");
				masterClient.PostRule("Game name", "My big game o' death.", 0);
				masterClient.PostRule("Game type", "Death match", 0);
				masterClient.PostRule("Score",0, 100);
			}
			else if (ch=='r')
			{
				printf("Removing rules. Query to update our own list.\n");
				masterClient.RemoveRule("Game type");
				masterClient.RemoveRule("Game name");
				masterClient.RemoveRule("Score");
			}
			else if (ch=='p')
			{
				printf("Pinging any servers in our list\n");
				masterClient.PingServers();
			}
			else if (ch=='c')
			{
				char ip[22];
				printf("Sending connection attempt notification to master server\n");
				printf("Enter IP of server from game list: ");
				gets(ip);
				printf("Enter port: ");
				gets(str);
				if (ip[0]!=0 && str[0]!=0)
				{
					masterClient.ConnectionAttemptNotification(ip, atoi(str));
					printf("Sent connection attempt notification to the server the master server\n");
				}
				else
				{
					printf("Aborting...");
				}				
			}
			else if (ch==' ')
			{
				serverListSize=masterClient.GetServerListSize();
				if (serverListSize==0)
				{
					printf("No servers in list\n");
				}
				else
				{
					for (index=0; index < serverListSize; index++)
					{
						printf("%i. ", index);
						outputString=masterClient.GetServerListRuleAsString(index, "IP", &identiferFound);
						if (identiferFound)
							printf("%s:", outputString);
						else
							printf("NO_IP:");
						outputInt=masterClient.GetServerListRuleAsInt(index, "Port", &identiferFound);
						if (identiferFound)
							printf("%i ", outputInt);
						else
							printf("NO_PORT ");
						outputInt=masterClient.GetServerListRuleAsInt(index, "Ping", &identiferFound);
						if (identiferFound)
							printf("%i ", outputInt);
						else
							printf("NO_PING ");
						outputString=masterClient.GetServerListRuleAsString(index, "Game type", &identiferFound);
						if (identiferFound)
							printf("%s ", outputString);
						else
							printf("NO_GT ");
						outputString=masterClient.GetServerListRuleAsString(index, "Game name", &identiferFound);
						if (identiferFound)
							printf("%s ", outputString);
						else
							printf("NO_GN ");
						outputInt=masterClient.GetServerListRuleAsInt(index, "Score", &identiferFound);
						if (identiferFound)
							printf("%i\n", outputInt);
						else
							printf("NO_SCORE\n");
					}
				}
			}
			ch=0;
		}

		masterClient.Update();
	}

	masterClient.Disconnect();
	RakNetworkFactory::DestroyRakPeerInterface(testGameServer);

	return 0;
}
