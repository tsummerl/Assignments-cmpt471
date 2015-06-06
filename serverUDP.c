/* server.c - code for example server program that uses TCP */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define	PROTOPORT	33516	/* default protocol port number */
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

	int	sd;	 /* socket descriptors			*/
	int	port;		 /* protocol port number		*/
	int	alen;		 /* length of address			*/
	char	buf[1000];	 /* buffer for string the server sends	*/

	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure	*/
	sad.sin_family = AF_INET;	  /* set family to Internet	*/
	sad.sin_addr.s_addr = htonl(INADDR_ANY); /* set the local IP address	*/

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

	if ( ((ptrp = getprotobyname("udp"))) == 0) {
		fprintf(stderr, "cannot map \"udp\" to protocol number");
		exit(1);
	}

	/* Create a socket */

	sd = socket(PF_INET, SOCK_DGRAM, ptrp->p_proto);
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

	/* Main server loop - accept and handle requests */
	alen = sizeof(cad);
	while (1) 
	{
	
		int bytesReceived = 0;
		int sentBytes = 0;
		int receivedBytes = 0;		    
		int sent = 0;
		int receive = 0;

		while (1)
		{
		    fd_set readSock; //creates a list of sockets to check if ready to read
		    FD_ZERO(&readSock);
		    FD_SET(sd, &readSock);

		    struct timeval tv;
		    tv.tv_sec = 0;
		    tv.tv_usec = 0;

		    if (select(sd+1, &readSock, NULL, NULL, &tv) > 0)
		    {
			    if(FD_ISSET(sd, &readSock))// modifies the list of available sockets to read
		    	{
		    		receive = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&cad, alen);
		    		if(receive == -1)
		    		{
						printf("ERROR IN RECEIVING! %s\n", strerror(errno));
						exit(1);
		    		}
		        	receivedBytes = receive + receivedBytes;
		        	sent = sendto(sd,buf,receive,0, (struct sockaddr *)&cad, sizeof(cad));
		    		printf("BYTES RECEIVED: %i\n", receivedBytes);
		    		printf("PORT NUMBER: %i\n", port);
		    		printf("ADDRESS OF SENDER: [%s]", inet_ntoa(cad.sin_addr));


		    	}
		    }
		}
		exit(0);
	}
}

