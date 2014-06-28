#ifndef DEF_ONUS_H
#define DEF_ONUS_H
#include <errno.h>
#include "gbv.h"
#include "def_stat.h"

#ifdef __cplusplus

extern "C" {
#endif

    #define SERV_PORT                           9999
    #define MAX_BUFFER                          1024
    //#define MQ_SERVER_NAME                      "/home/thuan/MQ_SERVER_NAME/"
    #define MQ_SERVER_NAME                      "/"

    #define LISTENQ                             64
    #define CLIENT_CLOSED_CONNECT               "exit"
    #define FILE_MODE                           (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

    extern char                                 APPLICATION_RUNNING;
    extern void close_child_process();
    extern void set_app_run(char);
    extern char get_app_run();

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#ifdef __cplusplus
}
#endif

#endif // DEF_ONUS_H
