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
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include "def.h"
#include "socket/recv/recv.h"
#include "socket/send/send.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <linux/sockios.h>

char                                       APPLICATION_RUNNING = 1;
static pthread_mutex_t                     APP_MUTEX_RUN = PTHREAD_MUTEX_INITIALIZER;
static void daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */

    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}

static void catch_message();
static char *ethernet_mactoa(struct sockaddr *addr);
int  main(int argc, char **argv)
{
//	daemonize();

    printf("argv[0]: %s - argc: %d\n", argv[0], (int)argc);
    int listen_sk, con_sk = 0;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listen_sk = socket (AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);

    bind( listen_sk, (struct sockaddr_in *) &servaddr, sizeof(servaddr));
    listen(listen_sk, LISTENQ);

    while(1){
        clilen = sizeof(cliaddr);
        struct arpreq arpreq_;
        bzero(&arpreq_, sizeof(struct arpreq));
        con_sk = accept( (int)listen_sk, (struct  sockaddr_in  *)&cliaddr, &clilen);
        fprintf (stderr, "IP: %x\n", cliaddr.sin_addr.s_addr);
        fprintf (stderr, "Port: %d\n", cliaddr.sin_port);
        if( ( ioctl(con_sk, SIOCGARP, &arpreq_) ) < 0 ){
        	perror("ioctl");
        }
        {
            unsigned char *ptr = &arpreq_.arp_ha.sa_data[0];
            printf("MAC: %x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1), *(ptr+2), *(ptr+3),
            *(ptr+4), *(ptr+5));
        }

        if ( fork()== 0){
        	//We should break here;
            //catch_message(con_sk);
            //Should BREAK;?
        	break;
        }
        else{
            close(con_sk);
            //exit(0);
        }
    }
    //shutdown(listen_sk, 2);
    //And we should catch
    if( con_sk > 0){
    	catch_message(con_sk);
    }
    else{
    	shutdown(listen_sk, 2);
    	exit(0);
    }
    //while(get_app_run()){
    while(1){
    	sleep(1);
    }
    exit (0);
}

void set_app_run(char run){
    pthread_mutex_lock(&APP_MUTEX_RUN);
    APPLICATION_RUNNING = run;
    pthread_mutex_unlock(&APP_MUTEX_RUN);
}
char get_app_run(){
    char rs = 0;
    pthread_mutex_lock(&APP_MUTEX_RUN);
    rs = APPLICATION_RUNNING;
    pthread_mutex_unlock(&APP_MUTEX_RUN);
    return rs;
}
#include "shm/twins/shm_twins_recv.h"
#include "shm/twins/shm_twins_send.h"
void catch_message(int con_sk)
{
    char message[MAX_BUFFER];
    memset( (void*)message, 0,(size_t) MAX_BUFFER);
    recv(con_sk, message, (MAX_BUFFER - 1), 0);
    fprintf (stderr, "--->Init mess: \"%s\"\n", message);

    if(message[0] == '0'){
    	shm_twins_recv_init();
    	char * key = get_shm_twins_recv_key();
        send((int)con_sk, key, strlen(key), 0);
        sk_send_onus((int)con_sk);
    }
    else if (message[0] == '1')
    {
        shm_twins_send_init(&(message[1]));
        send(con_sk, "0", strlen("0"), 0);
        sk_recv_onus(con_sk);
    }

}
static char *ethernet_mactoa(struct sockaddr *addr) {
static char buff[256];

	unsigned char *ptr = (unsigned char *) addr->sa_data;

	sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
		(ptr[0] & 0xff), (ptr[1] & 0xff), (ptr[2] & 0xff),
		(ptr[3] & 0xff), (ptr[4] & 0xff), (ptr[5] & 0xff));

return (buff);

}
