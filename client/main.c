#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
//htonl,htons
#include <strings.h>
//bzero
#include <netdb.h>          /* hostent struct, gethostbyname() */
#include <arpa/inet.h>      /* inet_ntoa() to format IP address */
#include <netinet/in.h>     /* in_addr structure */
#include <pthread.h>
#include <err.h>
#include <errno.h>
#include <error.h>
#include <signal.h>
char *host = "127.0.0.1";
//char *host = "115.77.68.15";
//char *host = "localhost";

#define SERV_PORT                           9999
#define MAX_BUFFER                          1299
//#define CLIENT_CLOSED_CONNECT             "exit"
#define CLIENT_CLOSED_CONNECT               "exit"
//#define _GNU_SOURCE

unsigned char app_exit = 0;

int sk_send = -1;
static void * part_send(void* param);
static void part_send_go();

int sk_recv = -1;
static void * part_recv(void* param);
static void part_recv_go();
char * mq_key_server;
pthread_t p_send_id, p_recv_id;

void sighand(int signo);
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

int main (int argc, char * argv[])
{
//	daemonize();
	struct sigaction        actions;
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_flags = 0;
	actions.sa_handler = sighand;

	int rc = sigaction(SIGALRM,&actions,0);

    //$ cat /proc/sys/net/ipv4/tcp_fin_timeout
    printf("argv[0]: %s - argc: %d\n", argv[0], (int)argc);    
    part_recv_go();
    while(!app_exit){
        sleep(1);
    }
    return EXIT_SUCCESS;
}
/**
  Part send
  **/
void * part_send(void* param){

    struct sockaddr_in dest;
    memset(&dest, (int)0, (size_t)sizeof(dest));
    sk_send = socket (AF_INET, SOCK_STREAM, 0);

    dest.sin_family = AF_INET;    /* select the desired network */
    dest.sin_port = htons(SERV_PORT);        /* select the port */
    //inet_aton(host, &dest.sin_addr);          /* remote address */
    dest.sin_addr.s_addr = inet_addr(host);
    if ( connect(sk_send, &dest, sizeof(dest)) != 0 )    /* connect! */
    {
        return 0;
    }
    perror("--->Socket connection");
    int i = 0;
    send(sk_send, mq_key_server, strlen(mq_key_server), 0);
    char message [MAX_BUFFER];
    memset(message, 0,  MAX_BUFFER);
    recv(sk_send, message, MAX_BUFFER - 1, 0);

    fprintf (stderr, "--->Init mess: \"%s\"\n", message);

    while(i < 60) {
        memset(message, 0,  MAX_BUFFER);
        sprintf(message, "%s Index: %d", "From send client socket.", i + 1);
        sleep(1);
        sleep(1);
        send(sk_send, message, strlen(message), 0);
        ++i;

    }
    send(sk_send, CLIENT_CLOSED_CONNECT, strlen(CLIENT_CLOSED_CONNECT), 0);
    fprintf (stderr, "close send socket\n");

    shutdown(sk_send, 2);
    shutdown(sk_recv, 2);

    sk_send = -1    ;
    exit(EXIT_SUCCESS);
}
static void part_send_go(){
    //http://linux.die.net/man/3/pthread_create
    pthread_t pthr_id = -1;
    pthread_attr_t attr;
    int s = pthread_attr_init(&attr);
    if (s != 0)
    {
        perror("pthread_attr_init");
        exit(1);
    }
    pthread_create(&pthr_id, &attr, part_send, 0);
    p_send_id = pthr_id;
//    pthread_create()
}
/**
  Part recv
  **/
static void * part_recv(void* param)
{
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    sk_recv = socket (AF_INET, SOCK_STREAM, 0);

    dest.sin_family = AF_INET;    /* select the desired network */
    dest.sin_port = htons(SERV_PORT);        /* select the port */
    //inet_aton(host, &dest.sin_addr);          /* remote address */
    dest.sin_addr.s_addr = inet_addr(host);
    int rs = connect(sk_recv, &dest, sizeof(dest));
    if ( rs != 0 )    /* connect! */
    {
    	fprintf(stderr, "error %d", errno);
        return 0;
    }
    fprintf (stderr, "Port: %d\n", dest.sin_port);
    perror("--->Socket connection");
//    int i = 0;
    char message [MAX_BUFFER];
    memset(message, 0, MAX_BUFFER);
    sprintf(message, "0%s", "from recv client;");
    send(sk_recv, message, strlen(message), 0);
    memset(message, 0, MAX_BUFFER);
    recv(sk_recv, message, MAX_BUFFER - 1, 0);
    fprintf (stderr, "--->Init mess: \"%s\"\n", message);
    //sleep(2);
    int n = strlen(message);
    mq_key_server = malloc(n+2);
    memset(mq_key_server, 0, n+2);
    mq_key_server[0] = '1';
    memcpy(&mq_key_server[1], message, n);
    sleep(1);
    part_send_go();

    while(1){
        memset(message, 0, MAX_BUFFER);
        recv(sk_recv, message, (MAX_BUFFER - 1), 0);      
        fprintf (stderr, "--->From server: \"%s\"\n", message);
        if(strcmp(message, CLIENT_CLOSED_CONNECT) == 0){
            break;
        }
        else if(strlen(message) == 0){
            break;
        }
        else{
        	pthread_kill(p_send_id, SIGALRM);
        }
    }
    shutdown(sk_send, 2);
    exit(EXIT_SUCCESS);
}
static void part_recv_go()
{
    //http://linux.die.net/man/3/pthread_create
    pthread_t pthr_id = -1;
    pthread_attr_t attr;
    int s = pthread_attr_init(&attr);
    if (s != 0)
    {
        perror("pthread_attr_init");
        exit(1);
    }
    pthread_create(&pthr_id, &attr, part_recv, 0);
}
void sighand(int signo)
{
	pthread_t             self = pthread_self();
	fprintf(stdout, "--->Sighand Pthread ID: %ld\n", self);
	fprintf(stdout, "--->Read data from data link list\n");
	return;
}
