#include "Autopatcher.h"
#include "RakNetworkFactory.h"
#include "GetTime.h"
#include "RakPeerInterface.h"
#include "PacketEnumerations.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h> // Sleep
#else
#include <unistd.h> // usleep
#endif

// Function prototype
unsigned int ShowDownloadStatus(AutoPatcher *autoPatcher);

int main(void)
{
	RakPeerInterface *rakPeer;
	bool isServer;
	AutoPatcher autoPatcher;
	Packet *packet;
	unsigned int time,runTime;
	char input[255];

	// No benefit to using RakServer and RakClient for this because we don't
	// care about other connections and ping relaying and such.
	rakPeer = RakNetworkFactory::GetRakPeerInterface();
	rakPeer->SetOccasionalPing(true);

	// The autopatcher needs to have access to the pointer so it can send packets
	autoPatcher.SetNetworkingSystem(rakPeer);

	// Just provides a way to quit without using kbhit() :)
	printf("How many seconds do you want to run for?\n");
	gets(input);
	runTime = atoi(input);

	printf("(B)roadband or (D)ialup?\n");
	gets(input);
	if (input[0]=='b' || input[0]=='B')
		rakPeer->SetMTUSize(1400); // This will triple transfer speed of big files :)
    
	printf("Enter s to act as a server, c to act as a client\n");
	gets(input);
	if (input[0]=='s' || input[0]=='S')
	{
		isServer=true;

		// Up to 8 connections, local port of 60000, thread priority 0
		if (rakPeer->Initialize(8,60000,0)==false)
		{
			printf("Failed to initialize network system.\n");
			RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
			return 0;
		}

		rakPeer->SetMaximumIncomingConnections(8);

		printf("Enter name of first file to make downloadable or hit enter for none\n");
		gets(input);

		if (input[0])
		{
			// CreateFileSignature is here as an example.
			// It creates the file signature for the file which is then read in
			// SetFileDownloadable (second param is true)
			// In reality you would have a different program create file signatures
			// and then check those in your game
			if (AutoPatcher::CreateFileSignature(input)==false)
				printf("Unable to create file signature %s.sha\n", input);

			if (autoPatcher.SetFileDownloadable(input, true)!=SET_FILE_DOWNLOADABLE_SUCCESS)
				printf("Unable to make %s downloadable\n", input);
		}

		printf("Enter name of second file to make downloadable or hit enter for none\n");
		gets(input);
		if (input[0] && autoPatcher.SetFileDownloadable(input, false)!=SET_FILE_DOWNLOADABLE_SUCCESS)
			printf("Unable to make %s downloadable\n", input);

		printf("Server active, waiting for connections...\n");
	}
	else
	{
		isServer=false;

		// Up to 1 connection, local port of 60001, thread priority 0
		if (rakPeer->Initialize(1,60001,0)==false)
		{
			printf("Failed to initialize network system.\n");
			RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
			return 0;
		}

		rakPeer->SetMaximumIncomingConnections(0);

		printf("Enter IP to connect to.\n");
		gets(input);
		if (rakPeer->Connect(input, 60000, 0, 0)==false)
		{
			printf("Call to connect failed.\n");
			RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
			return 0;
		}

		printf("Enter directory to put all downloads in or just hit enter for none\n");
		gets(input);
		autoPatcher.SetDownloadedFileDirectoryPrefix(input);

		printf("Please wait for connection...\n");
	}

	time = RakNet::GetTime();
	// Run for x seconds.  Change this to while(1) to run forever.
	while (RakNet::GetTime() < time + runTime*1000)
	{
		packet=rakPeer->Receive();

		if (packet)
		{
			// Parse the packet identifiers we care about
			switch(packet->data[0])
			{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Connection request accepted\n");
				// Start the chain by requesting the list of files from the server
				autoPatcher.RequestDownloadableFileList(packet->playerId);
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("New incoming connection\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("Disconnected\n");
				break;
			case ID_CONNECTION_LOST:
				printf("Connection lost\n");
				break;
			case ID_REMOTE_PORT_REFUSED:
				printf("Write to remote port refused\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("No free incoming connections\n");
				break;
			case ID_AUTOPATCHER_REQUEST_FILE_LIST:
				printf("Got a request for the file list\n");
				autoPatcher.SendDownloadableFileList(packet->playerId);				
				break;
			case ID_AUTOPATCHER_FILE_LIST:
				printf("\n-------\nGot the list of available server files.\nRequesting downloads.\n-------\n");
				autoPatcher.OnAutopatcherFileList(packet, true);
				if (ShowDownloadStatus(&autoPatcher)==0)
				{
					rakPeer->DeallocatePacket(packet);
					goto QUIT;
				}
				break;
			case ID_AUTOPATCHER_REQUEST_FILES:
				printf("Got a request for files\n");
				autoPatcher.OnAutopatcherRequestFiles(packet);
				break;
			case ID_AUTOPATCHER_SET_DOWNLOAD_LIST:
				printf("* Confirmed download list\n");
				autoPatcher.OnAutopatcherSetDownloadList(packet);
				break;
			case ID_AUTOPATCHER_WRITE_FILE:
				printf("-------\nGot a file\n-------\n");
				autoPatcher.OnAutopatcherWriteFile(packet);
				if (ShowDownloadStatus(&autoPatcher)==0)
				{
					rakPeer->DeallocatePacket(packet);
					goto QUIT;
				}
				break;
			}

			rakPeer->DeallocatePacket(packet);
		}

#ifdef _WIN32
		Sleep(30);
#else
		usleep(30 * 1000);
#endif
	}

QUIT:

	printf("Quitting.\n");
	RakNetworkFactory::DestroyRakPeerInterface(rakPeer);

	return 0;
}

unsigned int ShowDownloadStatus(AutoPatcher *autoPatcher)
{
	char fileName[256];
	unsigned numberOfDownloadingFiles;
	unsigned fileLength, compressedFileLength;
	bool dataIsCompressed;

	numberOfDownloadingFiles = autoPatcher->GetDownloadStatus(fileName, &fileLength, &dataIsCompressed, &compressedFileLength);
	if (numberOfDownloadingFiles>0)
	{
		printf("\n%i files on the download list\n", numberOfDownloadingFiles);
		printf("Current file: %s (%i bytes)\n", fileName, fileLength);
		if (dataIsCompressed==false)
			printf("File was transmitted uncompressed\n");
		else
			printf("File was transmitted compressed at %i bytes\n",compressedFileLength);
	}
	else
		printf("Not currently downloading any files.\n");	

	return numberOfDownloadingFiles;
}
