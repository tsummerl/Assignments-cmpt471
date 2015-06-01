/* client.c - code for example client program that uses TCP */

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
 * Purpose:   allocate a socket, connect to a server, and print all output
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
main(argc, argv)
int	argc;
char	*argv[];
{
	int bitSent = 0;
	int bitRecv = 0;

	struct	hostent	 *ptrh;	 /* pointer to a host table entry	*/
	struct	protoent *ptrp;	 /* pointer to a protocol table entry	*/
	struct	sockaddr_in sad; /* structure to hold an IP address	*/
	int	sd;		 /* socket descriptor			*/
	int	port;		 /* protocol port number		*/
	char	*host;		 /* pointer to host name		*/
	int	n;		 /* number of characters read		*/
	char	buf[1000];	 /* buffer for data from the server	*/

	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure	*/
	sad.sin_family = AF_INET;	  /* set family to Internet	*/

	/* Check command-line argument for protocol port and extract	*/
	/* port number if one is specified.  Otherwise, use the default	*/
	/* port value given by constant PROTOPORT			*/

	if (argc > 2) {			/* if protocol port specified	*/
		port = atoi(argv[2]);	/* convert to binary		*/
	} else {
		port = PROTOPORT;	/* use default port number	*/
	}
	if (port > 0)			/* test for legal value		*/
		sad.sin_port = htons((u_short)port);
	else {				/* print error message and exit	*/
		fprintf(stderr,"bad port number %s\n",argv[2]);
		exit(1);
	}

	/* Check host argument and assign host name. */

	if (argc > 1) {
		host = argv[1];		/* if host argument specified	*/
	} else {
		host = localhost;
	}

	/* Convert host name to equivalent IP address and copy to sad. */

	ptrh = gethostbyname(host);
	if ( ((char *)ptrh) == NULL ) {
		fprintf(stderr,"invalid host: %s\n", host);
		exit(1);
	}
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	/* Map TCP transport protocol name to protocol number. */

	if ( ((ptrp = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "cannot map \"tcp\" to protocol number");
		exit(1);
	}

	/* Create a socket. */

	sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sd < 0) {
		fprintf(stderr, "socket creation failed\n");
		exit(1);
	}

	/* Connect the socket to the specified server. */

	if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr,"connect failed\n");
		exit(1);
	}

	int buffSize = 1000;
	char *sendBuf[buffSize];

	//reads a file and puts it into a string//
	char *fileContents;
	long inputFileSize;
	FILE *inputFile = fopen("server.c", "rb");
	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile);
	rewind(inputFile);
	
	fileContents = malloc((inputFileSize + 1) * (sizeof(char)));
	fread(fileContents, sizeof(char), inputFileSize, inputFile);
	fclose(inputFile);
	fileContents[inputFileSize] = 0;
	printf("%s\n",fileContents);

	//starts sending packets///
	int packNum = pktSize(inputFileSize,buffSize);
	printf("TOTAL PACKETS: %i", packNum);
	for(int i = 0; i < packNum; i++)
	{
		printf("\nPACKET: %i\n", i + 1);
		memcpy(sendBuf,&fileContents[i*buffSize],buffSize);
		send(sd,sendBuf,strlen(sendBuf),0);
		//printf("%s\n", sendBuf);
	}





	/* Repeatedly read data from socket and write to user's screen. */

	n = recv(sd, buf, sizeof(buf), 0);
	while (n > 0) {
		write(1,"Client says: ", 13);
		write(1,buf,n);
		n = recv(sd, buf, sizeof(buf), 0);
	}

	/* Close the socket. */

	closesocket(sd);

	/* Terminate the client program gracefully. */

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









