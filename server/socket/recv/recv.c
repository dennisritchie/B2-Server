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
#include <sys/fcntl.h>
#include <sys/stat.h>

#include "recv.h"
#include "../../message/send/send_twins.h"
int sk_recv_current = -1;
void sk_recv_onus (int sk)
{
//    fprintf (stderr, "sk_recv_onus\n");
    sk_recv_current = sk;
    char buf[MAX_BUFFER];

    do {
		memset(buf, 0, MAX_BUFFER);
		recv(sk, buf, (MAX_BUFFER - 1), 0);
		fprintf (stderr, "--->Data from client: \"%s\"\n", buf);
		int n = strlen(buf);
		if(n > 0){
			shm_twins_send_write(buf);
			shm_twins_send_sig();
			if(strcmp(CLIENT_CLOSED_CONNECT, buf) == 0){
				break;
			}
		}else{
			break;
		}
    } while(1);
//	fprintf (stderr, "45: exit, pid: %d\n", getpid());

	shutdown(sk_recv_current, 2);
	shm_twins_send_unlink();
	sleep(6);
    exit(EXIT_SUCCESS);
}
