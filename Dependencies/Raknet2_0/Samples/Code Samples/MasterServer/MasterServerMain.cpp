// This is my own internal test program for the master server but serves as a good example.

#include "MasterCommon.h"
#include "MasterServer.h"
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <conio.h>
#else
#include "../Unix/kbhit.h"
#endif

#define READCHAR(arg) gets(arg); ch=arg[0];

int main(void)
{
	MasterServer masterServer;
	int serverListSize;
	const char *outputString;
	int outputInt;
	bool identiferFound;
	int index;
	char ch;

	if (masterServer.Initialize(10, 60000, 0))
		printf("Master server started.\n");
	else
		printf("Master server failed to start.\n");

	printf("(p)rint\n(q)uit\n");
	char buff[256];

	while (1)
	{
		if (kbhit())
		{
			READCHAR(buff);
			if (ch=='q')
				break;
			else if (ch=='p')
			{
				serverListSize=masterServer.GetServerListSize();
				if (serverListSize==0)
				{
					printf("No servers in list\n");
				}
				else
				{
					for (index=0; index < serverListSize; index++)
					{
						printf("%i. ", index);
						outputString=masterServer.GetServerListRuleAsString(index, "IP", &identiferFound);
						if (identiferFound)
							printf("%s:", outputString);
						else
							printf("NO_IP:");
						outputInt=masterServer.GetServerListRuleAsInt(index, "Port", &identiferFound);
						if (identiferFound)
							printf("%i ", outputInt);
						else
							printf("NO_PORT ");
						outputString=masterServer.GetServerListRuleAsString(index, "Game type", &identiferFound);
						if (identiferFound)
							printf("%s ", outputString);
						else
							printf("NIL_GT ");
						outputString=masterServer.GetServerListRuleAsString(index, "Game name", &identiferFound);
						if (identiferFound)
							printf("%s ", outputString);
						else
							printf("NIL_GN ");
						outputInt=masterServer.GetServerListRuleAsInt(index, "Score", &identiferFound);
						if (identiferFound)
							printf("%i\n", outputInt);
						else
							printf("NO_SCORE\n");
					}
				}
			}
			ch=0;
		}
		masterServer.Update();
	}

	return 0;
}
