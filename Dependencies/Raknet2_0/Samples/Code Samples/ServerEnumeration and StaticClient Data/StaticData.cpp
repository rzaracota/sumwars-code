// ----------------------------------------------------------------------
// RakNet version 1.408
// Filename StaticData.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) December 6, 2003
// ----------------------------------------------------------------------

#include "Multiplayer.h"
#include "RakServer.h"
#include "RakClient.h"
#include "RakNetworkFactory.h"

#ifdef WIN32
#include <conio.h> // For kbhit
#include <windows.h>
#else
#include "../Unix/kbhit.h"
#include <unistd.h>
#endif


// We use these structs to hold our data.  We could as easily use a BitStream
#pragma pack(push) // Save the current byte alignment
#pragma pack(1) // Makes everything byte aligned so we don't send excess padding when we cast and send the struct
struct LocalServerData
{
	char serverName[20];
	char serverMOTD[30];
	void Serialize(RakNet::BitStream &bstream)
	{
		bstream.Reset();
		int i = 0;
		while(serverName[i]) { bstream.Write(serverName[i]); i++; }
		bstream.Write('\0');
		i = 0;
		while(serverMOTD[i]) { bstream.Write(serverMOTD[i]); i++; }
		bstream.Write('\0');
	}
	void Deserialize(const char *dstream)
	{
		RakNet::BitStream bstream(dstream, 50, false);
		int i = 0;
		unsigned char x = 1;
		while(x)
		{
			bstream.Read(x);
			serverName[i] = x;
			++i;
		}
		i = 0;
		x = 1;
		while(x)
		{
			bstream.Read(x);
			serverMOTD[i] = x;
			++i;
		}
	}
};

#pragma pack(1)
struct LocalClientData
{
	char clientName[20];
	char clientGreeting[30];
	void Serialize(RakNet::BitStream &bstream)
	{
		bstream.Reset();
		int i = 0;
		while(clientName[i]) { bstream.Write(clientName[i]); i++; }
		bstream.Write('\0');
		i = 0;
		while(clientGreeting[i]) { bstream.Write(clientGreeting[i]); i++; }
		bstream.Write('\0');
	}
	void Deserialize(const char*dstream)
	{
		RakNet::BitStream bstream(dstream, 50, false);
		int i = 0;
		unsigned char x = 1;
		while(x)
		{
			bstream.Read(x);
			clientName[i] = x;
			++i;
		}
		i = 0;
		x = 1;
		while(x)
		{
			bstream.Read(x);
			clientGreeting[i] = x;
			++i;
		}
	}
};

// Restore the old byte alignment
#pragma pack(pop)

// Used to put data into the LocalServerData struct
void GetServerData(LocalServerData *localServerData)
{
	memset(localServerData->serverName, 0, 20);
	memset(localServerData->serverMOTD, 0, 30);
	printf("Enter the server name (19 char limit):\n");
	gets(localServerData->serverName);
	printf("Enter the server message of the day (29 char limit):\n");
	gets(localServerData->serverMOTD);
}

// Used to put data into the LocalClientData struct
void GetClientData(LocalClientData *localClientData)
{
	memset(localClientData->clientName, 0, 20);
	memset(localClientData->clientGreeting, 0, 30);
	printf("Enter the client name (19 char limit):\n");
	gets(localClientData->clientName);
	printf("Enter the client greeting (29 char limit):\n");
	gets(localClientData->clientGreeting);
}

void ShowHelp(void)
{
	// Note that all these commands assume the connection process worked ok.  Since we are doing a local connection
	// it always will and will happen immediately.  In a real game you should wait until the connection is complete.
	printf("Hit 's' to change the local server data on the server.\n");
	printf("Hit 'x' to change the local client data (player 0) on the server.\n");
	printf("Hit 'c' to change the local client data on the client.\n");
	printf("Hit 'y' to change the local server data on the client (pointless).\n");
	printf("Hit 'p' to read the current static data from the server and the client.\n");
	printf("Hit 'h' to show these instructions\n");
	printf("Hit 'q' to quit\n\n");

}
int main(void)
{
	Multiplayer<RakClientInterface> rakClientMultiplayer;
	Multiplayer<RakServerInterface> rakServerMultiplayer;

	RakClientInterface *rakClient=RakNetworkFactory::GetRakClientInterface();
	RakServerInterface *rakServer=RakNetworkFactory::GetRakServerInterface();

	struct LocalServerData localServerData;
	struct LocalClientData localClientData;

	GetServerData(&localServerData);
	GetClientData(&localClientData);

	RakNet::BitStream datBS(50);
	// Save our local data into the server
	localServerData.Serialize(datBS);
	rakServer->SetStaticServerData((char *)datBS.GetData(), datBS.GetNumberOfBytesUsed() );
	// Save our local data into the client. UNASSIGNED_PLAYER_ID specifies we are changing our own data and not that of another player.
	datBS.Reset();
	localClientData.Serialize(datBS);
	rakClient->SetStaticClientData(UNASSIGNED_PLAYER_ID, (char*) datBS.GetData(), datBS.GetNumberOfBytesUsed() );
	char buff[256];
	// Do the connection process.
	printf("Starting server...\n");
	rakServer->Start(32,0,30,2000);

	// If set to false, when a client changes its data and sends it to the server, it will not automatically send this new data to other clients.
	// Defaults to true, unless there are relayStaticClientDataMaxIndex (32) or more allowed players, in which case it defaults to false.
	// You would set this to false if you wanted to relay it manually, such as if you wanted to
	// verify the data before relaying it.
	printf("Server relay client data changes? (y/n)\n");
	if (gets(buff) && (buff[0] =='y'))
		rakServer->SetRelayStaticClientData(true);
	else
		rakServer->SetRelayStaticClientData(false);

	printf("Connecting client...\n");
	rakClient->Connect("127.0.0.1", 2000, 2100, 0, 30);

	ShowHelp();

	char ch=0;
	while (1)
	{
		rakClientMultiplayer.ProcessPackets(rakClient);
		rakServerMultiplayer.ProcessPackets(rakServer);
		if (kbhit()) 
		{
			gets(buff);
			ch = buff[0];
		}

		if (ch=='s')
		{
			GetServerData(&localServerData);
			// Save our local data into the server
			datBS.Reset();
			localServerData.Serialize(datBS);
			rakServer->SetStaticServerData((char *)datBS.GetData(), datBS.GetNumberOfBytesUsed() );
			printf("Relay this new information to all clients (y/n) ?\n");
			if (gets(buff) && (buff[0] =='y'))
			{
				printf("Relaying new server data...\n");
				rakServer->SendStaticServerDataToClient(UNASSIGNED_PLAYER_ID);
			}
			else
				printf("Not relaying new server data.  Clients will still have the old data!\n");

			ch=0;
		}
		else if (ch=='x')
		{
			GetClientData(&localClientData);
			// Save our local data into the server
			datBS.Reset();
			localServerData.Serialize(datBS);
			rakServer->SetStaticClientData(rakServer->GetPlayerIDFromIndex(0), (char*)&localClientData, sizeof(LocalClientData));
			printf("Changed data for player 0.  Relay this new information to all clients (y/n) ?\n");
			if (gets(buff) && (buff[0] =='y'))
			{
				printf("Relaying new client data...\n");
				// The 0 is our one connected client, player 0.  In a real game you would enter the PlayerID of the guy you want to change.
				rakServer->ChangeStaticClientData(rakServer->GetPlayerIDFromIndex(0), UNASSIGNED_PLAYER_ID);
			}
			else
				printf("Not relaying new client data.  Clients will still have the old data!\n");

			ch=0;
		}
		else if (ch=='c')
		{
			GetClientData(&localClientData);
			printf("Player 0 is the copy of our own data from the server.We normally only read this\n");
			printf("UNASSIGNED_PLAYER_ID is our local copy of our own data.\nWe normally only write to this.\n");
			printf("Change this data for player 0 or UNASSIGNED_PLAYER_ID?\n");
			printf("Hit '0' for player 0, anything else for UNASSIGNED_PLAYER_ID\n");
			if (gets(buff) && (buff[0] =='0'))
			{
				datBS.Reset();
				localClientData.Serialize(datBS);
				rakClient->SetStaticClientData(rakClient->GetPlayerID(), (char*) datBS.GetData(), datBS.GetNumberOfBytesUsed() );
				printf("Data changed.\n");
			}
			else
			{
				datBS.Reset();
				localClientData.Serialize(datBS);
				rakClient->SetStaticClientData(UNASSIGNED_PLAYER_ID, (char*) datBS.GetData(), datBS.GetNumberOfBytesUsed() );
				printf("Local data copy changed. Relay this to the server (y/n) ?\n");


				if (gets(buff) && (buff[0] =='y'))
				{
					printf("Relaying new client data...\n");
					// The 0 is our one connected client, player 0.  In a real game you would enter the PlayerID of the guy you want to change.
					rakClient->SendStaticClientDataToServer();
				}
				else
					printf("Not relaying new client data.  The server will still have the old data!\n");
			}

			ch=0;
		}
		else if (ch=='y')
		{
			GetServerData(&localServerData);
			// Save our local data into the client
			datBS.Reset();
			localServerData.Serialize(datBS);
			rakClient->SetStaticServerData((char*)datBS.GetData(), datBS.GetNumberOfBytesUsed());
			printf("Changed our copy of the server data.\n");
			
			ch=0;
		}
		else if (ch=='p')
		{
			// These pointers are used for casting to make data access easier.
			LocalServerData lsd;
			LocalClientData lcd;
			//printf("Player ID: %p\n", rakClient->GetPlayerID() );

			// Local server data on the server
			lsd.Deserialize((char *)rakServer->GetStaticServerData()->GetData());
			printf("Local server data on the server:\n");
			printf("serverName = %s\n", lsd.serverName);
			printf("serverMOTD = %s\n\n", lsd.serverMOTD);
			// Player 0 data on the server
			lcd.Deserialize((char *) rakServer->GetStaticClientData(rakServer->GetPlayerIDFromIndex(0))->GetData());
			printf("Player 0 data on the server:\n");
			printf("clientName = %s\n", lcd.clientName);
			printf("clientGreeting = %s\n\n", lcd.clientGreeting);
			// Server data on the client
			lsd.Deserialize((char *)rakClient->GetStaticServerData()->GetData());
			printf("Local server data on the client:\n");
			printf("serverName = %s\n", lsd.serverName);
			printf("serverMOTD = %s\n\n", lsd.serverMOTD);
			// Local client data on the client
			lcd.Deserialize((char *) rakClient->GetStaticClientData(UNASSIGNED_PLAYER_ID)->GetData());
			printf("Local data on the client:\n");
			printf("clientName = %s\n", lcd.clientName);
			printf("clientGreeting = %s\n\n", lcd.clientGreeting);
			// Player 0 data on the client
			//lcd.Deserialize((char *) rakClient->GetStaticClientData(rakClient->GetPlayerID())->GetData());
			RakNet::BitStream *pbs = rakClient->GetStaticClientData(rakClient->GetPlayerID());
			if (!pbs)
				printf("**** ERROR: incorrect data returned\n");
			else
				lcd.Deserialize((const char *)pbs->GetData());
			printf("Player 0 data on the client:\n");
			printf("clientName = %s\n", lcd.clientName);
			printf("clientGreeting = %s\n\n", lcd.clientGreeting);
			ch=0;
		}
		else if (ch=='h')
		{
			ShowHelp();
			ch=0;
		}
		else if (ch=='q')
			break;

		// Give the network a chance to send its data
#ifdef WIN32
		Sleep(30);
#else
		usleep(30*1000);
#endif
	}

	// Not necessary as disconnections are done in the destructor anyway :)
	rakServer->Disconnect(0);
	rakClient->Disconnect(0);

	RakNetworkFactory::DestroyRakClientInterface(rakClient);
	RakNetworkFactory::DestroyRakServerInterface(rakServer);

	return 0;
}
