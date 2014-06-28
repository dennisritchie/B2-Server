/*
 * shm_twins_recv.c
 *
 *  Created on: 14 Dec 2013
 *      Author: heracles
 */

#include "shm_twins_recv.h"
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
static char * shm_twins_recv_key;

NTT_SHM_TWINS_DATA * shm_twins_recv_segment;

static void shm_twins_recv_gen_key();

static void shm_twins_recv_gen_key(){
	fprintf (stderr, "--->Generate shared memory key.\n");
    FILE *fp = popen("uuidgen", "r");
    shm_twins_recv_key = malloc(64);
    memset(shm_twins_recv_key, 0, 64);
    //gb_mq_name[0] = '/';
    int n = strlen("/");
    memcpy(shm_twins_recv_key, "/", n);
    while (fgets(&shm_twins_recv_key[n], 39, fp) != 0){}
    pclose(fp);
}
char * get_shm_twins_recv_key(){
	return shm_twins_recv_key;
}
static void shm_twins_recv_sig_handle(int sig_type);
int shm_twins_recv_init(){
	fprintf (stderr, "--->Initiate shared memory.\n");
	int rs = 0;
	shm_twins_recv_gen_key();
	int shm = shm_open(shm_twins_recv_key, O_CREAT|O_RDWR, NTT_SHM_FILE_MODE);
	if(shm < 0){
		if(shm_unlink(shm_twins_recv_key) != 0){
			//error;
			fprintf (stderr, "---> Initial shared memory: error\n");

			exit(EXIT_FAILURE);
		}
		//error;
		fprintf (stderr, "---> Initial shared memory: error\n");
		exit(EXIT_FAILURE);
	}
//	fprintf (stderr, "function shm_twins_recv_init ok shm_open\n");
	int size_seg = sizeof(NTT_SHM_TWINS_DATA);
	ftruncate((int)shm, size_seg);
    shm_twins_recv_segment = ( NTT_SHM_TWINS_DATA *)mmap(0,
    		size_seg, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    if (shm_twins_recv_segment == NULL) {
        perror("In mmap()");
        exit(EXIT_FAILURE);
    }
    close(shm);
//	fprintf (stderr, "function shm_twins_recv_init after close\n");

    pthread_mutexattr_t att;
    (void) pthread_mutexattr_init(&att);
    (void) pthread_mutexattr_setpshared(&att, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(shm_twins_recv_segment->ntt_mutex), &att);
    char * data = (char*)shm_twins_recv_segment->data;
    memset(data, 0, NTT_SHM_TWINS_SISE);
    shm_twins_recv_segment->pid_recv = (pid_t)getpid();
    shm_twins_recv_segment->pid_send = 0;
    signal(SIGALRM, &shm_twins_recv_sig_handle);
	return rs;

}
static void shm_twins_recv_sig_handle(int sig_type){
	shm_twins_recv_read();
}

char * shm_twins_recv_read(){
//	fprintf (stderr, "--->Read data from shared memory.\n");
	//Access shared memory data
	char * data = 0;
	pthread_mutex_lock(&(shm_twins_recv_segment->ntt_mutex));
		int n = strlen(shm_twins_recv_segment->data);
		data = malloc(n+1);
		memset(data, 0, n+1);
		memcpy(data, shm_twins_recv_segment->data, n);
	pthread_mutex_unlock(&(shm_twins_recv_segment->ntt_mutex));
	fprintf (stderr, "--->Read data from shared memory: \"%s\"\n", data);

	//send to client data
	if(strcmp(data, CLIENT_CLOSED_CONNECT)==0){
		sk_send_shutdown();
		shm_twins_recv_unlink();
		sleep(6);
		exit(EXIT_SUCCESS);
	}
	else{
		fprintf(stderr, "--->Send data to client.\n");
		sk_send_data(data);
		fprintf (stderr, "<------------------------------------------------------------------------>\n");
		free(data);
	}
	return 0;
}
int shm_twins_recv_unlink(){
	shm_unlink(shm_twins_recv_key);
	return 0;
}
