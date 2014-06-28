/*
 * shm_twins_data.h
 *
 *  Created on: 14 Dec 2013
 *      Author: heracles
 */

#ifndef __NTT_SHM_TWINS_DATA_H_
#define __NTT_SHM_TWINS_DATA_H_
#include <pthread.h>
#define NTT_SHM_TWINS_SISE 					MAX_BUFFER
typedef struct __NTT_SHM_TWINS_DATA__ {
    pthread_mutex_t ntt_mutex;
    char* data[NTT_SHM_TWINS_SISE];
    pid_t pid_recv;
    pid_t pid_send;

} NTT_SHM_TWINS_DATA;
#define NTT_SHM_FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#endif /* __NTT_SHM_TWINS_DATA_H_ */
