#include <stdio.h>
#include <stdlib.h>
#include "def.h"
//http://pubs.opengroup.org/onlinepubs/009696799/functions/popen.html
char * gb_mq_name;
char * gb_mq_gen()
{
    FILE *fp = popen("uuidgen", "r");
    gb_mq_name = malloc(64);
    memset(gb_mq_name, 0, 64);
    //gb_mq_name[0] = '/';
    int n = strlen(MQ_SERVER_NAME);
    memcpy(gb_mq_name, MQ_SERVER_NAME, n);
    while (fgets(&gb_mq_name[n], 39, fp) != NULL){}
    pclose(fp);
    return gb_mq_name;
}
