// ----------------------------------------------------------------------
// Demonstrates a series of master clients uploading to one master server
// And then downloading and displaying a list.
// ----------------------------------------------------------------------

#include "MasterServer.h"
#include "MasterClient.h"
#include <conio.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void main(void)
{
	MasterServer *masterServer;
	MasterClient *masterClient;
	MasterServerDataStruct localData;
	MasterServerDataStruct *remoteData;
	unsigned long i, lastMasterServerDataListSize;

	masterServer = new MasterServer;
	masterClient = new MasterClient;

	masterServer->Start(false,MASTER_SERVER_PORT);

	// It is important to give time for the master server threads to start before we try uploading.
	// Otherwise the first uploads might fail
#ifdef _WIN32
	Sleep(100);
#else
	usleep(100 * 1000);
#endif

	for (i=0; i < 30; i++)
	{
		printf("Uploading game server #%i\n", i);
		sprintf(localData.serverName, "This is server #%i", i+1);
		if (masterClient->UpdateMasterGameServer("127.0.0.1", MASTER_SERVER_PORT, MASTER_CLIENT_PORT+i, 1000, &localData)==false)
			printf("Update %i failed\n",i);

		// The client will disconnect when done.  So just Update and wait until it is done.
		while (masterClient->IsConnected())
		{
			masterServer->Update();
			masterClient->Update();
#ifdef _WIN32
			Sleep(30);
#else
			usleep(30 * 1000);
#endif
		}
	}
	
	printf("Updates finished\n");

	if (masterClient->QueryMasterGameServer("127.0.0.1", MASTER_SERVER_PORT, MASTER_CLIENT_PORT, 1000)==false)
		printf("Failed to query master game server\n");
	else
	{
		printf("Waiting to receive number of game servers.\n");

		// GetNumberOfGameServers returns 65535 to indicate it doesn't yet know how many servers to
		// download.  In a real application you should let the user cancel out if it never arrives
		while (masterClient->GetNumberOfGameServers()==65535)
		{
			masterServer->Update();
			masterClient->Update();
#ifdef _WIN32
			Sleep(30);
#else
			usleep(30 * 1000);
#endif
		}

		printf("Downloading %i servers.\n",masterClient->GetNumberOfGameServers());

		lastMasterServerDataListSize=0;
		// GetMasterServerDataListSize is how many servers we have already downloaded
		while (masterClient->GetMasterServerDataListSize() < masterClient->GetNumberOfGameServers())
		{
			if (lastMasterServerDataListSize != masterClient->GetMasterServerDataListSize())
			{
				// Note that packets will arrive in bunches, so Got up to server %i / %i probably
				// won't increment sequentially
				lastMasterServerDataListSize=masterClient->GetMasterServerDataListSize();
				printf("Got up to server %i / %i.  Please wait...\n", lastMasterServerDataListSize, masterClient->GetNumberOfGameServers());
			}

			// As before, it is important to keep updating the master server and client
			masterServer->Update();
			masterClient->Update();
#ifdef _WIN32
			Sleep(30);
#else
			usleep(30 * 1000);
#endif
		}

		printf("Got all servers.  Printing list:\n");
		for (i=0; i < masterClient->GetMasterServerDataListSize(); i++)
		{
			remoteData = masterClient->GetMasterServerDataAtIndex(i);
			printf("%i/%i. %s:%i %s\n", i+1, masterClient->GetNumberOfGameServers(), remoteData->IP,remoteData->port,remoteData->serverName);
		}
	}

	printf("No servers in the list.\n");


	delete masterServer;
	delete masterClient;
}