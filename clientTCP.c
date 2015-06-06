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

#define	PROTOPORT	33578		/* default protocol port number */
#define DEFAULTBUFFSIZE  1440  /* default buffer size */
static const char DEFAULTINPUTFILE[] = "echoClient.in";
static const char DEFAULTOUTPUTFILE[] = "echoClient.out";


extern	int		errno;
char	localhost[] =	"localhost";	/* default host name		*/
/*------------------------------------------------------------------------
 * Program:   client
 *
 * Purpose:   allocate a socket, connect to a server, and print all output
 *
 * Syntax:    client serverIP, clientIP, portNumber, BufferSize, inputFile, outputFile,
 *
 *		 host  - name of a computer on which server is executing
 *		 port  - protocol port number server is using
 *
 * Note:   required parameters: serverIP, clientIP
 *			if only one file is given then it is consider the inputfile
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
	char *clientIP;
	char *serverIP;
	int buffSize;
	int	n;		 /* number of characters read		*/
	char	buf[buffSize];	 /* buffer for data from the server	*/

	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure	*/
	sad.sin_family = AF_INET;	  /* set family to Internet	*/

	if (argc < 3) // not enough arguments given //
	{
		printf("Not enough argumenets given. \n Arguments are as follow: \nserverIP, clientIP, portNumber, bufferSize, inputFile, outputFile");
	}

	/* Check command-line argument for protocol port and extract	*/
	/* port number if one is specified.  Otherwise, use the default	*/
	/* port value given by constant PROTOPORT			*/

	if (argc > 3) {			/* if protocol port specified	*/
		port = atoi(argv[3]);	/* convert to binary		*/
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

	if (argc > 2) {
		serverIP = argv[1];		/* NOT IN USE YET; MUST IMPLEMENT	*/
		clientIP = argv[2];
	} else {
		host = localhost;
	}
	if (argc > 4)
	{
		buffSize = argv[4];
	}
	else
	{
		buffSize = DEFAULTBUFFSIZE;
	}

	/* Convert host name to equivalent IP address and copy to sad. */

	ptrh = gethostbyname(host); // this should be similar for setting the server IP and client IP
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
	int optval = 1;
	/* Connect the socket to the specified server. */

	if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr,"connect failed\n");
		exit(1);
	}
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);  // BE ABLE TO REUSE ADDRESS
	setsockopt(sd, SOL_SOCKET, SO_SNDBUF, &buffSize, sizeof(buffSize)); // SET THE SEND SIZE OF THE BUFFER
	setsockopt(sd, SOL_SOCKET, SO_RCVBUF, &buffSize, sizeof(buffSize)); // SET THE RECEIVE SIZE OF THE BUFFER


	//reads a file and puts it into a string//
	char *fileContents;
	int inputFileSize;
	FILE *inputFile = fopen("server.c", "rb");
	FILE *outputFile = fopen("output.txt","w");
	if(outputFile == NULL)
	{
		outputFile = fopen("output.txt","a"); // IF THE FILE DOESN'T EXIST
	}
	else
	{
		fprintf(outputFile, ""); 
	}
	outputFile = fopen("output.txt","a");
	fseek(inputFile, 0, SEEK_END);
	inputFileSize = ftell(inputFile); // CHECK THE SIZE OF THE FILE
	rewind(inputFile);
	
	fileContents = malloc((inputFileSize + 1) * (sizeof(char)));
	fread(fileContents, sizeof(char), inputFileSize, inputFile);
	fclose(inputFile);
	fileContents[inputFileSize] = 0;

	int sentBytes = 0;
	int receivedBytes = 0;
	int sizeOfMessage = buffSize;
	char *readBuff = malloc((inputFileSize + 1) * (sizeof(char)));
	printf("THE SIZE OF THE FILE: %i\n", inputFileSize);
	while (receivedBytes < inputFileSize)
	{
	    fd_set readSock; //creates a list of sockets to check if ready to read
	    FD_ZERO(&readSock);
	    FD_SET(sd, &readSock);

	    fd_set writeSock; // creates a list of sockets to check if ready to wrtie
	    FD_ZERO(&writeSock);
	    FD_SET(sd, &writeSock);

	    struct timeval tv;
	    tv.tv_sec = 0;
	    tv.tv_usec = 0;

	    int sent = 0;
		int receive = 0;
	    if (select(sd+1, &readSock, &writeSock, NULL, &tv) > 0) //check if we can read or write
	    {
	    	if(FD_ISSET(sd, &readSock))// CHECKS TO SEE IF WE CAN READ
	    	{
	    		receive = recv(sd, readBuff, sizeof(readBuff), 0);
	        	receivedBytes = receive + receivedBytes;
	        	printf("BYTES SENT: %i\n", sentBytes);
	    		printf("BYTES RECEIVED: %i\n", receivedBytes );
	    		fprintf(outputFile, "%s", readBuff);

	    	}
	    	if(FD_ISSET(sd, &writeSock))// CHECKS TO SEE IF WE CAN WRITE
	    	{
			    if(inputFileSize - sentBytes < buffSize)
				{
					sizeOfMessage = inputFileSize - sentBytes;
				}
				if(sentBytes < inputFileSize)
				{
					sent = send(sd,fileContents + sentBytes,(sizeOfMessage),0);
					sentBytes = sent + sentBytes;
		        	printf("BYTES SENT: %i\n", sentBytes);
		    		printf("BYTES RECEIVED: %i\n", receivedBytes);
				}
				if(sentBytes >= inputFileSize)
	    		{
	    			shutdown(sd, 1);
	    		} 
	    	}
	    }

	}
	fclose(outputFile);
	outputFile = fopen("output.txt","r");
	char c;
	if (outputFile) 
	{
	    while ((c = getc(outputFile)) != EOF) // WRITES FILE FOR DEBUGGING
	        putchar(c);
	    fclose(outputFile);
	}


	/* Close the socket. */

	close(sd);

	/* Terminate the client program gracefully. */

	exit(0);
}









