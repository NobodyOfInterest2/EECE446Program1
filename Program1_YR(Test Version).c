/* This code is an updated version of the sample code from "Computer Networks: A Systems
 * Approach," 5th Edition by Larry L. Peterson and Bruce S. Davis. Some code comes from
 * man pages, mostly getaddrinfo(3). */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

/*
 * Lookup a host IP address and connect to it using service. Arguments match the first two
 * arguments to getaddrinfo(3).
 *
 * Returns a connected socket descriptor or -1 on error. Caller is responsible for closing
 * the returned socket.
 */
int lookup_and_connect( const char *host, const char *service );

int main( ) {
	int s;
	const char *host = "www.ecst.csuchico.edu";
	const char *port = "80";

	/* Lookup IP and connect to server */
	if ( ( s = lookup_and_connect( host, port ) ) < 0 ) {
		exit( 1 );
	}

	/* Modify the program so it
	 * 1) read the chunk size from the command line arguments
	 * 2) connects to www.ecst.csuchico.edu on port 80 (mostly done above)
	 * 3) sends "GET /~kkredo/file.html HTTP/1.0\r\n\r\n" to the server
	 * 4) receives and process the file
      * a) receive a chunk of data
      * b) count the h1 tags in the chunk
      * c) count the bytes received
      * d) repeat until the file is done
 	 * 5) prints the number of <h1> tags and total bytes received
	 *
	 * */
	char *msg = "GET /~kkredo/file.html HTTP/1.0\r\n\r\n";
        int len, bytes_sent;
	len = strlen(msg);
	bytes_sent = send(s, msg, len, 0);
	printf("Bytes Sent: %d\n", bytes_sent); //How many bytes did we send getting the file?

	char re[1000];
	int reLen = 1000, bytes_recv = 0, h1_recv = 0, temp = 0, chunk = 0;
	//char *header = "<h1>";
  	scanf("%d\n", chunk);                  //Get input from the command line specifying chunk size

	while(1){                              //Continuously receive bytes
	 	temp = bytes_recv;                   //Initialize our bytes recieved counter
	  	bytes_recv += recv(s, re, reLen, 0); //Increment our bytes recieved counter
		
		char *toSearch = re;
		toSearch = strstr( toSearch, header );
		while( toSearch != NULL)
		{
			h1_recv++;
			toSearch += 4;
			toSearch = strstr( toSearch, header );
		}
		/*
		for(int i = 0; i < 1000; i+){       //Count the <h1> tags in the chunk
      		if(re[i] == '<'){                  //Condition evaluates to true if we have a < in the chunk
				if(re[i+1] == 'h'){							 //Condition evaluates to true if the next char is an h
					if(re[i+2] == '1'){						 //Condition evaluates to true if the next char is a 1
						if(re[i+3] == '>'){					 //Condition evaluates to true if the next char is a >
							h1_recv++;                 //Increment h1_rev counter, we have a valid <h1> in our chunk
						}
					}
				}
			}
    	}																		 //Process the whole re chunk we just received
		*/
	  	if(bytes_recv == temp){break;}       //Evaluates to true if we tried to recieve more bytes and didn't
	}                                      //End the while loop because there is nothing left to recieve
	printf("Number of <h1> tags: %d\n", h1_recv); //Print out how many <h1> tags we counted
	printf("Number of bytes: %d\n", bytes_recv); //Print out how many bytes we recieved in total
	close( s );

	return 0;
}

//Function that calls getaddrinfo, and connects to a socket
int lookup_and_connect( const char *host, const char *service ) {
	struct addrinfo hints;
	struct addrinfo *rp, *result;
	int s;

	/* Translate host name into peer's IP address */
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	if ( ( s = getaddrinfo( host, service, &hints, &result ) ) != 0 ) {
		fprintf( stderr, "stream-talk-client: getaddrinfo: %s\n", gai_strerror( s ) );
		return -1;
	}

	/* Iterate through the address list and try to connect */
	for ( rp = result; rp != NULL; rp = rp->ai_next ) {
		if ( ( s = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol ) ) == -1 ) {
			continue;
		}

		if ( connect( s, rp->ai_addr, rp->ai_addrlen ) != -1 ) {
			break;
		}

		close( s );
	}
	if ( rp == NULL ) {
		perror( "stream-talk-client: connect" );
		return -1;
	}
	freeaddrinfo( result );

	return s;
}
