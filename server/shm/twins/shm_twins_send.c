/*
 * shm_twins_send.c
 *
 *  Created on: 14 Dec 2013
 *      Author: heracles
 */

#include "shm_twins_send.h"
#include "shm_twins_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>
static char * shm_twins_send_key;
static NTT_SHM_TWINS_DATA * shm_twins_send_seg;

int shm_twins_send_init(char * key){
	int rs  = 0;
	int n = strlen(key);
	shm_twins_send_key = (char *) malloc(n + 1);
	memset(shm_twins_send_key, 0, n + 1);
	memcpy(shm_twins_send_key, key, n);

	int shm = shm_open(shm_twins_send_key, O_CREAT|O_RDWR, NTT_SHM_FILE_MODE);
	if(shm < 0){
		if(shm_unlink(shm_twins_send_key) != 0){
			//error;
			exit(EXIT_FAILURE);
		}
		//error;
		exit(EXIT_FAILURE);
	}
	int size_seg = sizeof(NTT_SHM_TWINS_DATA);
	ftruncate((int)shm, size_seg);
	shm_twins_send_seg = ( NTT_SHM_TWINS_DATA *)mmap(0,
    		size_seg, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    if (shm_twins_send_seg == 0) {
        perror("In mmap()");
        exit(EXIT_FAILURE);
    }
    close(shm);

    pthread_mutexattr_t att;
    (void) pthread_mutexattr_init(&att);
    (void) pthread_mutexattr_setpshared(&att, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(shm_twins_send_seg->ntt_mutex), &att);


//    char * data = (char*)shm_twins_send_seg->data;
//    memset(data, 0, NTT_SHM_TWINS);
//    shm_twins_send_seg->pid_recv = getpid();
    shm_twins_send_seg->pid_send = getpid();

	return rs;
}
int shm_twins_send_sig(){
	fprintf (stderr, "--->Send a signal to send process.\n");
	int rs = 0;
	pid_t pid_recv = shm_twins_send_seg->pid_recv;
	kill(pid_recv, SIGALRM);
	return rs;
}

int shm_twins_send_write(char * key){
	fprintf (stderr, "--->Write message to shared memory\n");
	int rs= 0;
	pthread_mutex_lock(&(shm_twins_send_seg->ntt_mutex));
		char * t = shm_twins_send_seg->data;
		memset(t, 0, NTT_SHM_TWINS_SISE);
		memcpy(t, key, strlen(key));
	pthread_mutex_unlock(&(shm_twins_send_seg->ntt_mutex));
	return rs;
}
int shm_twins_send_unlink(){
	shm_unlink(shm_twins_send_key);
	return 0;
}
