
//http://linux.die.net/man/3/mq_notify
#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../gbv.h"
#include "recv_twins.h"
#include <error.h>
#include <err.h>
#include <errno.h>
static mqd_t mq_recv_twins;


//static void * thread_recv (void *);
static void mq_recv_tfunc(union sigval sv);
static void close_recv_twins ();
struct sigevent sev_recv_twins;

int mess_recv_twins ()
{
    fprintf (stderr, "gb_mq_name: %s\n", gb_mq_name);
    struct mq_attr attr;
    mq_recv_twins = mq_open(gb_mq_name, O_RDWR | O_CREAT | O_NONBLOCK, FILE_MODE, 0 );
    if(mq_recv_twins < 0){
         perror("In mq_open()");
        fprintf (stderr, "mess_recv_twins, create error,\n");
        exit(EXIT_FAILURE);
    }
    else{
        fprintf (stderr, "mess_recv_twins: %d, create OK,\n", mq_recv_twins);
    }

    sev_recv_twins.sigev_notify = SIGEV_THREAD;
    sev_recv_twins.sigev_notify_function = mq_recv_tfunc;
    sev_recv_twins.sigev_notify_attributes = 0;
    sev_recv_twins.sigev_value.sival_ptr = &mq_recv_twins;

    while(1)
    {
        if (mq_getattr(mq_recv_twins, &attr) == -1)
        {
            fprintf (stderr, "Error: mq_getattr %x\n",getpid());
        }
        char * buf = malloc(attr.mq_msgsize + 1);
        memset(buf, 0, attr.mq_msgsize + 1);
        int prio;
        int nr = mq_receive(mq_recv_twins, buf, attr.mq_msgsize, &prio);
        if((strcmp(buf, "exit") == 0)){
            fprintf (stderr, "Exit\n");
        }
        if(nr < 0){
            if (mq_notify(mq_recv_twins, &sev_recv_twins) == -1){
                handle_error("mq_notify");
            }
            fprintf (stderr, "Break\n");
            break;
        }
        free(buf);
    }

    while(get_app_run()){
        sleep(2);
    }
    sleep(1);
    return 0;
}

void mq_recv_tfunc(union sigval sv){
    struct mq_attr attr;
    ssize_t nr;
    char *buf;
    mqd_t mqdes = *((mqd_t *) sv.sival_ptr);
    if (mq_getattr(mq_recv_twins, &attr) == -1){
        handle_error("mq_getattr");
    }
    char is_exit = 0;
    while(1)
    {
        if (mq_getattr(mq_recv_twins, &attr) == -1){
            handle_error("mq_getattr");
        }
        buf = (char*)malloc(attr.mq_msgsize + 1);
        memset(buf, 0, attr.mq_msgsize + 1);
        nr = mq_receive(mqdes, buf, attr.mq_msgsize, 0);
        if(nr < 0){
            if (mq_notify(mq_recv_twins, &sev_recv_twins) == -1){
                //Notify error
            }
            break;
        }
        else{
            if((strcmp(buf, CLIENT_CLOSED_CONNECT) == 0)){
                is_exit = 1;
                sk_send_comm(CLIENT_CLOSED_CONNECT);
                break;
            }
            sk_send_comm("data from server\n");
        }
        free(buf);
    }
    if(is_exit){
        set_app_run(0);
        close_recv_twins();
        sleep(2);
        exit(EXIT_SUCCESS);
    }
}
void close_recv_twins (){
    mq_close(mq_recv_twins);
    fprintf (stderr, "mq_unlink: %s\n", gb_mq_name);
    if(mq_unlink(mq_recv_twins)){
        fprintf (stderr, "mq_unlink: %s\n", gb_mq_name);
        perror("In mq_unlink()");
    }else{
        fprintf (stderr, "mq_unlink: OK");
    }
}
