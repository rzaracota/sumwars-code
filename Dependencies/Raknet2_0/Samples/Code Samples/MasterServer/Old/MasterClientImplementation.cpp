// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterClientImplementation.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 18, 2003
// Test / example implementation of the Master Client.  This would normally
// be in your game
// ----------------------------------------------------------------------

#include "MasterClient.h"
#include <conio.h>
#include <stdio.h>
#include <Shlwapi.h>
#include "GetTime.h"

void main(void)
{
	char ch=0;
	MasterClient masterClient;
	MasterServerDataStruct *masterServerDataStruct;
	MasterServerDataStruct localMasterServerDataStruct;

	char str[200];
	puts("Enter master server IP or domain name");
	gets(str);

	puts("q to quit, u to upload, r to remove, g to get servers, p to print.");

	while (ch!='q')
	{
		if (kbhit())
			ch=getch();

		if (ch=='p')
		{
			if (masterClient.GetMasterServerDataListSize()==0)
			{
				puts("No servers in the list.");
			}
			else
			{
				if (masterClient.GetNumberOfGameServers()==65535)
					puts("Download still in progress...");
				for (unsigned long i=0; i < masterClient.GetMasterServerDataListSize(); i++)
				{
					masterServerDataStruct = masterClient.GetMasterServerDataAtIndex(i);
					printf("%i/%i. %s:%i %s\n", i+1, masterClient.GetNumberOfGameServers(), masterServerDataStruct->IP,masterServerDataStruct->port,masterServerDataStruct->serverName);
				}
			}

			ch=0;
		}
		else if (ch=='u')
		{
			// Normally the name goes here, but lets put the time for testing purposes
			sprintf(localMasterServerDataStruct.serverName, "Server time = %i", getTime());

			bool b = masterClient.UpdateMasterGameServer(str,MASTER_SERVER_PORT,MASTER_CLIENT_PORT, 2000, &localMasterServerDataStruct);
			if (b==false)
				puts("Update failed");
			else
				puts("Update succeeded");

			ch=0;

		}
		else if (ch=='r')
		{
			if (masterClient.RemoveFromMasterGameServer(str,MASTER_SERVER_PORT,MASTER_CLIENT_PORT, 2000)==false)
				puts("Removal failed");
			else
				puts("Removal succeeded");
			ch=0;
		}
		else if (ch=='g')
		{
			if (masterClient.QueryMasterGameServer(str,MASTER_SERVER_PORT,MASTER_CLIENT_PORT, 2000)==false)
				puts("Download failed");
			else
				puts("Download beginning");
			ch=0;
		}

		masterClient.Update();
		Sleep(30); // Don't waste CPU cycles
	}

	masterClient.Disconnect();
}