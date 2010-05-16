// ----------------------------------------------------------------------
// RakNet version 1.032
// MasterServerImplementation.cpp
// Created by Rakkar Software (rakkar@rakkarsoftware.com) February 18, 2003
// Test / example implementation of the Master Server
// ----------------------------------------------------------------------

#include "MasterServer.h"
#include <conio.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void main(void)
{
	char ch=0;
	MasterServer masterServer;
	MasterServerDataStruct *masterServerDataStruct;

	puts("q to quit, p to print the server list.");

	// Test with low priority to make sure it works
	masterServer.Start(false,MASTER_SERVER_PORT);

	while (ch!='q')
	{
		if (kbhit())
			ch=getch();

		if (ch=='p')
		{
			if (masterServer.GetMasterServerDataListSize()==0)
			{
				puts("No servers in the list.");
			}
			else
			{
				for (unsigned long i=0; i < masterServer.GetMasterServerDataListSize(); i++)
				{
					masterServerDataStruct = masterServer.GetMasterServerDataAtIndex(i);
					printf("%i. %s:%i %s\n", i, masterServerDataStruct->IP,masterServerDataStruct->port,masterServerDataStruct->serverName);
				}
			}

			ch=0;
		}

		masterServer.Update();
#ifdef _WIN32
		Sleep(30);
#else
		usleep(30 * 1000);
#endif
	}

	masterServer.Disconnect();
}