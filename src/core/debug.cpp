#include "debug.h"

void hexwrite(char* data,int len)
{
	unsigned char* ucp;
	char* cp;
	char a,b;
	int i=0;
	for (cp= data;cp <data+len;cp++)
	{
		ucp = (unsigned char*) cp;
		a= (*ucp)/16;
		b= (*ucp)%16;
		if (a>9)
			printf("%c",a-10+'a');
		else
			printf("%c",a+'0');

		if (b>9)
			printf("%c",b-10+'a');
		else
			printf("%c",b+'0');
			
		printf(" ");
		i++;
		if (i%4==0)
		{
			printf("| ");
		}
	
	}
	printf("\n");
}

