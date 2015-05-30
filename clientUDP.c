/* client code that uses UDP */

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
main(serverIP,portNum,clientIP,inputFile,outputFile,buffSize)
{

}
