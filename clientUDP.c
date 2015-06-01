/* client code that uses UDP */

typedef int bool;
enum { false, true };

#define closesocket close
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define	PROTOPORT	5193		/* default protocol port number */

extern	int		errno;
char	localhost[] =	"localhost";	/* default host name		*/
/*------------------------------------------------------------------------
 * Program:   client
 *
 * Purpose:   opens a file, treats the contents of the file as a data stream, 
 			and transmits the contents of that data stream as the data within 
 			a sequence of UDP packets.
 *
 * Syntax:    client [ host [port] ]
 *
 *		 host  - name of a computer on which server is executing
 *		 port  - protocol port number server is using
 *
 * Note:      Both arguments are optional.  If no host name is specified,
 *	      the client uses "localhost"; if no protocol port is
 *	      specified, the client uses the default given by PROTOPORT.
 *
 *------------------------------------------------------------------------
 */

main(int argc, char *argv[])

{
	int buffSize = 1000;
	char *sendBuf[buffSize];
	if(argc > 1)
	{
		//reads a file and puts it into a string//
		char *fileContents;
		long inputFileSize;
		FILE *inputFile = fopen(argv[1], "rb");
		fseek(inputFile, 0, SEEK_END);
		inputFileSize = ftell(inputFile);
		rewind(inputFile);
		
		fileContents = malloc((inputFileSize + 1) * (sizeof(char)));
		fread(fileContents, sizeof(char), inputFileSize, inputFile);
		fclose(inputFile);
		fileContents[inputFileSize] = 0;
		printf("%s\n",fileContents);

		//starts sending packets///
		// int packNum = pktSize(inputFileSize,buffSize);
		// for(int i = 0; i < packNum; i++)
		// {
		// 	printf("\nPACKET: %i\n", i+1);
		// 	memcpy(sendBuf,&fileContents[i*buffSize],buffSize);
		// 	printf("%s", sendBuf);
		// }


	}

	
	exit(0);
}

int pktSize(int fileSize, int buffSize)
{
	printf("%i\n", fileSize);
	printf("%i\n", buffSize);

	if(fileSize % buffSize == 0)
	{
		return fileSize/buffSize;
	}
	else
	{
		return (fileSize/buffSize) + 1;
	}
}

