//http://linux.die.net/man/3/mq_notify
#include <pthread.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <error.h>
#include "send_twins.h"
#include "../../gbv.h"
static mqd_t mq_send_twins;
char *gb_mq_name;
static struct sigevent sev_send_twins;
int open_mess_twins (){
    mq_send_twins = mq_open(gb_mq_name, O_RDWR | O_CREAT  , FILE_MODE, 0 );
    return -1;
}
int close_mess_twins (){
    mq_close(mq_send_twins);
    return -1;
}
int mess_send_twins (const char* message){
    char rs = 0;
    int s = mq_send(mq_send_twins, message, strlen(message), 30);
    if(s == 0){
    }
    else{
        fprintf (stderr,"mq_send_twins: sent error %d : ...%s...\n",errno,strerror (errno));
    }
    if(s<0){
        rs = -1;
    }
    return rs;
}
