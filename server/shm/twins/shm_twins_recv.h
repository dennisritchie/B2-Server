/*
 * shm_twins_recv.h
 *
 *  Created on: 14 Dec 2013
 *      Author: heracles
 */

#ifndef __NTT_SHM_TWINS_RECV_H_
#define __NTT_SHM_TWINS_RECV_H_

#include "../../def.h"
#include "shm_twins_data.h"
#ifdef __cplusplus
extern "C" {
#endif
	int shm_twins_recv(const char*);
	int shm_twins_recv_init();
	int shm_twins_recv_unlink();
	char * get_shm_twins_recv_key();
	char * shm_twins_recv_read();

#ifdef __cplusplus
}
#endif




#endif /* __NTT_SHM_TWINS_RECV_H_ */
