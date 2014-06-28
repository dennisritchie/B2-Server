#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
//htonl,htons
#include <strings.h>
//bzero
#include <pthread.h>
#include <netdb.h>          /* hostent struct, gethostbyname() */
#include <arpa/inet.h>      /* inet_ntoa() to format IP address */
#include <netinet/in.h>     /* in_addr structure */
#include <sys/stat.h>

#include "send.h"
#include "../../message/recv/recv_twins.h"
static int sk_send_current = -1;

void sk_send_onus (int sk)
{
    sk_send_current = sk;    
}
int sk_send_comm (const char* command)
{
    fprintf (stderr, "__________I enter: %d\n", sk_send_current);

    if(sk_send_current < 0)
        return -1;
    int n = send(sk_send_current, command, strlen(command), 0);
    if(n > 0){
//        fprintf (stderr, "I sk_send_current: %s\n", command);
    }
    else{
        fprintf (stderr, "____I sk_send_current: ERROR\n");
    }
    if(strcmp(command, CLIENT_CLOSED_CONNECT) == 0){
        close(sk_send_current);
    }
    return 0;
}
int sk_send_data (const char* data)
{
    int n = send(sk_send_current, data, strlen(data), 0);
	return n;
}
int sk_send_shutdown(){
	return shutdown(sk_send_current, 2);
}
