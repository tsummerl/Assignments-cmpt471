/* server.c - code for example server program that uses TCP */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define	PROTOPORT	33578	/* default protocol port number */
#define	QLEN		6		/* size of request queue	*/

int	visits	    =   0;		/* counts client connections	*/
/*------------------------------------------------------------------------
 * Program:   server
 *
 * Purpose:   allocate a socket and then repeatedly execute the following:
 *		(1) wait for the next connection from a client
 *		(2) send a short message to the client
 *		(3) close the connection
 *		(4) go back to step (1)
 *
 * Syntax:    server [ port ]
 *
 *		 port  - protocol port number to use
 *
 * Note:      The port argument is optional.  If no port is specified,
 *	      the server uses the default given by PROTOPORT.
 *
 *------------------------------------------------------------------------
 */
main(argc, argv)
int	argc;
char	*argv[];
{
	struct	hostent	 *ptrh;	 /* pointer to a host table entry	*/
	struct	protoent *ptrp;	 /* pointer to a protocol table entry	*/
	struct	sockaddr_in sad; /* structure to hold server's address	*/
	struct	sockaddr_in cad; /* structure to hold client's address	*/
	int	sd, sd2;	 /* socket descriptors			*/
	int	port;		 /* protocol port number		*/
	int	alen;		 /* length of address			*/
	char	buf[1000];	 /* buffer for string the server sends	*/

	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure	*/
	sad.sin_family = AF_INET;	  /* set family to Internet	*/
	sad.sin_addr.s_addr = INADDR_ANY; /* set the local IP address	*/

	/* Check command-line argument for protocol port and extract	*/
	/* port number if one is specified.  Otherwise, use the	default	*/
	/* port value given by constant PROTOPORT			*/

	if (argc > 1) {			/* if argument specified	*/
		port = atoi(argv[1]);	/* convert argument to binary	*/
	} else {
		port = PROTOPORT;	/* use default port number	*/
	}
	if (port > 0)			/* test for illegal value	*/
		sad.sin_port = htons((u_short)port);
	else {				/* print error message and exit	*/
		fprintf(stderr,"bad port number %s\n",argv[1]);
		exit(1);
	}

	/* Map TCP transport protocol name to protocol number */

	if ( ((ptrp = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "cannot map \"tcp\" to protocol number");
		exit(1);
	}

	/* Create a socket */

	sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	int optval = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	if (sd < 0) {
		fprintf(stderr, "socket creation failed\n");
		exit(1);
	}

	/* Bind a local address to the socket */

	if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr,"bind failed\n");
		exit(1);
	}

	/* Specify size of request queue */

	if (listen(sd, QLEN) < 0) {
		fprintf(stderr,"listen failed\n");
		exit(1);
	}

	/* Main server loop - accept and handle requests */

	while (1) 
	{
		alen = sizeof(cad);
		if ( (sd2=accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
			fprintf(stderr, "accept failed\n");
			exit(1);
		}
		int bytesReceived = 0;
		int sentBytes = 0;
		int receivedBytes = 0;		    
		int sent = 0;
		int receive = 0;

		while (1)
		{
		    fd_set readSock; //creates a list of sockets to check if ready to read
		    FD_ZERO(&readSock);
		    FD_SET(sd2, &readSock);

		    fd_set writeSock; // creates a list of sockets to check if ready to wrtie
		    FD_ZERO(&writeSock);
		    FD_SET(sd2, &writeSock);

		    struct timeval tv;
		    tv.tv_sec = 0;
		    tv.tv_usec = 0;



		    if (select(sd2+1, &readSock, &writeSock, NULL, &tv) > 0)
		    {
		    	if(FD_ISSET(sd2, &writeSock))
		    	{
		    		sent = send(sd2,buf,receive,0);
					sentBytes = sent + sentBytes;
		        	printf("BYTES SENT: %i\n", sentBytes);
		    		printf("BYTES RECEIVED: %i\n", receivedBytes);
		    	}
			    if(FD_ISSET(sd2, &readSock))// modifies the list of available sockets to read
		    	{
		    		receive = recv(sd2, buf, sizeof(buf), 0);
		        	receivedBytes = receive + receivedBytes;
		        	printf("BYTES SENT: %i\n", sentBytes);
		    		printf("BYTES RECEIVED: %i\n", receivedBytes);
		    		if(receive == 0)
		    		{
		    			close(sd2);
		    		}
		    	}
		    }
		    else
		    {
		    	receive = recv(sd2, buf, sizeof(buf), 0);
		    	if(receive == 0)
		    	{
		    		close(sd2);
		    	}
		    }
		}
		exit(0);
		// while(1)
		// {
		// 	int bytesInPac = recv(sd2,buf,1000,0);
		// 	bytesReceived +=bytesInPac;
		// 	printf("%s",buf);
		// 	if(bytesInPac == 0)
		// 	{
		// 		close(sd2);
		// 	}
		// 	printf("BYTES RECEIVED: %i\n",bytesReceived);
		// }
	}
}